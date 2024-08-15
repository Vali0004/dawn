#pragma once
#include "pch/pch.h"
#include "logger.h"
#include "common/joaat.h"
#define STATUS_OOB_POINTER_ADDITIVE 0xE0000001

namespace dwn::memory
{
	class mem
	{
	public:
		mem(void* p = nullptr)
			: m_ptr(p)
		{}
		mem(u64 p)
			: mem(reinterpret_cast<void*>(p))
		{}

		template <typename T>
		std::enable_if_t<std::is_pointer_v<T>, T> as()
		{
			return static_cast<T>(m_ptr);
		}
		template <typename T>
		std::enable_if_t<std::is_lvalue_reference_v<T>, T> as()
		{
			return *static_cast<std::remove_reference_t<T>*>(m_ptr);
		}
		template <typename T>
		std::enable_if_t<std::is_same_v<T, u64>, T> as()
		{
			return reinterpret_cast<T>(m_ptr);
		}

		nodisc mem add(u64 v)
		{
			likely if (m_ptr)
			{
				return as<decltype(v)>() + v;
			}

			return mem{};
		}
		nodisc mem sub(u64 v)
		{
			likely if (m_ptr)
			{
				return as<decltype(v)>() - v;
			}

			return mem{};
		}

		nodisc mem rip()
		{
			likely if (m_ptr)
			{
				return add(as<s32&>()).add(4);
			}

			return mem{};
		}
		nodisc mem call()
		{
			likely if (m_ptr)
			{
				return add(1).rip();
			}

			return mem{};
		}
		nodisc mem dword()
		{
			likely if (m_ptr)
			{
				return add(2).rip();
			}

			return mem{};
		}
		nodisc mem qword()
		{
			likely if (m_ptr)
			{
				return add(3).rip();
			}

			return mem{};
		}

		nodisc operator bool() const
		{
			return m_ptr;
		}
	private:
		void* m_ptr{};
	};

	inline std::string name_manip(const std::string& name)
	{
		likely if (name.empty())
		{
			return {};
		}

		unlikely if (std::fs::path(name).has_extension())
		{
			return name;
		}

		return name + ".dll";
	}

	class hmodule
	{
	public:
		hmodule(const std::string& name) :
			m_name(name_manip(name)),
			m_begin(GetModuleHandleA(m_name.empty() ? nullptr : m_name.c_str())),
			m_size(m_begin ? m_begin.add(m_begin.as<IMAGE_DOS_HEADER*>()->e_lfanew).as<IMAGE_NT_HEADERS*>()->OptionalHeader.SizeOfImage : 0),
			m_end(m_begin.add(m_size))
		{}
		hmodule()
			: hmodule(std::string{})
		{}

		mem begin()
		{
			return m_begin;
		}
		mem end()
		{
			return m_end;
		}

		nodisc const u64 size()
		{
			return m_size;
		}

		nodisc const bool exists()
		{
			return size();
		}

		const std::string& name()
		{
			unlikely if (m_name.compare("") && !m_name.empty())
			{
				return m_name;
			}

			return "GTA5.exe";
		}
	protected:
		std::string m_name;
		mem m_begin;
		mem m_end;
		u64 m_size;
	};

	typedef std::optional<u8> byte_t;

	inline byte_t char_to_hex(const char c)
	{
		if (c >= 'a' && c <= 'f')
			return static_cast<u8>(static_cast<s32>(c) - 87);

		if (c >= 'A' && c <= 'F')
			return static_cast<u8>(static_cast<s32>(c) - 55);

		if (c >= '0' && c <= '9')
			return static_cast<u8>(static_cast<s32>(c) - 48);

		return {};
	}

	inline std::vector<byte_t> get_bytes_from_ida_mem_signature(const std::string& ptr)
	{
		std::vector<byte_t> bytes{};

		for (size_t i{}; i != ptr.size() - 1; ++i)
		{
			likely if (ptr[i] == ' ')
			{
				continue;
			}

			if (ptr[i] != '?')
			{
				likely if (auto c{ char_to_hex(ptr[i]) })
				{
					likely if (auto c2{ char_to_hex(ptr[i + 1]) })
					{
						bytes.emplace_back(static_cast<u8>((*c * 0x10) + *c2));
					}
				}
			}
			else
			{
				bytes.push_back(std::nullopt);
			}
		}

		return bytes;
	}
	
