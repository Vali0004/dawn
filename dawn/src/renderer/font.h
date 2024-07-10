#pragma once
#include <vector>
#include <stdio.h>
#include "vectors.h"
#include "d3d/shader_resource_view.h"
#include "d3d/texture_2d.h"
#ifndef STB_RECT_PACK_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"
#endif
#ifndef STB_TRUETYPE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"
#endif
#define iFloor(x) static_cast<float>(static_cast<int>(x))

namespace dwn::directx
{
	class font_character
	{
	public:
		// Both position and size cannot be precalculated. However, the texture coords can be.
		// We need to either do OTF calculation, or precalc then append data on. The first option will actually be faster.
		Vector4 top_bottom(Vector2 pos)
		{
			const stbtt_packedchar b{ m_packed_character };
			float x = (float)STBTT_ifloor((pos.x + b.xoff) + 0.5f);
			float y = (float)STBTT_ifloor((pos.y + b.yoff) + 0.5f);
			return { { x, y }, { x + b.xoff2 - b.xoff, y + b.yoff2 - b.yoff } };
		}
		Vector4 left_right()
		{
			return Vector4{ m_quad.s0, m_quad.s1, m_quad.t0, m_quad.t1 };
		}
		
		char m_character{};
		stbtt_packedchar m_packed_character{};
		stbtt_aligned_quad m_quad{};
		Vector2 m_codepoint_metrics{};
		Vector4 m_codepoint_bitmap_boxes{};
	};

	class font
	{
	public:
		font()
		{
			m_first_codepoint = 32;
			m_num_characters = 96;
			m_characters.resize(m_num_characters);
		}

		stbtt_fontinfo m_font_info{};
		float m_scale{};
		float m_font_size{};
		int m_ascent{}, m_descent{}, m_line_gap{};
		uint8_t* m_ttf_buffer{};
		Vector2 m_texture_size{};
		uint32_t* m_texture_buffer{};
		shader_resource_view m_texture{};

		uint8_t m_first_codepoint{};
		uint8_t m_num_characters{};
		std::vector<font_character> m_characters{};
	};

	template <typename TOut = uint32_t, typename TIn = uint8_t>
	inline void rebake_as_rgba(TOut*& outPixels, TIn* inPixels, size_t display_size, uint8_t& bpp)
	{
		bpp = sizeof(TOut);
		outPixels = reinterpret_cast<TOut*>(malloc(display_size * sizeof(TOut)));
		if (outPixels)
		{
			for (size_t i{ display_size }; i; --i)
			{
				Color c{ 255, 255, 255, inPixels[i] };
				outPixels[i] = c.pack();
			}
		}
	}

	inline stbtt_aligned_quad get_packed_quad(font& f, stbtt_packedchar& packedChar, Vector2 pos)
	{
		stbtt_aligned_quad q{};

		int pixel_width{ static_cast<int>(f.m_texture_size.x) }, pixel_height{ static_cast<int>(f.m_texture_size.y) };

		stbtt_GetPackedQuad(&packedChar, pixel_width, pixel_height, 0, &pos.x, &pos.y, &q, TRUE);

		return q;
	}

	inline Vector2 get_codepoint_metrics(font& f, char c)
	{
		int advance_width{}, left_side_bearing{};

		stbtt_GetCodepointHMetrics(&f.m_font_info, c, &advance_width, &left_side_bearing);

		return { advance_width, left_side_bearing };
	}
	inline Vector4 get_codepoint_bitmap_box(font& f, char c)
	{
		int x0{}, y0{}, x1{}, y1{};

		stbtt_GetCodepointBitmapBox(&f.m_font_info, c - f.m_first_codepoint, f.m_scale, f.m_scale, &x0, &y0, &x1, &y1);

		return { x0, y0, x1, y1 };
	}

	inline Vector2 get_string_size(font& f, const char* str)
	{
		Vector2 size{};

		for (size_t i{}; str[i] >= f.m_first_codepoint && str[i] < (f.m_first_codepoint + f.m_num_characters); ++i)
		{		
			const font_character& c{ f.m_characters[str[i] - f.m_first_codepoint] };
			size.x += (c.m_codepoint_metrics.x * f.m_scale);
		}

		return { iFloor(size.x), iFloor((static_cast<float>(f.m_descent) * (f.m_scale * 2.f))) };
	}

