#pragma once
#include "minhook/MinHook.h"
#include "common/persistent.h"
#include "base.h"

namespace dwn::hooking::etc
{
	class persist_mh : public persist_impl
	{
	public:
		persist_mh()
			: persist_impl(init, uninit)
		{}
		static void apply_queued()
		{
			MH_ApplyQueued();
		}
	private:
		static void init()
		{
			MH_Initialize();
		}
		static void uninit()
		{
			MH_Uninitialize();
		}
	} persist_mh{};

	inline int handle_mh_status(MH_STATUS& status, hook_impl& hook)
	{
		switch (status)
		{
			case MH_ERROR_NOT_INITIALIZED:
			{
				MH_Initialize();
				return 1;
			} break;

			case MH_ERROR_UNSUPPORTED_FUNCTION:
			case MH_ERROR_NOT_EXECUTABLE:
			{
				hook.correct_target();
				status = MH_CreateHook(hook.target(), hook.fn(), hook.og());
				return handle_mh_status(status, hook);
			} break;

			case MH_ERROR_ALREADY_CREATED:
			{
				return -1;
			} break;

			case MH_ERROR_NOT_CREATED:
			{
				MH_CreateHook(hook.target(), hook.fn(), hook.og());
				return handle_mh_status(status, hook);
			} break;

			case MH_ERROR_ENABLED:
			{
				return -1;
			} break;

			default:
			case MH_OK:
			{
				return 0;
			} break;
		}
	}

	inline void create_impl(hook_impl& hook, bool queue)
	{
		g_hooks.insert({ hook.name(), hook });

		MH_STATUS status = MH_CreateHook(hook.target(), hook.fn(), hook.og());
		//handle_mh_status(status, hook);

		if (queue)
		{
			MH_QueueEnableHook(hook.target());
			return;
		}

		status = MH_EnableHook(hook.target());
		//handle_mh_status(status, hook);
	}

	inline void remove_impl(hook_impl& hook)
	{
		g_hooks.erase(hook.name());

		if (hook.target())
		{
			MH_DisableHook(hook.target());
			MH_RemoveHook(hook.target());
		}
	}

	template <typename T>
	class hook
	{
	public:
		hook(const std::string& id, T target, T function, bool queued = false)
			: m_hook(id, (void*)target, (void*)function), m_queued(queued)
		{
			create();
		}
		~hook()
		{
			remove();
			if (this)
			{
				delete reinterpret_cast<hook*>(this);
			}
		}

		T original()
		{
			return (T)m_hook.original();
		}

		hook_impl& get_impl()
		{
			return m_hook;
		}

		void create()
		{
			create_impl(m_hook, m_queued);
		}
		void remove()
		{
			remove_impl(m_hook);
		}
	private:
		bool m_queued{};
		hook_impl m_hook;
	};
}