	inline mem scan_bmh(std::string name, std::string pattern, hmodule module = {})
	{
		const std::vector<byte_t> bytes{ get_bytes_from_ida_mem_signature(pattern) };
		const u64 bytes_size{ bytes.size() };
		s64 max_shift{ static_cast<s64>(bytes_size) };
		s64 max_idx{ max_shift - 1 };

		// Get wildcard index, and store max shifable byte count
		s64 wild_card_idx{ -1 };
		for (s64 i{ max_idx - 1 }; i >= 0; --i)
		{
			if (!bytes[i])
			{
				max_shift = max_idx - i;
				wild_card_idx = i;
				break;
			}
		}

		// Store max shiftable bytes for non wildcards.
		std::array<s64, 0x100> shift_table{};
		shift_table.fill(max_shift);
		for (s64 i{ wild_card_idx + 1 }; i != max_idx; ++i)
		{
			shift_table[bytes[i].value()] = max_idx - i;
		}

		// Start search
		const u64 scan_end{ module.end() - bytes_size };
		for (auto it{ module.begin() }; it <= scan_end;)
		{
			s64 sig_idx{ max_idx };
			while (sig_idx >= 0)
			{
				likely if (bytes[sig_idx].has_value() && it.add(sig_idx).as<u8&>() != bytes[sig_idx].value())
				{
					break;
				}
				--sig_idx;
			}

			if (sig_idx < 0)
			{
				return it;
			}

			it = it.add(shift_table[it.add(max_idx).as<u8&>()]);
		}

		g_running = false;

		return {};
	}

	inline bool does_memory_match(u8* target, std::optional<u8> const* sig, u64 len)
	{
		for (u64 i{ len }; i; --i)
			if (sig[i] && *sig[i] != target[i])
				return false;

		return true;
	}

	inline std::vector<u64> get_all_results(std::string ptr, hmodule module = {})
	{
		std::vector<std::optional<u8>> bytes{ get_bytes_from_ida_mem_signature(ptr) };
		std::vector<u64> results{};

		for (u64 i{}; i != module.size() - bytes.size(); ++i)
		{
			if (does_memory_match(module.begin().add(i).as<u8*>(), bytes.data(), bytes.size()))
			{
				results.push_back(module.begin().add(i).as<u64>());
			}
		}

		return results;
	}

	enum class eMemoryOperation : s8
	{
		rip,
		qword,
		dword,
		call
	};
	enum class ePointerThreadState : s8
	{
		invalid = -1,
		killed,
		paused,
		active
	};

	class memory_operand
	{
	public:
		memory_operand(const std::string& name, const std::string& signature, const std::string& expression, hmodule hmodule) :
			m_name(name), m_signature(signature), m_expression(expression),
			m_module(hmodule)
		{}
		memory_operand() = default;

		void handle_instruction(std::string str)
		{
			switch (atStringHash(str))
			{
				case "rip"_j:
				{
					m_instructions.push_back(static_cast<s8>(eMemoryOperation::rip));
				} break;
				case "qword"_j:
				{
					m_instructions.push_back(static_cast<s8>(eMemoryOperation::qword));
				} break;
				case "dword"_j:
				{
					m_instructions.push_back(static_cast<s8>(eMemoryOperation::dword));
				} break;
				case "call"_j:
				{
					m_instructions.push_back(static_cast<s8>(eMemoryOperation::call));
				} break;
				default:
				{
					// Fingerprint operator
					bool is_negative{};
					if (str.at(0) == '+')
					{
						str = str.substr(1);
					}
					else if (str.at(0) == '-')
					{
						is_negative = true;
						str = str.substr(1);
					}

					if (fingerprint_hex(str))
					{
						s64 value = std::strtoll(str.data(), 0, 16);
						if (value > INT8_MAX || value < INT8_MIN)
						{
							RaiseException(STATUS_OOB_POINTER_ADDITIVE, NULL, NULL, nullptr);
							return;
						}

						if (is_negative)
						{
							value = -value;
						}

						m_instructions.push_back(static_cast<s8>(value));
					}
				} break;
			}
		}

		void parse_expression()
		{
			std::vector<std::string> strings{ split_string(m_expression, ' ') };

			if (!strings.empty())
			{
				for (const auto& s : strings)
				{
					std::string str{ s };
					// Strip commas
					strip_character(str, ',');
					handle_instruction(str);
				}
			}
		}

