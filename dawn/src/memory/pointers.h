#pragma once
#include "hooking/etc/detour.h"
#include "hooking/etc/vmt.h"
#include "memory/memory.h"
#include "memory/types.h"

namespace dwn::pointers
{
	inline types::CLoadingScreensSetLoadingContextT g_CLoadingScreensSetLoadingContext{};
	inline types::CLoadingScreensSetMovieContextT g_CLoadingScreensSetMovieContext{};
	inline types::CLoadingScreensShutdownT g_CLoadingScreensShutdown{};
	inline types::scrThreadRunT g_scrThreadRun{};
	inline types::CExtraContentManagerLoadContentT g_CExtraContentManagerLoadContent{};
	inline types::CExtraContentManagerEndEnumerateContentT g_CExtraContentManagerEndEnumerateContent{};
	inline types::CExtraContentManagerAddContentFolderT g_CExtraContentManagerAddContentFolder{};
	inline types::CExtraContentManagerGetCRCT g_CExtraContentManagerGetCRC{};
	inline types::fiPackfileReInitT g_fiPackfileReInit{};
	inline types::fwScriptGuidGetBaseFromGuidT g_fwScriptGuidGetBaseFromGuid{};
	inline types::AESisTransformITKeyT g_AESisTransformITKey{};
	inline types::AESTransformITDecryptT g_AESTransformITDecrypt{};
	inline types::AESDecryptT g_AESDecrypt{};
	inline types::rlSysBattlEyeInitT g_rlSysBattlEyeInit{};
	inline types::CNetworkCheckNetworkAccessT g_CNetworkCheckNetworkAccess{};
	inline types::CNetworkBailT g_CNetworkBail{};
	inline types::NetworkAssetEqualsT g_NetworkAssetEquals{};
	inline types::CommandShouldWarnOfSimpleModCheckT g_CommandShouldWarnOfSimpleModCheck{};
	inline types::GetProcAddressT g_GetProcAddress{};
	inline types::grcSwapChainPresentT g_grcSwapChainPresent{};
	inline types::grcSwapChainResizeBuffersT g_grcSwapChainResizeBuffers{};

	inline CStreamedScripts* g_StreamedScripts{};
	inline rage::fwTxdStore* g_TxdStore{};
	inline rage::grcTextureFactory** g_TextureFactory{};
	inline CExtraContentManager** g_CExtraContentManagersmInstance{};
	inline rage::grcSwapChain** g_pSwapChain{};
	inline u32* g_sysObfRandNext{};
	inline rage::gameSkeleton* g_gameSkeleton{};
	inline rage::scrCommandHash<rage::scrCmd>* g_sCommandHash{};
	inline rage::atArray<rage::scrThread*>* g_smThreads{};
	inline rage::atRangeArray<rage::scrValue*, scrProgram::MAX_GLOBAL_BLOCKS>* g_smGlobals{};
	inline const char** g_iVersionId{};
	inline CNetworkAssetVerifier** g_CNetworkAssetVerifiersmInstance{};
	inline rage::rlTitleId** g_rlTitleId{};
	inline LoadingScreenContext* g_LoadingScreenContext{};
	inline rage::rlPc* g_rlPc{};
	inline HWND g_hwnd{};

	inline void defeat_arx()
	{
		LOG_TO_STREAM("Defeating GuardIT algorithms...");
		s32 jumpception_count{},
			healers_count{};

		for (auto& h : memory::get_all_results("48 8D 45 ? 48 89 45 ? 48 8D 05 ? ? ? ? 48 89 45"))
		{
			++jumpception_count, ++healers_count;
			memset((void*)(h + 8), 0x90, 7);
		}

		if (!jumpception_count)
		{
			LOG_TO_STREAM("There are no GuardIT algorithms to defeat!");
		}
		else
		{
			LOG_TO_STREAM("Defeated all algorithms!");
			LOG_TO_STREAM("Number of times Jumpception was used: " << jumpception_count);
			LOG_TO_STREAM("Number of times Jumpception (code healers) was used: " << (healers_count / 2) << " (t2, group_count:2)");
		}
	}

	inline void trigger_scan(memory::pointer_manager& ptr_mgr)
	{
		ptr_mgr.run();
		while (!ptr_mgr.finished())
			;

		ptr_mgr.get_all();
		ptr_mgr.print();
	}

