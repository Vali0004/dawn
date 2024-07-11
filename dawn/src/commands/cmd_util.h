#pragma once
#include "pch/framework.h"
#include "memory/pointers.h"
#include "util.h"

namespace rage
{
	inline void run_as_thread(u32 hash, std::function<void()> cb)
	{
		auto tls_ctx = rage::tlsContext::get();
		for (auto& thread : *dwn::pointers::g_smThreads)
		{
			if (!thread || !thread->m_Serialized.m_ThreadId || thread->m_ScriptNameHash != hash)
				continue;

			auto orignal_thread = tls_ctx->m_script_thread;

			tls_ctx->m_script_thread = thread;
			tls_ctx->m_is_script_thread_active = true;

			if (cb) cb();

			tls_ctx->m_script_thread = orignal_thread;
			tls_ctx->m_is_script_thread_active = (orignal_thread != nullptr);
		}
	}

	inline std::stack<std::pair<u32, std::function<void()>>> g_thread_callbacks{};
	inline void invoke_thread_callbacks(u32 thread_hash)
	{
		if (!g_thread_callbacks.empty())
		{
			auto& p{ g_thread_callbacks.top() };
			if (thread_hash == p.first)
			{
				run_as_thread(thread_hash, p.second);
				g_thread_callbacks.pop();
			}
		}
	}
	inline void run_as_thread_callback(u32 hash, std::function<void()> cb)
	{
		g_thread_callbacks.push(std::make_pair(hash, cb));
	}

	inline void run_as_thread_detached(u32 hash, std::function<void()> cb)
	{
		run_as_thread(hash, [cb] {
			dwn::util::spawn_detached_thread(cb);
		});
	}
}