		void parse()
		{
			parse_expression();

			m_result = scan_bmh(m_name, m_signature, m_module);

			if (!m_instructions.empty())
			{
				for (u64 i{}; i != m_instructions.size(); ++i)
				{
					switch (static_cast<eMemoryOperation>(m_instructions[i]))
					{
						case eMemoryOperation::rip:
						{
							m_result = m_result.rip();
						} break;
						case eMemoryOperation::dword:
						{
							m_result = m_result.dword();
						} break;
						case eMemoryOperation::qword:
						{
							m_result = m_result.qword();
						} break;
						case eMemoryOperation::call:
						{
							m_result = m_result.call();
						} break;
						default:
						{
							m_result = m_result.add(m_instructions[i]);
						} break;
					}
				}
			}
		}

		mem get()
		{
			return m_result;
		}

		hmodule module()
		{
			return m_module;
		}

		std::string m_name{};
	private:
		std::string m_signature{};
		std::string m_expression{};
		hmodule m_module{};
		mem m_result{};
		std::vector<s8> m_instructions{};
	};
	class pointer
	{
	public:
		pointer(const std::string& name, const std::string& sig, const std::string& expression, hmodule hmodule)
			: m_memory(name, sig, expression, hmodule)
		{}
		pointer() = default;

		void create_thread()
		{
			std::thread([&] {
				if (valid())
					return;

				m_state = ePointerThreadState::active;
				m_memory.parse();
				m_state = ePointerThreadState::killed;

			}).detach();
		}

		bool valid()
		{
			return m_state == ePointerThreadState::killed;
		}

		std::string name()
		{
			return m_memory.m_name;
		}

		operator mem()
		{
			return m_memory.get();
		}

		memory_operand get()
		{
			return m_memory;
		}

		u64 address()
		{
			return m_memory.get().as<u64>();
		}

		u64 relative()
		{
			return m_memory.get().sub(m_memory.module().begin().as<u64>());
		}
	private:
		memory_operand m_memory{};
		ePointerThreadState m_state{ ePointerThreadState::invalid };
	};
	class pointer_manager
	{
	public:
		void add(const std::string& name, const std::string& signature, const std::string& expression = {}, hmodule hmodule = {})
		{
			m_pointers.insert(std::make_pair(name, pointer(name, signature, expression, hmodule)));
		}

		template <typename T>
		void add(const std::string& name, T& ptr, const std::string& signature, const std::string& expression = {}, hmodule hmodule = {})
		{
			m_pointer_instances.insert(std::make_pair(name, (void**)&ptr));
			m_pointers.insert(std::make_pair(name, pointer(name, signature, expression, hmodule)));
		}

		template <typename T>
		void get(const std::string& name, T& value)
		{
			value = get(name).as<T>();
		}

		mem get(const std::string& name)
		{
			for (auto& p : m_pointers)
			{
				if (!p.second.name().compare(name))
				{
					return mem(p.second);
				}
			}

			return mem{};
		}

		void run()
		{
			if (m_intialised)
				return;

			m_intialised = true;

			for (auto& p : m_pointers)
			{
				p.second.create_thread();
			}
		}

		bool finished()
		{
			if (!m_intialised)
			{
				run();
			}

			for (auto& p : m_pointers)
			{
				if (!p.second.valid())
					return false;
			}

			return true;
		}

		void print()
		{
			for (auto& p : m_pointers)
			{
				LOG_TO_STREAM("Found " << p.second.name() << " at address " << HEX(p.second.address()));
			}
		}

		void get_all()
		{
			for (auto& p_instance : m_pointer_instances)
			{
				if (auto& p{ m_pointers[p_instance.first] }; p.valid())
				{
					//LOG_TO_STREAM("Setting pointer " << p_instance.first << "(Pointer module address: " << HEX((u64)p_instance.second) << ") to " << HEX(p.address()));
					*p_instance.second = reinterpret_cast<void*>(p.address());
				}
			}
		}

		void clear()
		{
			m_pointers.clear();
			m_pointer_instances.clear();
		}
	private:
		bool m_intialised{};
		std::unordered_map<std::string, pointer> m_pointers{};
		std::unordered_map<std::string, void**> m_pointer_instances{};
	};
}