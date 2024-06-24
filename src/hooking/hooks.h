#pragma once
#include "hooks/hooks_base.h"
#include "hooks/game_skeleton.h"
#include "hooks/rgsc.h"
#include "hooks/net_http_request.h"
#include "hooks/http_process_response.h"
#include "hooks/aes.h"

namespace cs::hooking
{
	inline etc::hook<pointers::types::sysEpicInitClassT>* g_sysEpicInitClass{};
	inline bool sysEpicInitClass(void* _This, const char* epicProductName, char* epicProductVersion, const char* epicProductId, const char* epicSandboxId, const char* epicClientId, const char* epicClientSecret, const char* epicDeploymentId)
	{
		//return false;
		return g_sysEpicInitClass->original()(_This, epicProductName, epicProductVersion, epicProductId, epicSandboxId, epicClientId, epicClientSecret, epicDeploymentId);
	}

	inline etc::hook<pointers::types::entitlementManagerInitT>* g_entitlementManagerInit{};
	inline void entitlementManagerInit(u32 v)
	{
		//return;
		return g_entitlementManagerInit->original()(v);
	}

	inline etc::hook<pointers::types::fiAssetManagerFullPathT>* g_fiAssetManagerFullPath{};
	inline void fiAssetManagerFullPath(void* _This, char* dest, int maxLen, const char* b, const char* ext, int pathIndex)
	{
		/*std::string base{b};
		if (!strcmp(ext, "fxc")) {
			LOG_TO_STREAM("Loading shader " << base);
		}*/
		//LOG(Info, "fiAssetManager::FullPath called. Loading {}", base);
		g_fiAssetManagerFullPath->original()(_This, dest, maxLen, b, ext, pathIndex);
		/*if (!strcmp(ext, "fxc")) {
			LOG_TO_STREAM("Full path: " << dest);
		}*/
	}

	inline etc::hook<pointers::types::grcEffectInitT>* g_grcEffectInit{};
	inline bool grcEffectInit(rage::grcEffect* _This, const char* path)
	{
		LOG_TO_STREAM("Effect doesn't already exist.");
		return g_grcEffectInit->original()(_This, path);
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

	inline bool g_early_hook{};
	inline void hook_rgsc()
	{
		//make_hook_time_critical("RgscRetailLog::RgscRetailMsg", RgscRetailLogRgscRetailMsg);
		make_hook_time_critical("Rgsc::Init", RgscInit);
		//make_hook_time_critical("rlHttpTask::BuildUrl", rlHttpTaskBuildUrl);
		//make_hook_time_critical("rlHttpTask::BuildUrlSc", rlHttpTaskBuildUrlSc);
		//make_hook_time_critical("CreateTicketTask::BuildUrlSc", CreateTicketTaskBuildUrlSc);
		////make_hook_time_critical("netHttpRequest::StartRequest", netHttpRequestStartRequest);
		////make_hook_time_critical("netHttpRequest::Update", netHttpRequestUpdate);
		////make_hook_time_critical("netHttpRequest::Begin", netHttpRequestBegin);
		//make_hook_time_critical("rlRosHttpTask::ProcessResponse", rlRosHttpTaskProcessResponse);
		//make_hook_time_critical("rlRosHttpTask::ProcessResponseSc", rlRosHttpTaskProcessResponseSc);
		//make_hook_time_critical("rlProfileStatsFlushTask::Configure", rlProfileStatsFlushTaskConfigure);
		////make_hook_time_critical("GameTransactionBaseHttpTask::ProcessResponse", GameTransactionBaseHttpTaskProcessResponse);
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
			g_early_hook = true;
			hook_rgsc();

			return result;
		}

		return g_GetProcAddress->original()(hModule, lpProcName);
	}

	inline etc::hook_vtbl* g_grcSwapChain{};
	inline HRESULT grcSwapChainPresent(rage::grcSwapChain* current, u32 syncInterval, u32 flags)
	{
		if (renderer::get())
		{
			renderer::get()->present();
		}

		return g_grcSwapChain->original<pointers::types::grcSwapChainPresentT>(8)(current, syncInterval, flags);
	}

	inline HRESULT grcSwapChainResizeBuffers(rage::grcSwapChain* current, u32 bufferCount, u32 width, u32 height, DXGI_FORMAT newFormat, u32 swapChainFlags)
	{
		HRESULT result{};
		ImGui_ImplDX11_InvalidateDeviceObjects();
		result = g_grcSwapChain->original<pointers::types::grcSwapChainResizeBuffersT>(13)(current, bufferCount, width, height, newFormat, swapChainFlags);
		if (SUCCEEDED(result))
		{
			ImGui_ImplDX11_CreateDeviceObjects();
		}

		return result;
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
			renderer::create();
		}

		pointers::late_scan();
		LOG_TO_STREAM("Finished scanning pointers.");

		make_hook_vtbl(grcSwapChain, *pointers::g_pSwapChain, 19,
		{
			LOG_TO_STREAM("Hooking SwapChain");
			_this->set_func(8, grcSwapChainPresent);
			_this->set_func(13, grcSwapChainResizeBuffers);
		});

		/*spawn_loading_screen_wait_thread(LOADINGSCREEN_CONTEXT_NONE, [] {
			std::this_thread::sleep_for(5s);
			make_hook_time_critical("CommandShouldWarnOfSimpleModCheck", CommandShouldWarnOfSimpleModCheck);

			make_hook("AES::TransformITDecrypt", AESTransformITDecrypt);
			make_hook("AES::Decrypt", AESDecrypt);
			etc::persist_mh::apply_queued();

			//make_hook_vtbl(gameSkeleton, pointers::g_gameSkeleton, 3,
			//{
			//	_this->set_func(1, gameSkeletonInit);
			//	_this->set_func(3, gameSkeletonUpdate);
			//});
		});*/
	}

	inline void create()
	{
		pointers::wait_for_rgsc();
		pointers::early_scan();

		make_hook_time_critical("sysEpic::InitClass", sysEpicInitClass);
		//make_hook_time_critical("CEntitlementManager::Init", entitlementManagerInit);
		//make_hook_time_critical("fiAssetManager::FullPath", fiAssetManagerFullPath);
		make_hook_time_critical("grcEffect::Init", grcEffectInit);

		pointers::remove_keyboard_hook();

		std::thread(&spawn_delayed_create).detach();
	}

	inline void remove()
	{
		if (renderer::get())
		{
			renderer::destroy();
		}

		return etc::remove_all();
	}
}