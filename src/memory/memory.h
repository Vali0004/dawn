#pragma once
#include "pch/pch.h"
#include "logger.h"
#include "common/joaat.h"
#define STATUS_OOB_POINTER_ADDITIVE 0xE0000001

namespace cs::memory
{
	class mem
	{
	public:
		mem(void* p = nullptr)
			: m_ptr(p)
		{}
		mem(u64 p)
			: mem((void*)p)
		{}

		template <typename T>
		std::enable_if_t<std::is_pointer_v<T>, T> as()
		{
			return static_cast<T>(m_ptr);
		}
		template <typename T>
		std::enable_if_t<std::is_lvalue_reference_v<T>, T> as()
		{
			return *static_cast<base_type<T>*>(m_ptr);
		}
		template <typename T>
		std::enable_if_t<std::is_same_v<T, u64>, T> as()
		{
			return reinterpret_cast<T>(m_ptr);
		}

		mem add(u64 v)
		{
			return as<decltype(v)>() + v;
		}
		mem sub(u64 v)
		{
			return as<decltype(v)>() - v;
		}

		mem rip()
		{
			return operator bool() ? add(as<s32&>()).add(4) : mem{};
		}
		mem call()
		{
			return operator bool() ? add(1).rip() : mem{};
		}
		mem dword()
		{
			return operator bool() ? add(2).rip() : mem{};
		}
		mem qword()
		{
			return operator bool() ? add(3).rip() : mem{};
		}

		operator bool()
		{
			return m_ptr;
		}
	private:
		void* m_ptr{};
	};

	inline std::string name_manip(const std::string& name)
	{
		if (stdfs::path(name).has_extension())
		{
			return name;
		}

		if (name.empty())
		{
			return {};
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

		u64 size()
		{
			return m_size;
		}

		bool exists()
		{
			return !size();
		}

		const std::string& name()
		{
			return m_name.compare("") && !m_name.empty() ? m_name : "GTA5.exe";
		}
	protected:
		std::string m_name;
		mem m_begin;
		mem m_end;
		u64 m_size;
	};

	inline std::optional<u8> char_to_hex(const char c)
	{
		if (c >= 'a' && c <= 'f')
			return static_cast<u8>(static_cast<s32>(c) - 87);

		if (c >= 'A' && c <= 'F')
			return static_cast<u8>(static_cast<s32>(c) - 55);

		if (c >= '0' && c <= '9')
			return static_cast<u8>(static_cast<s32>(c) - 48);

		return {};
	}

	inline std::vector<std::optional<u8>> get_bytes_from_ida_mem_signature(const std::string& ptr)
	{
		std::vector<std::optional<u8>> bytes{};

		for (size_t i{}; i != ptr.size() - 1; ++i)
		{
			if (ptr[i] == ' ')
				continue;

			if (ptr[i] != '?')
			{
				if (auto c{ char_to_hex(ptr[i]) })
				{
					if (auto c2{ char_to_hex(ptr[i + 1]) })
						bytes.emplace_back(static_cast<uint8_t>((*c * 0x10) + *c2));
				}
			}
			else
				bytes.push_back({});
		}

		return bytes;
	}

	inline mem scan_bmh(std::string name, std::string ptr, hmodule module = {})
	{
		std::vector<std::optional<u8>> bytes{ get_bytes_from_ida_mem_signature(ptr) };
		s64 maxShift{ static_cast<s64>(bytes.size()) }; //signed unsigned bullshit...
		s64 maxIdx{ maxShift - 1 };

		//Get wildcard index, and store max shifable byte count
		s64 wildCardIdx{ -1 };
		for (s64 i{ maxIdx - 1 }; i >= 0; --i)
		{
			if (!bytes[i])
			{
				maxShift = maxIdx - i;
				wildCardIdx = i;
				break;
			}
		}

		//Store max shiftable bytes for non wildcards.
		s64 shiftTable[0xFF + 1]{};
		for (s64 i{}; i <= 0xFF; ++i)
			shiftTable[i] = maxShift;
		for (s64 i{ wildCardIdx + 1 }; i != maxIdx; ++i)
			shiftTable[*bytes[i]] = maxIdx - i;

		s64 curIdx{}, endIdx{ static_cast<s64>(module.size() - bytes.size()) };
		bool breakLoop{};
		mem result{};
		//Loop data
		do
		{
			try
			{
				for (s64 sigIdx{ maxIdx }; sigIdx >= 0; --sigIdx)
				{
					if (bytes[sigIdx].has_value() && *module.begin().add(curIdx + sigIdx).as<u8*>() != bytes[sigIdx].value())
					{
						curIdx += shiftTable[*module.begin().add(curIdx + maxIdx).as<u8*>()];
						break;
					}

					if (!sigIdx)
					{
						result = module.begin().add(curIdx), breakLoop = true;
						break;
					}
				}
			}
			catch (...)
			{}
		} while (curIdx != endIdx && !breakLoop);

		g_running = result;

		return result;
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