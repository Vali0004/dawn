#pragma once
#include "minhook/hde/hde64.h"
#include "pch/framework.h"
#include "logger.h"
#include "amd64_disassembler.h"
#include "hooking/etc/detour.h"
#include "hooking/renderer.h"

namespace dwn::exception::etc
{
	inline std::string determine_module(u64 addr)
	{
		HMODULE mod{};
		if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, reinterpret_cast<LPCSTR>(addr), &mod))
		{
			char moduleName[MAX_PATH]{};
			if (GetModuleFileNameA(mod, moduleName, MAX_PATH) > 0)
			{
				std::string wideModuleName(moduleName);
				return moduleName;
			}
		}

		// This rarely fucking works, I cannot be fucked to fix it.
		return "unknown";
	}

	class exception_data
	{
	public:
		exception_data(s64 t, std::string_view r)
			: m_type(t), m_reason(r)
		{}

		std::string what()
		{
			return m_reason.data();
		}

		s64 value()
		{
			return m_type;
		}

		operator bool()
		{
			return !what().empty();
		}
	protected:
		s64 m_type{};
		std::string_view m_reason{};
	};

	class exception_context
	{
	public:
		exception_context(EXCEPTION_POINTERS* exception) :
			m_record(*exception->ExceptionRecord),
			m_context(*exception->ContextRecord),
			m_type(m_record.ExceptionInformation[0]),
			m_deathAddress(m_record.ExceptionInformation[1])
		{
			m_code = m_record.ExceptionCode;
			m_exceptionAddress = (uint64_t)m_record.ExceptionAddress;
		}

		void print_registers()
		{
			LOG(Registers, "RAX: 0x{:X} || RSI: 0x{:X}", m_context.Rax, m_context.Rsi);
			LOG(Registers, "RBX: 0x{:X} || RDI: 0x{:X}", m_context.Rbx, m_context.Rdi);
			LOG(Registers, "RCX: 0x{:X} || RBP: 0x{:X}", m_context.Rcx, m_context.Rbp);
			LOG(Registers, "RDX: 0x{:X} || RSP: 0x{:X}", m_context.Rdx, m_context.Rsp);
			LOG(Registers, "R8:  0x{:X} || R9:  0x{:X}", m_context.R8, m_context.R9);
			LOG(Registers, "R10: 0x{:X} || R11: 0x{:X}", m_context.R10, m_context.R11);
			LOG(Registers, "R12: 0x{:X} || R13: 0x{:X}", m_context.R12, m_context.R13);
			LOG(Registers, "R14: 0x{:X} || R15: 0x{:X}", m_context.R14, m_context.R15);
		}
	public:
		u32 m_code{};
		u64 m_exceptionAddress{};
		EXCEPTION_RECORD& m_record;
		CONTEXT& m_context;
		u64 m_type{};
		u64 m_deathAddress{};
		std::string m_fileoffset{};
	};

	#define PAIR(v) exception_data(v, #v)

	inline std::array<exception_data, 62> g_exception_types
	{
		PAIR(STATUS_WAIT_0),
		PAIR(STATUS_ABANDONED_WAIT_0),
		PAIR(STATUS_USER_APC),
		PAIR(STATUS_TIMEOUT),
		PAIR(STATUS_PENDING),
		PAIR(DBG_EXCEPTION_HANDLED),
		PAIR(DBG_CONTINUE),
		PAIR(STATUS_SEGMENT_NOTIFICATION),
		PAIR(STATUS_FATAL_APP_EXIT),
		PAIR(DBG_REPLY_LATER),
		PAIR(DBG_TERMINATE_THREAD),
		PAIR(DBG_TERMINATE_PROCESS),
		PAIR(DBG_CONTROL_C),
		PAIR(DBG_PRINTEXCEPTION_C),
		PAIR(DBG_RIPEXCEPTION),
		PAIR(DBG_CONTROL_BREAK),
		PAIR(DBG_PRINTEXCEPTION_WIDE_C),
		PAIR(DBG_COMMAND_EXCEPTION),
		PAIR(STATUS_GUARD_PAGE_VIOLATION),
		PAIR(STATUS_DATATYPE_MISALIGNMENT),
		PAIR(STATUS_BREAKPOINT),
		PAIR(STATUS_SINGLE_STEP),
		PAIR(STATUS_LONGJUMP),
		PAIR(STATUS_UNWIND_CONSOLIDATE),
		PAIR(DBG_EXCEPTION_NOT_HANDLED),
		PAIR(STATUS_ACCESS_VIOLATION),
		PAIR(STATUS_IN_PAGE_ERROR),
		PAIR(STATUS_INVALID_HANDLE),
		PAIR(STATUS_INVALID_PARAMETER),
		PAIR(STATUS_NO_MEMORY),
		PAIR(STATUS_ILLEGAL_INSTRUCTION),
		PAIR(STATUS_NONCONTINUABLE_EXCEPTION),
		PAIR(STATUS_INVALID_DISPOSITION),
		PAIR(STATUS_ARRAY_BOUNDS_EXCEEDED),
		PAIR(STATUS_FLOAT_DENORMAL_OPERAND),
		PAIR(STATUS_FLOAT_DIVIDE_BY_ZERO),
		PAIR(STATUS_FLOAT_INVALID_OPERATION),
		PAIR(STATUS_FLOAT_OVERFLOW),
		PAIR(STATUS_FLOAT_STACK_CHECK),
		PAIR(STATUS_FLOAT_UNDERFLOW),
		PAIR(STATUS_INTEGER_DIVIDE_BY_ZERO),
		PAIR(STATUS_INTEGER_OVERFLOW),
		PAIR(STATUS_PRIVILEGED_INSTRUCTION),
		PAIR(STATUS_STACK_OVERFLOW),
		PAIR(STATUS_DLL_NOT_FOUND),
		PAIR(STATUS_ORDINAL_NOT_FOUND),
		PAIR(STATUS_ENTRYPOINT_NOT_FOUND),
		PAIR(STATUS_CONTROL_C_EXIT),
		PAIR(STATUS_DLL_INIT_FAILED),
		PAIR(STATUS_CONTROL_STACK_VIOLATION),
		PAIR(STATUS_FLOAT_MULTIPLE_FAULTS),
		PAIR(STATUS_REG_NAT_CONSUMPTION),
		PAIR(STATUS_HEAP_CORRUPTION),
		PAIR(STATUS_STACK_BUFFER_OVERRUN),
		PAIR(STATUS_INVALID_CRUNTIME_PARAMETER),
		PAIR(STATUS_ASSERTION_FAILURE),
		PAIR(STATUS_ENCLAVE_VIOLATION),
		PAIR(STATUS_INTERRUPTED),
		PAIR(STATUS_THREAD_NOT_RUNNING),
		PAIR(STATUS_ALREADY_REGISTERED),
		PAIR(STATUS_SXS_EARLY_DEACTIVATION),
		PAIR(STATUS_SXS_INVALID_DEACTIVATION)
	};

	inline std::array<exception_data, 3> g_exception_error_reasons
	{
		exception_data(0, "a attempt to read an invalid address"),
		exception_data(1, "a attempt to write to an invalid address"),
		exception_data(8, "an data exception prevention (DEP)")
	};

	inline std::string get_exception_type(ULONG_PTR type)
	{
		if (g_exception_error_reasons[type])
		{
			return g_exception_error_reasons[type].what();
		}

		return "unknown";
	}

	inline u8 get_ins_length(u8* code)
	{
		u8* start{ code };
		hde64s hde{};

		if (u8 len{ static_cast<u8>(hde64_disasm(reinterpret_cast<void*>(code), &hde)) })
		{
			return len;
		}
		else if (x64::disassembleInstructionCode(code).isValid())
		{
			return static_cast<u8>(code - start);
		}

		return NULL;
	}
}

