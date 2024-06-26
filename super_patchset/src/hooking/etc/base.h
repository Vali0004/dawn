#pragma once
#include "pch/pch.h"

namespace cs::hooking::etc::helpers
{
	inline u8* find_jump(const void* code)
	{
		u8* byte = (u8*)code;
		while (*(u8*)byte != 0xE8 && *(u8*)byte != 0xE9)
		{
			byte++;
		}
		return byte;
	}

	inline volatile const void* follow_jump_near(const void* code)
	{
		const u8* jmp = find_jump(code);
		u32 relativeOffset = *(u32*)(jmp + 1);
		u64 baseAddress = (u64)(jmp + 5);
		const void* ret;
		ret = (void*)(baseAddress + relativeOffset);
		return ret;
	}
}

namespace cs::hooking::etc
{
	class hook_vtbl;

	class hook_impl;
	inline std::map<std::string, hook_impl> g_hooks{};
	inline std::vector<hook_vtbl*> g_vtbl_hooks{};

	class hook_impl final
	{
	public:
		hook_impl(const std::string& name, void* target, void* function)
			: m_name(name), m_target(target), m_original(target), m_function(function) {}
		hook_impl()
		{}

		void correct_target()
		{
			volatile const void* func = helpers::follow_jump_near(m_target);
			m_target = reinterpret_cast<void*>((u64)func);
		}

		void* target()
		{
			return m_target;
		}

		void* fn()
		{
			return m_function;
		}

		void* original()
		{
			return m_original;
		}
		void** og()
		{
			return &m_original;
		}

		const std::string& name()
		{
			return m_name;
		}
	protected:
		std::string m_name{};
	private:
		void* m_target{};
		void* m_function{};
		void* m_original{};
	};

	inline void remove_all()
	{
		if (!g_hooks.empty())
		{
			MH_DisableHook(MH_ALL_HOOKS);
			MH_RemoveHook(MH_ALL_HOOKS);
			g_hooks.clear();
		}
	}
}