	inline void remove_keyboard_hook()
	{
		using removeKeybaordHookT = fptr<void()>;
		removeKeybaordHookT remove_fn{};
		memory::pointer_manager ptr_mgr{};
		ptr_mgr.add("RemoveKeyboardHook", remove_fn, "48 8B 0D ? ? ? ? 48 FF 25 ? ? ? ? C3");
		trigger_scan(ptr_mgr);

		if (remove_fn)
		{
			remove_fn();
			LOG_TO_STREAM("Removed keyboard hook. (Windows key lockout)");
		}
		else
		{
			LOG_TO_STREAM("There is no keyboard hook!");
		}
	}

	inline void wait_for_rgsc()
	{
		if (g_was_injected_early)
		{
			int elapsed_time{};
			while (!GetModuleHandleA("socialclub.dll"))
			{
				elapsed_time++;
				std::this_thread::sleep_for(duration<double, std::pico>(900));
			}
			LOG_TO_STREAM("RGSC took " << elapsed_time / 100 << "ms to load");
		}
	}

	inline bool g_game_speedup_scanned{};
	inline void game_speedup_scan(memory::pointer_manager& ptr_mgr, bool scan = true)
	{
		ptr_mgr.add("LoadingScreenContext", g_LoadingScreenContext, "8B 05 ? ? ? ? 33 ED 8B F2", "dword");
		ptr_mgr.add("CLoadingScreens::SetLoadingContext", g_CLoadingScreensSetLoadingContext, "48 83 EC ? 3B 0D ? ? ? ? 74");
		ptr_mgr.add("CLoadingScreens::SetMovieContext", g_CLoadingScreensSetMovieContext, "40 53 48 83 EC ? 8B D9 48 8D 0D ? ? ? ? E8 ? ? ? ? 84 C0 74 ? 8B 05");
		ptr_mgr.add("CLoadingScreens::Shutdown", g_CLoadingScreensShutdown, "48 89 5C 24 ? 56 48 83 EC ? 8B 05 ? ? ? ? 85 C0");
		if (!g_game_speedup_scanned && scan)
		{
			trigger_scan(ptr_mgr);
		}
		g_game_speedup_scanned = true;
	}

	inline void early_scan()
	{
		memory::pointer_manager ptr_mgr{};
		ptr_mgr.add("rage::rlSysBattlEye::Init", g_rlSysBattlEyeInit, "E9 ? ? ? ? CC 4A 68 E4 E8 A6 3F", "call");
		ptr_mgr.add("CNetwork::CheckNetworkAccess", g_CNetworkCheckNetworkAccess, "E8 ? ? ? ? 90 E9 ? ? ? ? ? ? 48 83 C4 ? 48 8D 64 ? ? 48 8B 5C ? ? E9 ? ? ? ? EB", "call");
		ptr_mgr.add("CNetwork::Bail", g_CNetworkBail, "E8 ? ? ? ? 32 C0 EB 11", "call");
		ptr_mgr.add("CNetwork::NetworkAssetVerifier", g_NetworkAssetEquals, "E8 ? ? ? ? 84 C0 74 B8 E8", "call");

		if (!g_game_speedup_scanned)
		{
			// Just append onto early_scan, it'll make it faster by a good chunk.
			game_speedup_scan(ptr_mgr, false);
		}
		trigger_scan(ptr_mgr);
	}

