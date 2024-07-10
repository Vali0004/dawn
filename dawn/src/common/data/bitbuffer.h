#pragma once
#include <cstdint>
#include <vcruntime_string.h>
#define to_byte_s(v) (((v)) >> 3)
#define to_byte(v) to_byte_s(v+7)
#define to_bit(v) ((v) << 3)
#define in_range(v, a, b) (v >= a && v <= b)
#define max_uint ~0u

namespace rage {
    template <typename T>
    struct ValueTraits {
        static const int NumBytes = sizeof(T);
        static const int NumBits = NumBytes * 8;
    };
	// Their bitbuffer impl is garbage. (why didn't they use a fucking byte buffer?)
	class datBitBuffer {
    public:
        void reset() {
            *(datBitBuffer*)(this) = datBitBuffer();
        }

        bool read_only() const { return m_readonly; }
        bool null_write() const { return m_null_write; }
        bool writable() const { return !read_only() && get_max_bits() > 0; }
        bool can_write_bits(const int numBits) const { return !read_only() && (get_cursor_pos() + numBits) <= get_max_bits(); }
        bool can_read_bits(const int numBits) const { return !null_write() && (get_cursor_pos() + numBits) <= get_bit_len(); }
        bool can_read_bytes(const int numBytes) const { return can_read_bits(to_bit(numBytes)); }
        bool can_write_bytes(const int numBytes) const { return can_write_bits(to_bit(numBytes)); }
        int get_num_bytes_written() const { return to_byte(m_num_bits_written); }
        int get_num_bits_written() const { return m_num_bits_written; }
        int get_num_bytes_read() const { return to_byte(m_num_bits_read); }
        int get_num_bits_read() const { return m_num_bits_read; }
        int get_bit_len() const { return read_only() ? get_max_bits() : get_num_bits_written(); }
        int get_byte_len() const { return to_byte(get_bit_len()); }
        int get_max_bits() const { return m_max; }
        int get_base_bit_offset() const { return m_base_bit_offset; }
        int get_cursor_pos() const { return m_cursor; }
        bool set_cursor_pos(const int pos) {
            if (in_range(pos, 0, get_bit_len())) {
                m_cursor = pos;
                return true;
            }
            return false;
        }
        void bump_cursor(const int delta) { m_cursor += delta; }
        void bump_read_cursor(const int delta) { bump_cursor(delta); m_num_bits_read = m_cursor > m_num_bits_read ? m_cursor : m_num_bits_read; }
        void bump_write_cursor(const int delta) { bump_cursor(delta); m_num_bits_written = m_cursor > m_num_bits_written ? m_cursor : m_num_bits_written; }
        bool read_bits(void* dst, const int numBits, const int bitOffset) {
            if (numBits && can_read_bits(numBits)) {
                copy_bits((uint8_t*)dst, &m_buffer[to_byte_s(get_base_bit_offset())], numBits, bitOffset, m_cursor + (m_base_bit_offset & 7));
                bump_read_cursor(numBits);
                return true;
            }
            return false;
        }
        bool read_uns(uint32_t& value, const int numBits) {
            bool success = false;
            if (can_read_bits(numBits)) {
                read_uns(m_buffer, value, numBits, m_cursor + m_base_bit_offset);
                bump_read_cursor(numBits);
                success = true;
            }
            else {
                value = 0;
            }
            return success;
        }
        bool read_uns(uint64_t& value, const int numBits) {
            bool success = false;
            static const int num_uns_bits = ValueTraits<uint32_t>::NumBits;
            uint32_t hi = 0, lo = 0;
            success = numBits > num_uns_bits ? (read_uns(lo, numBits) && read_uns(hi, numBits - num_uns_bits)) : read_uns(lo, numBits);
            if (success) {
                value = numBits > num_uns_bits ? (uint64_t(hi) << num_uns_bits) | lo : lo;
            }
            return success;
        }
        bool write_uns(const uint64_t value, const int numBits) {
            static const int num_uns_bits = ValueTraits<uint32_t>::NumBits;
            return numBits > num_uns_bits ?
                write_uns(uint32_t(value), num_uns_bits) && write_uns(uint32_t(value >> num_uns_bits), numBits - num_uns_bits) :
                write_uns(uint32_t(value), numBits);
        }
        bool write_bits(void* src, const int numBits, const int bitOffset) {
            if (numBits && can_write_bits(numBits)) {
                if (!null_write()) {
                    copy_bits(&m_buffer[to_byte_s(get_base_bit_offset())], (uint8_t*)src, numBits, m_cursor + (m_base_bit_offset & 7), bitOffset);
                }
                bump_read_cursor(numBits);
                return true;
            }
            return false;
        }
        bool write_uns(const uint32_t value, const int numBits) {
            bool success = false;
            if (can_write_bits(numBits)) {
                if (!null_write()) {
                    write_uns(m_buffer, value, numBits, m_cursor + m_base_bit_offset);
                }
                bump_write_cursor(numBits);
                success = true;
            }
            return success;
        }
    private:
        static void read_uns(uint8_t* bits, uint32_t& value, const int numBits, const int bitOffset) {
            const int first_byte = to_byte_s(bitOffset);
            uint8_t* src = &bits[first_byte];
            const int bit = bitOffset & 7;
            int cur_byte = 0;
            const int last_byte = ((bitOffset + ((numBits > 0) ? numBits - 1 : 0)) >> 3) - first_byte;
            uint32_t val = uint8_t(src[cur_byte++] << bit);
            for (int i = 8; i < numBits && cur_byte <= last_byte; i += 8) {
                val = (val << 8) | (src[cur_byte++] << bit);
            }
            if (cur_byte <= last_byte) {
                val |= src[cur_byte] >> (8 - bit);
            }
            val >>= ((numBits + 7) & ~7) - numBits;
            value = val;
        }
        static void write_uns(uint8_t* bits, const uint32_t value, const int numBits, const int bitOffset) {
            const int max_bits = ValueTraits<uint32_t>::NumBits;
            const uint32_t top8_bits = max_bits - 8;
            uint8_t* dst = &bits[to_byte_s(bitOffset)];
            const int bit = bitOffset & 7;
            uint32_t val = value << (max_bits - numBits);
            uint32_t dstMask = (max_uint << (max_bits - numBits));
            *dst = uint8_t((*dst & ~((dstMask >> top8_bits) >> bit)) | ((val >> top8_bits) >> bit));
            dst++;
            val <<= (8 - bit);
            dstMask <<= (8 - bit);
            for (int i = (8 - bit); i != numBits; i += 8) {
                *dst = uint8_t((*dst & ~(dstMask >> top8_bits)) | (val >> top8_bits));
                val <<= 8;
                dstMask <<= 8;
            }
        }
        static void copy_bits(uint8_t* dst, const uint8_t* src, const int numBits, const int dstBitOffset, const int srcBitOffset) {
            const uint8_t* s = &src[to_byte_s(srcBitOffset)];
            int cur_dst_bit = dstBitOffset;
            const int src_bit = srcBitOffset & 7;
            int bit_count = numBits;
            if (src_bit) {
                int num_bits_to_write = 8 - src_bit;
                if(num_bits_to_write > numBits)
                    num_bits_to_write = numBits;
                write_uns(dst, uint32_t((0xFF & (*s << src_bit)) >> (8 - num_bits_to_write)), num_bits_to_write, cur_dst_bit);
                ++s;
                bit_count -= num_bits_to_write;
                cur_dst_bit += num_bits_to_write;
            }
            if (bit_count) {
                const int num_bytes = to_byte_s(bit_count);
                if (num_bytes) {
                    uint8_t* d = &dst[to_byte_s(cur_dst_bit)];
                    const int bit = cur_dst_bit & 7;
                    if (bit) {
                        const int upshift = 8 - bit, downshift = bit;
                        const uint32_t upmask = 0xFF << upshift, downmask = 0xFF >> downshift;
                        for (int i = 0; i < num_bytes; ++i, ++s) {
                            *d = (*d & upmask) | (*s >> downshift);
                            ++d;
                            *d = (*d & downmask) | (*s << upshift);
                        }
                    }
                    else {
                        memcpy(d, s, num_bytes);
                        s += num_bytes;
                    }
                    cur_dst_bit += num_bytes << 3;
                    const int leftover = bit_count & 0x07;
                    if (leftover) {
                        write_uns(dst, *s >> (8 - leftover), leftover, cur_dst_bit);
                    }
                }
                else {
                    write_uns(dst, *s >> (8 - bit_count), bit_count, cur_dst_bit);
                }
            }
        }
    protected:
        uint8_t* m_buffer;
        int m_base_bit_offset;
        int m_max;
        int m_cursor;
        int m_num_bits_written;
        int m_num_bits_read;
        bool m_readonly   : 1;
        bool m_null_write  : 1;
	};
}
