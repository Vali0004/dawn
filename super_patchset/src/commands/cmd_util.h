#pragma once
#include "pch/framework.h"
#include "memory/pointers.h"
#include "util.h"

namespace rage
{
	inline void run_as_thread(u32 hash, std::function<void()> cb)
	{
		auto tls_ctx = rage::tlsContext::get();
		for (auto& thread : *cs::pointers::g_smThreads)
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

	inline void run_as_thread_detached(u32 hash, std::function<void()> cb)
	{
		run_as_thread(hash, [cb] {
			cs::util::spawn_detached_thread(cb);
		});
	}
}