	inline void late_scan()
	{
		memory::pointer_manager ptr_mgr{};
		ptr_mgr.add("rage::scrThread::Run", g_scrThreadRun, "48 8B C4 4C 89 40 ? 48 89 50 ? 48 89 48 ? 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 4D 8B F9");
		ptr_mgr.add("CExtraContentManager::LoadContent", g_CExtraContentManagerLoadContent, "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC ? 48 8B F9 48 8D 4D");
		ptr_mgr.add("CExtraContentManager::EndEnumerateContent", g_CExtraContentManagerEndEnumerateContent, "48 89 5C 24 ? 57 48 83 EC ? 80 A1 ? ? ? ? ? 41 8A D8");
		ptr_mgr.add("CExtraContentManager::AddContentFolder", g_CExtraContentManagerAddContentFolder, "48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 55 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 8B F9 48 8B DA");
		ptr_mgr.add("CExtraContentManager::GetCRC", g_CExtraContentManagerGetCRC, "48 89 5C 24 ? 55 56 57 48 83 EC ? 33 ED 8B DA");
		ptr_mgr.add("rage::fiPackfile::ReInit", g_fiPackfileReInit, "48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 4C 89 60 ? 55 41 56 41 57 48 8B EC 48 83 EC ? 48 8B D9");
		ptr_mgr.add("rage::AES::isTransformITKey", g_AESisTransformITKey, "E8 ? ? ? ? 48 8B 53 20 44 8B C7 41 8B CE E8", "call");
		ptr_mgr.add("rage::AES::TransformITDecrypt", g_AESTransformITDecrypt, "E8 ? ? ? ? 41 8B D4 44 39 63 28 76 3F 41 B9", "call");
		ptr_mgr.add("rage::AES::Decrypt", g_AESDecrypt, "E8 ? ? ? ? 8B 55 F8 48 8B 43 10 48 03 D0 48 8B CB 48 89 53 18 66 44 89 22 33 D2 E8", "call");
		ptr_mgr.add("CommandShouldWarnOfSimpleModCheck", g_CommandShouldWarnOfSimpleModCheck, "40 53 48 83 EC 30 48 8B 1D ? ? ? ? BA");
		ptr_mgr.add("rage::fwScriptGuid::GetBaseFromGuid", g_fwScriptGuidGetBaseFromGuid, "83 F9 ? 74 ? 4C 8B 0D");

		ptr_mgr.add("rage::gameSkeleton::m_GameSkeleton", g_gameSkeleton, "48 8D 0D ? ? ? ? BA ? ? ? ? 74", "qword");
		ptr_mgr.add("rage::s_CommandHash", g_sCommandHash, "48 8D 0D ? ? ? ? 48 8B 14 FA", "qword");
		ptr_mgr.add("rage::scrThread::sm_Threads", g_smThreads, "48 8B 05 ? ? ? ? 8B CF 48 8B 1C C8 39 6B", "qword");
		ptr_mgr.add("rage::scrProgram::sm_Globals", g_smGlobals, "48 8D 15 ? ? ? ? 4C 8B C0 E8 ? ? ? ? 48 85 FF", "qword");
		ptr_mgr.add("CDebug::m_iVersionId", g_iVersionId, "48 8D 2D ? ? ? ? 48 85 C0 0F 84", "qword");
		ptr_mgr.add("CNetworkAssetVerifier::sm_Instance", g_CNetworkAssetVerifiersmInstance, "48 8B 0D ? ? ? ? E8 ? ? ? ? B1", "qword");
		ptr_mgr.add("rage::grcDevice::sm_pSwapChain", g_pSwapChain, "48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8", "qword");
		ptr_mgr.add("rage::sysObfuscatedTypes::obfRand::next", g_sysObfRandNext, "8B 0D ? ? ? ? BA ? ? ? ? 69 C9", "dword");
		ptr_mgr.add("rage::fwTxdStore::sm_Instance", g_TxdStore, "48 8D 0D ? ? ? ? E8 ? ? ? ? EB ? 48 8B 53", "qword");
		ptr_mgr.add("g_StreamedScripts", g_StreamedScripts, "48 8D 0D ? ? ? ? 4C 0F 45 C0 E8", "qword");
		ptr_mgr.add("rage::grcTextureFactory::sm_Instance", g_TextureFactory, "48 8B 0D ? ? ? ? 45 33 C0 48 8B 01 33 D2 FF 90 ? ? ? ? 48 8B 0D ? ? ? ? 83 64 24", "qword");
		ptr_mgr.add("CExtraContentManager::sm_Instance", g_CExtraContentManagersmInstance, "48 8B 1D ? ? ? ? 8B F9 83 F9", "qword");
		ptr_mgr.add("rage::rlTitleId::g_rlTitleId", g_rlTitleId, "48 8B 0D ? ? ? ? 48 8B D8 E8 ? ? ? ? 41", "qword");
		ptr_mgr.add("rage::rlPc::g_rlPc", g_rlPc, "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 85 C0 74 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B C8 48 8B 10 FF 92", "qword");
		trigger_scan(ptr_mgr);

		g_hwnd = FindWindowA("grcWindow", NULL);
	}

	inline void scan_all()
	{
		early_scan();
		late_scan();
	}
}

namespace rage
{
	namespace sysObfuscatedTypes
	{
		inline u32 obfRand()
		{
			u32& next = *dwn::pointers::g_sysObfRandNext;
			next = next * 214013 + 2531011;
			return next;
		}
	}
}