namespace dwn::exception
{
	inline void* g_handle{};
	inline LPTOP_LEVEL_EXCEPTION_FILTER g_filter_handle{};

	inline bool attempt_stack_recovery(PEXCEPTION_POINTERS exceptionInfo)
	{
		PCONTEXT ctx{ exceptionInfo->ContextRecord };
		if (const auto len{ etc::get_ins_length(reinterpret_cast<u8*>(ctx->Rip)) })
		{
			ctx->Rip += len;
			return true;
		}

		return false;
	}

	inline bool handle_module_exception(u32 status_code)
	{
		switch (status_code)
		{
			case STATUS_OOB_POINTER_ADDITIVE:
			{
				LOG(Exception, "Exception caused by our module. Invalid pointer addtive (Out Of Bounds)");
				g_running = false;
				return true;
			} break;
		}

		return false;
	}

	inline LONG WINAPI handle_exception_inner(PEXCEPTION_POINTERS info)
	{
		etc::exception_context ctx{ info };

		if (!ctx.m_fileoffset.empty())
		{
			std::string name{ etc::g_exception_types[ctx.m_code] ? etc::g_exception_types[ctx.m_code].what() : std::format("0x{:X}", ctx.m_code) };

			switch (ctx.m_code)
			{
				case CONTROL_C_EXIT:
				case EXCEPTION_BREAKPOINT:
				case EXCEPTION_SINGLE_STEP:
				{
					LOG(Exception, "The game has suffered a non-fatal exception, you may disregard this message ({} at {})", name, ctx.m_fileoffset);
					return EXCEPTION_CONTINUE_EXECUTION;
				} break;

				case EXCEPTION_GUARD_PAGE:
				case EXCEPTION_ACCESS_VIOLATION:
				{
					std::string type{ etc::get_exception_type(ctx.m_type) };
					LOG(Exception, "The game suffered an fatal exception, you may need to restart the game. ({} at {}, reason of {} was {}{})", name, ctx.m_fileoffset, name, type, ctx.m_type != 8 && name != "unknown" ? "" : std::format("0x{:X}", ctx.m_deathAddress));
				} break;

				default:
				{
					if (!handle_module_exception(ctx.m_code))
					{
						LOG(Exception, "The game suffered a exception of unknown severity, you may need to restart the game. ({} at {}, reason of exception is unknown)", name, ctx.m_fileoffset);
					}
				} break;
			}
		}

		//LOG(Exception, "Dumping registerss...");
		//ctx.print_registers();

		if (!attempt_stack_recovery(info))
		{
			return EXCEPTION_CONTINUE_SEARCH;
		}

		return EXCEPTION_CONTINUE_EXECUTION;
	}

	inline LONG WINAPI handle_exception(PEXCEPTION_POINTERS info)
	{
		return handle_exception_inner(info);
	}

	inline LONG WINAPI handle_unhandled_exception(PEXCEPTION_POINTERS info)
	{
		return handle_exception_inner(info);
	}

	inline void attach_handler()
	{
		g_handle = AddVectoredExceptionHandler(FALSE, handle_exception);

		g_filter_handle = SetUnhandledExceptionFilter(handle_unhandled_exception);
	}
	inline void detach_handler()
	{
		if (g_handle)
		{
			RemoveVectoredExceptionHandler(g_handle);
		}

		if (g_filter_handle)
		{
			SetUnhandledExceptionFilter(g_filter_handle);
		}
	}
}