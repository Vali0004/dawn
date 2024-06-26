#pragma once
#include "hooks/hooks_base.h"
#include "hooks/swapchain.h"
#include "hooks/game_skeleton.h"
#include "hooks/rgsc.h"
#include "hooks/net_http_request.h"
#include "hooks/http_process_response.h"
#include "hooks/build_url.h"
#include "hooks/aes.h"

namespace cs::hooking
{
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

	inline etc::hook<pointers::types::grcStateBlockFlushRasterizerStateT>* g_grcStateBlockFlushRasterizerState{};
	inline void grcStateBlockFlushRasterizerState(rage::grcStateBlock::grcRasterizerStateHandle newState)
	{
		rage::grcStateBlock::grcRasterizerStateHandle newStateBackup = newState;

		if (renderer::WireframeOverride)
		{
			LOG_TO_STREAM("Wireframe override, set newState to WireFrame");
			newState = renderer::RS_WireFrame;
		}

		g_grcStateBlockFlushRasterizerState->original()(newState);

		//if (renderer::WireframeOverride)
		//{
			//LOG_TO_STREAM("Wireframe override, set state");
			// Trickry to mimick what the Debug build does to achieve wireframe
			//pointers::g_grcStateBlockSetRasterizerState(newState);
			// We don't want it to actually set it, we just want RS_Active to be newState.
			//*pointers::g_StateblockDirty &= ~rage::grcStateBlock::RASTERIZER_STATE_DIRTY;
		//}
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
			hook_rgsc();
		}

		pointers::late_scan();
		LOG_TO_STREAM("Finished scanning pointers.");

		if (!g_early_hook)
		{
			renderer::create();
		}

		make_hook_vtbl(grcSwapChain, *pointers::g_pSwapChain, 19,
		{
			LOG_TO_STREAM("Hooking SwapChain");
			_this->set_func(8, grcSwapChainPresent);
			_this->set_func(13, grcSwapChainResizeBuffers);
		});

		//std::this_thread::sleep_for(5s);
		make_hook("CommandShouldWarnOfSimpleModCheck", CommandShouldWarnOfSimpleModCheck);

		//make_hook("AES::TransformITDecrypt", AESTransformITDecrypt);
		//make_hook("AES::Decrypt", AESDecrypt);

		make_hook("grcStateBlock::FlushRasterizerState", grcStateBlockFlushRasterizerState);
		etc::persist_mh::apply_queued();

		make_hook_vtbl(gameSkeleton, pointers::g_gameSkeleton, 3,
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