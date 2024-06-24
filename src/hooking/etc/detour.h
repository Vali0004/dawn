#pragma once
#include "pch/pch.h"
#include "minhook/MinHook.h"
#include "common/persistent.h"

namespace cs::hooking::etc::helpers {
	inline u8* find_jump(const void* code) {
		u8* byte = (u8*)code;
		while (*(u8*)byte != 0xE8 && *(u8*)byte != 0xE9) {
			byte++;
		}
		return byte;
	}

	inline volatile const void* follow_jump_near(const void* code) {
		const u8* jmp = find_jump(code);
		u32 relativeOffset = *(u32*)(jmp + 1);
		u64 baseAddress = (u64)(jmp + 5);
		const void* ret;
		ret = (void*)(baseAddress + relativeOffset);
		return ret;
	}
}
namespace cs::hooking::etc {
	class persist_mh : public persist_impl {
	public:
		persist_mh()
			: persist_impl(init, uninit)
		{}
		static void apply_queued() {
			MH_ApplyQueued();
		}
	private:
		static void init() {
			MH_Initialize();
		}
		static void uninit() {
			MH_Uninitialize();
		}
	} persist_mh{};
	class hook_vtbl;
	class hook_impl;
	inline std::map<std::string, hook_impl> g_hooks{};
	inline std::vector<hook_vtbl*> g_vtbl_hooks{};
	class hook_impl final {
	public:
		hook_impl(const std::string& name, void* target, void* function)
			: m_name(name), m_target(target), m_original(target), m_function(function) {}
		hook_impl()
		{}

		void correct_target() {
			volatile const void* func = helpers::follow_jump_near(m_target);
			m_target = reinterpret_cast<void*>((u64)func);
		}

		void* target() {
			return m_target;
		}

		void* fn() {
			return m_function;
		}

		void* original() {
			return m_original;
		}
		void** og() {
			return &m_original;
		}

		const std::string& name() {
			return m_name;
		}
	protected:
		std::string m_name{};
	private:
		void* m_target{};
		void* m_function{};
		void* m_original{};
	};

	class hook_vtbl {
	public:
		hook_vtbl(const std::string& id, void* vtable, u64 num_funcs)
			: m_vtable(reinterpret_cast<u64**>(vtable)),
			m_original(*m_vtable),
			m_num_funcs(num_funcs)
		{
			m_vtable_new = std::make_unique<u64[]>(m_num_funcs);
			std::copy_n(m_original, m_num_funcs, m_vtable_new.get());
			g_vtbl_hooks.push_back(this);
		}
		~hook_vtbl() {
			remove();
		}

		template <typename T>
		T original(u64 index) {
			return reinterpret_cast<T>(m_original[index]);
		}

		template <typename T>
		void set_func(u64 index, T function) {
			m_vtable_new[index] = (u64)function;
		}
		void restore_func(u64 index) {
			m_vtable_new[index] = m_original[index];
		}

		void create() {
			*m_vtable = m_vtable_new.get();
		}
		void remove() {
			*m_vtable = m_original;
		}

	private:
		u64 m_function;
		u64** m_vtable;
		std::unique_ptr<u64[]> m_vtable_new;
		u64* m_original;
		u64 m_num_funcs;
	};

	inline int handle_mh_status(MH_STATUS& status, hook_impl& hook) {
		switch (status) {
			case MH_ERROR_NOT_INITIALIZED: {
				MH_Initialize();
				return 1;
			} break;

			case MH_ERROR_UNSUPPORTED_FUNCTION:
			case MH_ERROR_NOT_EXECUTABLE: {
				hook.correct_target();
				status = MH_CreateHook(hook.target(), hook.fn(), hook.og());
				return handle_mh_status(status, hook);
			} break;

			case MH_ERROR_ALREADY_CREATED: {
				return -1;
			} break;

			case MH_ERROR_NOT_CREATED: {
				MH_CreateHook(hook.target(), hook.fn(), hook.og());
				return handle_mh_status(status, hook);
			} break;

			case MH_ERROR_ENABLED: {
				return -1;
			} break;

			default:
			case MH_OK: {
				return 0;
			} break;
		}
	}

	inline void create_impl(hook_impl& hook, bool queue) {
		g_hooks.insert({ hook.name(), hook });

		MH_STATUS status = MH_CreateHook(hook.target(), hook.fn(), hook.og());
		//handle_mh_status(status, hook);

		if (queue) {
			MH_QueueEnableHook(hook.target());
			return;
		}

		status = MH_EnableHook(hook.target());
		//handle_mh_status(status, hook);
	}

	inline void remove_impl(hook_impl& hook) {
		g_hooks.erase(hook.name());

		if (hook.target()) {
			MH_DisableHook(hook.target());
			MH_RemoveHook(hook.target());
		}
	}

	inline void remove_all() {
		if (!g_hooks.empty()) {
			MH_DisableHook(MH_ALL_HOOKS);
			MH_RemoveHook(MH_ALL_HOOKS);
			g_hooks.clear();
		}

		if (!g_hooks.empty()) {
			for (auto& h : g_vtbl_hooks) {
				h->remove();
			}
			g_vtbl_hooks.clear();
		}
	}

	template <typename T>
	class hook {
	public:
		hook(const std::string& id, T target, T function, bool queued = false)
			: m_hook(id, (void*)target, (void*)function), m_queued(queued)
		{
			create();
		}
		~hook() {
			remove();
			if (this) {
				delete reinterpret_cast<hook*>(this);
			}
		}

		T original() {
			return (T)m_hook.original();
		}

		hook_impl& get_impl() {
			return m_hook;
		}

		void create() {
			create_impl(m_hook, m_queued);
		}
		void remove() {
			remove_impl(m_hook);
		}
	private:
		bool m_queued{};
		hook_impl m_hook;
	};
}