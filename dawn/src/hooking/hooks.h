#pragma once
#include "hooks/hooks_base.h"
#include "hooks/swapchain.h"
#include "hooks/game_skeleton.h"
#include "hooks/rgsc.h"
#include "hooks/net_http_request.h"
#include "hooks/http_process_response.h"
#include "hooks/build_url.h"
#include "hooks/aes.h"

namespace dwn::hooking
{
	inline etc::hook<pointers::types::scrThreadRunT>* g_scrThreadRun{};
	inline rage::scrThread::State scrThreadRun(rage::scrValue* stack, rage::scrValue* globals, rage::scrProgram* pt, rage::scrThread::Serialized* ser)
	{
		invoke_thread_callbacks(pt->HashCode);

		if (pt->HashCode == "freemode"_j || pt->HashCode == "main_persistent"_j)
		{
			renderer::menu::tick();
		}

		return g_scrThreadRun->original()(stack, globals, pt, ser);
	}

	inline etc::hook<pointers::types::CommandShouldWarnOfSimpleModCheckT>* g_CommandShouldWarnOfSimpleModCheck{};
	inline bool CommandShouldWarnOfSimpleModCheck()
	{
		return false;
	}

	inline etc::hook<pointers::types::rlProfileStatsFlushTaskConfigureT>* g_rlProfileStatsFlushTaskConfigure{};
	inline bool rlProfileStatsFlushTaskConfigure(rage::rlProfileStatsFlushTask* task, rage::rlProfileStatsClient* ctx, rage::rlProfileStatsDirtyIterator* flushIt, rage::netStatus* status)
	{
		ctx->m_SubmitCompressed = false;

		return g_rlProfileStatsFlushTaskConfigure->original()(task, ctx, flushIt, status);
	}

	inline etc::hook<pointers::types::fiPackfileReInitT>* g_fiPackfileReInit{};
	inline bool fiPackfileReInit(rage::fiPackfile* _This, const char* filename, bool readNameHeap, rage::fiPackEntry* headerData)
	{
		bool result = g_fiPackfileReInit->original()(_This, filename, readNameHeap, headerData);
		if (result)
		{
			for (int i{}; i != _This->m_EntryCount; ++i) 
			{
				rage::fiPackEntry& entry = _This->m_Entries[i];
				if (entry.IsFile() && entry.m_NameOffset > 0 && entry.u.file.m_Encrypted)
				{
					if (g_keyId == 0x4E45504F)
					{
						entry.u.file.m_Encrypted = 0xFEFFFFF;
					}
				}
			}
			LOG_TO_STREAM("KeyID: " << HEX(_This->m_KeyId));
			if (g_keyId == 0x4E45504F)
			{
				//_This->m_CachedDataSize = 0xFEFFFFF;
				//_This->m_KeyId = 0xFEFFFFF;
			}
		}
		return result;
	}

	inline bool g_early_hook{};
	inline void hook_rgsc()
	{
		//make_hook_time_critical("RgscRetailLog::RgscRetailMsg", RgscRetailLogRgscRetailMsg);
		make_hook_time_critical("Rgsc::Init", RgscInit);
		make_hook_time_critical("rlHttpTask::BuildUrl", rlHttpTaskBuildUrl);
		make_hook_time_critical("rlHttpTask::BuildUrlSc", rlHttpTaskBuildUrlSc);
		make_hook_time_critical("CreateTicketTask::BuildUrlSc", CreateTicketTaskBuildUrlSc);
		//make_hook_time_critical("netHttpRequest::StartRequest", netHttpRequestStartRequest);
		//make_hook_time_critical("netHttpRequest::Update", netHttpRequestUpdate);
		//make_hook_time_critical("netHttpRequest::Begin", netHttpRequestBegin);
		make_hook_time_critical("rlRosHttpTask::ProcessResponse", rlRosHttpTaskProcessResponse);
		make_hook_time_critical("rlRosHttpTask::ProcessResponseSc", rlRosHttpTaskProcessResponseSc);
		make_hook_time_critical("rlProfileStatsFlushTask::Configure", rlProfileStatsFlushTaskConfigure);
		//make_hook_time_critical("GameTransactionBaseHttpTask::ProcessResponse", GameTransactionBaseHttpTaskProcessResponse);
	}

	inline etc::hook<pointers::types::GetProcAddressT>* g_GetProcAddress{};
	inline FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
	{
		if (lpProcName == (LPCSTR)1)
		{
			LOG_TO_STREAM("Scanning pointers...");
			pointers::rgsc_scan();

			LOG_TO_STREAM("Game requested RGSC import, reusing...");
			FARPROC result = g_GetProcAddress->original()(hModule, lpProcName);
			//g_early_hook = true;
			//hook_rgsc();

			return result;
		}

		return g_GetProcAddress->original()(hModule, lpProcName);
	}

	inline void spawn_loading_screen_wait_thread(LoadingScreenContext ctx, fptr<void()> callback)
	{
		if (*pointers::g_LoadingScreenContext != ctx)
		{
			LOG_TO_STREAM("Loading screen state is not what we desire, spawning thread to wait for execution...");
			std::thread([ctx, callback] {
				while (*pointers::g_LoadingScreenContext != ctx)
				{
					std::this_thread::sleep_for(1ms);
				}

				LOG_TO_STREAM("Passed loading screen state, triggering callback...");
				if (callback)
				{
					callback();
				}
			}).detach();
		}
	}

	inline void spawn_delayed_create()
	{
		if (!g_early_hook)
		{
			std::this_thread::sleep_for(100ms);
			LOG_TO_STREAM("Scanning pointers...");
			pointers::rgsc_scan();
			//hook_rgsc();
		}

		pointers::late_scan();
		LOG_TO_STREAM("Finished scanning pointers.");

		if (!g_early_hook)
		{
			renderer::create();
		}

		make_hook_vtbl(grcSwapChain, *pointers::g_pSwapChain, 19,
		{
			_this->set_func(8, grcSwapChainPresent);
			_this->set_func(13, grcSwapChainResizeBuffers);
		});

		make_hook("scrThread::Run", scrThreadRun);

		make_hook("CommandShouldWarnOfSimpleModCheck", CommandShouldWarnOfSimpleModCheck);

		//make_hook("AES::isTransformITKey", AESisTransformITKey);
		//make_hook("AES::TransformITDecrypt", AESTransformITDecrypt);
		//make_hook("AES::Decrypt", AESDecrypt);
		//make_hook("fiPackfile::ReInit", fiPackfileReInit);

		etc::persist_mh::apply_queued();

		make_hook_vtbl(gameSkeleton, pointers::g_gameSkeleton, 4,
		{
			_this->set_func(1, gameSkeletonInit);
			_this->set_func(3, gameSkeletonUpdate);
		});
	}

	inline void create()
	{
		pointers::wait_for_rgsc();
		pointers::early_scan();

		pointers::remove_keyboard_hook();

		spawn_delayed_create();
	}

	inline void remove()
	{
		if (renderer::get())
		{
			renderer::destroy();
		}

		etc::remove_all();
		etc::remove_all_vmt();
	}
}