	inline font load_font(const char* fontPath, float fontSize, device& device)
	{
		font f{};
		f.m_font_size = fontSize;
		FILE* font_file{};
		fopen_s(&font_file, fontPath, "rb");

		if (!font_file)
		{
			return {};
		}

		fseek(font_file, 0, SEEK_END);
		long size{ ftell(font_file) };
		fseek(font_file, 0, SEEK_SET);

		f.m_ttf_buffer = (uint8_t*)malloc(size);
		fread(f.m_ttf_buffer, 1, size, font_file);

		fclose(font_file);

		if (!stbtt_InitFont(&f.m_font_info, f.m_ttf_buffer, stbtt_GetFontOffsetForIndex(f.m_ttf_buffer, 0)))
		{
			free(f.m_ttf_buffer);
			return {};
		}

		f.m_scale = stbtt_ScaleForMappingEmToPixels(&f.m_font_info, f.m_font_size);
		switch (static_cast<int>(f.m_scale * 100.f))
		{
			case 0:
			{
				f.m_texture_size = { 512.f, 512.f };
			} break;
			case 1:
			{
				f.m_texture_size = { 1024.f, 1024.f };
			} break;
			case 2:
			{
				f.m_texture_size = { 2048.f, 2048.f };
			} break;
			case 3:
			default:
			{
				f.m_texture_size = { 4096.f, 4096.f };
			} break;
		}

		int pixel_width{ static_cast<int>(f.m_texture_size.x) }, pixel_height{ static_cast<int>(f.m_texture_size.y) };

		size_t display_size{ static_cast<size_t>(pixel_width * pixel_height) };

		uint8_t* p_buffer{ reinterpret_cast<uint8_t*>(malloc(display_size)) };

		if (!p_buffer)
		{
			return {};
		}

		memset(p_buffer, 0, display_size);

		std::vector<stbtt_packedchar> f_data{};
		f_data.resize(f.m_num_characters);

		stbtt_pack_context pack_context{};
		stbtt_PackBegin(&pack_context, p_buffer, pixel_width, pixel_height, 0, 1, nullptr);
		stbtt_PackSetOversampling(&pack_context, 2, 1);
		stbtt_pack_range ranges[1]{};
		ranges[0].chardata_for_range = f_data.data();
		ranges[0].array_of_unicode_codepoints = 0;
		ranges[0].first_unicode_codepoint_in_range = f.m_first_codepoint;
		ranges[0].num_chars = f.m_num_characters;
		ranges[0].font_size = STBTT_POINT_SIZE(f.m_font_size);
		stbtt_PackFontRanges(&pack_context, f.m_ttf_buffer, 0, ranges, sizeof(ranges) / sizeof(*ranges));
		stbtt_PackEnd(&pack_context);

		stbtt_GetFontVMetrics(&f.m_font_info, &f.m_ascent, &f.m_descent, &f.m_line_gap);

		for (size_t i{}; i != f.m_characters.size(); ++i)
		{
			auto& c{ f.m_characters[i] };
			c.m_character = static_cast<char>(i + f.m_first_codepoint);
			c.m_packed_character = f_data[i];
			c.m_quad = get_packed_quad(f, c.m_packed_character, {});
			c.m_codepoint_metrics = get_codepoint_metrics(f, c.m_character);
			c.m_codepoint_bitmap_boxes = get_codepoint_bitmap_box(f, c.m_character);
		}

		f_data.clear();

		uint8_t bpp{ 1 };
		rebake_as_rgba(f.m_texture_buffer, p_buffer, display_size, bpp);

		texture_2d texture{};
		texture.create(device, { f.m_texture_size.x, f.m_texture_size.y }, f.m_texture_buffer, DXGI_FORMAT_R8G8B8A8_UNORM, bpp);
		f.m_texture.create(device, texture);
		texture.release();

		free(p_buffer);

		return f;
	}

	inline void destroy_font(font& f)
	{
		f.m_characters.clear();
		free(f.m_ttf_buffer);
	}
}