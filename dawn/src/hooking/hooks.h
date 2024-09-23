#pragma once
#include "hooks/hooks_base.h"
#include "hooks/swapchain.h"
#include "hooks/game_skeleton.h"
#include "hooks/battleye.h"
#include "hooks/aes.h"
#include "scripthookv/scripthookv.h"

namespace dwn::hooking
{
	inline etc::hook<pointers::types::scrThreadRunT>* g_scrThreadRun{};
	inline rage::scrThread::State scrThreadRun(rage::scrValue* stack, rage::scrValue* globals, rage::scrProgram* pt, rage::scrThread::Serialized* ser)
	{
		if (!g_scrThreadRun)
		{
			return rage::scrThread::State();
		}

		invoke_thread_callbacks(pt->HashCode);

		if (pt->HashCode == "freemode"_j || pt->HashCode == "main_persistent"_j)
		{
			if (DLC::IS_DLC_PRESENT("DEBUG_DETECT"_j))
			{
				LOG_TO_STREAM("Fuck, we tripped their AntiTamper debugger BS.");
			}
			renderer::menu::tick();
		}

		return g_scrThreadRun->original()(stack, globals, pt, ser);
	}

	inline etc::hook<pointers::types::CommandShouldWarnOfSimpleModCheckT>* g_CommandShouldWarnOfSimpleModCheck{};
	inline bool CommandShouldWarnOfSimpleModCheck()
	{
		return false;
	}

	inline bool g_early_hook{};
	inline void hook_ac()
	{
		make_hook_time_critical("rlSysBattlEye::Init", rlSysBattlEyeInit);
		make_hook_time_critical("CNetwork::CheckNetworkAccess", CNetworkCheckNetworkAccess);
		make_hook_time_critical("CNetwork::Bail", CNetworkBail);
	}

	inline etc::hook<pointers::types::GetProcAddressT>* g_GetProcAddress{};
	inline FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
	{
		if (lpProcName == (LPCSTR)1)
		{
			LOG_TO_STREAM("Game requested RGSC import, reusing...");

			FARPROC result = g_GetProcAddress->original()(hModule, lpProcName);

			g_early_hook = true;
			LOG_TO_STREAM("Scanning pointers...");
			hook_ac();

			return result;
		}

		if (reinterpret_cast<u64>(hModule) == "dwn"_j)
		{
			switch (atStringHash(lpProcName))
			{
				SHV_FN_IMPL(scriptRegister);
				SHV_FN_IMPL(scriptRegisterAdditionalThread);
				SHV_FN_IMPL(scriptUnregister);
				SHV_FN_IMPL(scriptUnregisterDepricated);
				SHV_FN_IMPL(nativeInit);
				SHV_FN_IMPL(nativePush64);
				SHV_FN_IMPL(nativeCall);
				SHV_FN_IMPL(getGlobalPtr);
				SHV_FN_IMPL(createTexture);
				SHV_FN_IMPL(drawTexture);
				SHV_FN_IMPL(scriptWait);
				SHV_FN_IMPL(keyboardHandlerRegister);
				SHV_FN_IMPL(keyboardHandlerUnregister);
				SHV_FN_IMPL(getGameVersion);
				SHV_FN_IMPL(getScriptHandleBaseAddress);
				SHV_FN_IMPL(presentCallbackRegister);
				SHV_FN_IMPL(presentCallbackUnregister);
				SHV_FN_IMPL(worldGetAllVehicles);
				SHV_FN_IMPL(worldGetAllPeds);
				SHV_FN_IMPL(worldGetAllObjects);
				SHV_FN_IMPL(worldGetAllPickups);
			}
		}

		return g_GetProcAddress->original()(hModule, lpProcName);
	}

	inline void create()
	{
		pointers::early_scan();
		pointers::wait_for_rgsc();

		//pointers::remove_keyboard_hook();

		if (!g_early_hook)
		{
			//std::this_thread::sleep_for(100ms);
			LOG_TO_STREAM("Scanning pointers...");
			hook_ac();
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

		//make_hook("CExtraContentManager::GetCRC", CExtraContentManagerGetCRC);
		
		//Can this be removed as this isn't really working ? 
		//make_hook("AES::isTransformITKey", AESisTransformITKey);
		//make_hook("AES::TransformITDecrypt", AESTransformITDecrypt);
		//make_hook("AES::Decrypt", AESDecrypt);
		//make_hook("fiPackfile::ReInit", fiPackfileReInit);

		etc::persist_mh::apply_queued();

		//Is This Still needed ? 
		make_hook_vtbl(gameSkeleton, pointers::g_gameSkeleton, 4,
		{
			_this->set_func(1, gameSkeletonInit);
			_this->set_func(3, gameSkeletonUpdate);
		});
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