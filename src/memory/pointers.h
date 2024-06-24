#pragma once
#include "hooking/etc/detour.h"
#include "hooking/etc/vmt.h"
#include "memory/memory.h"
#include "memory/types.h"

namespace cs::pointers
{
	inline types::CLoadingScreensSetLoadingContextT g_CLoadingScreensSetLoadingContext{};
	inline types::CLoadingScreensSetMovieContextT g_CLoadingScreensSetMovieContext{};
	inline types::CLoadingScreensShutdownT g_CLoadingScreensShutdown{};
	inline types::sysEpicInitClassT g_sysEpicInitClass{};
	inline types::entitlementManagerInitT g_entitlementManagerInit{};
	inline types::fiAssetManagerFullPathT g_fiAssetManagerFullPath{};
	inline types::grcEffectInitT g_grcEffectInit{};
	inline types::grcEffectCreateT g_grcEffectCreate{};
	inline types::grcBeginT g_grcBegin{};
	inline types::grcVertexT g_grcVertex{};
	inline types::grcEndT g_grcEnd{};
	inline types::grcStateBlockCreateRasterizerStateT g_grcStateBlockCreateRasterizerState{};
	inline types::grcStateBlockDestroyRasterizerStateT g_grcStateBlockDestroyRasterizerState{};
	inline types::RgscRetailLogRgscRetailMsgT g_RgscRetailLogRgscRetailMsg{};
	inline types::RgscInitT g_RgscInit{};
	inline types::rlHttpTaskBuildUrlT g_rlHttpTaskBuildUrl{};
	inline types::rlHttpTaskBuildUrlScT g_rlHttpTaskBuildUrlSc{};
	inline types::CreateTicketTaskBuildUrlScT g_CreateTicketTaskBuildUrlSc{};
	inline types::netHttpRequestStartRequestT g_netHttpRequestStartRequest{};
	inline types::netHttpRequestUpdateT g_netHttpRequestUpdate{};
	inline types::netHttpRequestBeginT g_netHttpRequestBegin{};
	inline types::rlRosHttpTaskProcessResponseT g_rlRosHttpTaskProcessResponse{};
	inline types::rlRosHttpTaskProcessResponseScT g_rlRosHttpTaskProcessResponseSc{};
	inline types::rlProfileStatsFlushTaskConfigureT g_rlProfileStatsFlushTaskConfigure{};
	inline types::GameTransactionBaseHttpTaskProcessResponseT g_GameTransactionBaseHttpTaskProcessResponse{};
	inline types::CExtraContentManagerLoadContentT g_CExtraContentManagerLoadContent{};
	inline types::CExtraContentManagerEndEnumerateContentT g_CExtraContentManagerEndEnumerateContent{};
	inline types::CExtraContentManagerAddContentFolderT g_CExtraContentManagerAddContentFolder{};
	inline types::AESTransformITDecryptT g_AESTransformITDecrypt{};
	inline types::AESDecryptT g_AESDecrypt{};
	inline types::AESisTransformITKeyT g_AESisTransformITKey{};
	inline types::CommandShouldWarnOfSimpleModCheckT g_CommandShouldWarnOfSimpleModCheck{};
	inline types::GetProcAddressT g_GetProcAddress{};
	inline types::grcSwapChainPresentT g_grcSwapChainPresent{};
	inline types::grcSwapChainResizeBuffersT g_grcSwapChainResizeBuffers{};

	inline CExtraContentManager** g_CExtraContentManagersmInstance{};
	inline rage::grcSwapChain** g_pSwapChain{};
	inline u32* g_sysObfRandNext{};
	inline rage::gameSkeleton* g_gameSkeleton{};
	inline rage::scrCommandHash<rage::scrCmd>* g_sCommandHash{};
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
			std::this_thread::sleep_for(1ns);

		ptr_mgr.get_all();
		ptr_mgr.print();
	}

	using removeKeybaordHookT = fptr<void()>;
	inline void remove_keyboard_hook()
	{
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

			LOG(Info, "RGSC took {}ns to load", elapsed_time);
		}
	}

	inline void rgsc_scan()
	{
		memory::pointer_manager ptr_mgr{};
		ptr_mgr.add("rgsc::RgscRetailLog::RgscRetailMsg", g_RgscRetailLogRgscRetailMsg, "48 8B C4 48 89 58 ? 55 56 41 56", {}, {"socialclub.dll"});
		ptr_mgr.add("rgsc::Rgsc::Init", g_RgscInit, "48 89 5C 24 ? 55 57 41 54 41 56", {}, { "socialclub.dll" });
		ptr_mgr.add("rage::rlHttpTask::BuildUrl", g_rlHttpTaskBuildUrl, "4C 8B DC 49 89 5B ? 49 89 6B ? 49 89 73 ? 57 48 81 EC ? ? ? ? 48 8B 01 48 8B FA");
		ptr_mgr.add("rage::rlHttpTask::BuildUrlSc", g_rlHttpTaskBuildUrlSc, "4C 8B DC 49 89 5B ? 49 89 6B ? 49 89 73 ? 57 48 81 EC", {}, { "socialclub.dll" });
		ptr_mgr.add("rage::CreateTicketTask::BuildUrlSc", g_CreateTicketTaskBuildUrlSc, "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B 01", {}, { "socialclub.dll" });
		ptr_mgr.add("rage::netHttpRequest::StartRequest", g_netHttpRequestStartRequest, "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B D9 48 81 C1 ? ? ? ? 48 8B F2 33 FF");
		ptr_mgr.add("rage::netHttpRequest::Update", g_netHttpRequestUpdate, "40 55 57 41 54 41 55");
		ptr_mgr.add("rage::netHttpRequest::Begin", g_netHttpRequestBegin, "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC ? 4D 8B F1 49 8B E8");
		ptr_mgr.add("rage::rlRosHttpTask::ProcessResponse", g_rlRosHttpTaskProcessResponse, "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 33 FF 4D 8B E0");	
		ptr_mgr.add("rage::rlRosHttpTask::ProcessResponseSc", g_rlRosHttpTaskProcessResponseSc, "48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 55 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 48 83 64 24", {}, { "socialclub.dll" });
		ptr_mgr.add("rage::rlProfileStatsFlushTask::Configure", g_rlProfileStatsFlushTaskConfigure, "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 33 DB 49 8B F9 48 8B F1 4D 85 C9 74 ? 41 83 39 ? 74 ? 48 39 99");
		// This sig needs to be updated every time the game does, it's ARX protected.
		// Grab it from RsonReader::Init, func+random_loc "45 8B C8 45 33 C0 E9"
		ptr_mgr.add("GameTransactionBaseHttpTask::ProcessResponse", g_GameTransactionBaseHttpTaskProcessResponse, "48 8D 64 24 ? 44 8B 04 24 48 8D 64 24 ? 48 8B 44 24");
		ptr_mgr.add("rage::rlPc::g_rlPc", g_rlPc, "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 85 C0 74 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B C8 48 8B 10 FF 92", "qword");
		trigger_scan(ptr_mgr);
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
		ptr_mgr.add("rage::sysEpic::InitClass", g_sysEpicInitClass, "48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 55 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 33 DB 4D 8B F1");
		ptr_mgr.add("rage::fiAssetManager::FullPath", g_fiAssetManagerFullPath, "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 48 89 78 ? 41 56 48 83 EC ? 48 8B FA 49 8B D1");
		// This sig needs to be updated every time the game does, it's ARX protected.
		// Grab it from CGame::PreLoadingScreenInit, bottom func "48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 4C 89 70 ? 55 48 8D 68 ? 48 81 EC ? ? ? ? 41 BE"
		ptr_mgr.add("CEntitlementManager::Init", g_entitlementManagerInit, "E9 ? ? ? ? E9 ? ? ? ? 90", "call");

		ptr_mgr.add("rage::grcEffect::Init", g_grcEffectInit, "48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 55 48 8D 68 ? 48 81 EC ? ? ? ? 48 8B F2 48 8B F9 E8");
		ptr_mgr.add("rage::grcEffect::Create", g_grcEffectCreate, "48 89 5C 24 ? 57 48 83 EC ? 33 D2 48 8B F9 E8 ? ? ? ? 8B C8");
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
		ptr_mgr.add("CExtraContentManager::LoadContent", g_CExtraContentManagerLoadContent, "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC ? 48 8B F9 48 8D 4D");
		ptr_mgr.add("CExtraContentManager::EndEnumerateContent", g_CExtraContentManagerEndEnumerateContent, "48 89 5C 24 ? 57 48 83 EC ? 80 A1 ? ? ? ? ? 41 8A D8");
		ptr_mgr.add("CExtraContentManager::AddContentFolder", g_CExtraContentManagerAddContentFolder, "48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 55 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 8B F9 48 8B DA");
		ptr_mgr.add("rage::AES::TransformITDecrypt", g_AESTransformITDecrypt, "E8 ? ? ? ? 41 8B D4 44 39 63 28 76 3F 41 B9", "call");
		ptr_mgr.add("rage::AES::Decrypt", g_AESDecrypt, "E8 ? ? ? ? 8B 55 F8 48 8B 43 10 48 03 D0 48 8B CB 48 89 53 18 66 44 89 22 33 D2 E8", "call");
		ptr_mgr.add("rage::AES::isTransformITKey", g_AESisTransformITKey, "E8 ? ? ? ? 48 8B 53 20 44 8B C7 41 8B CE E8", "call");
		ptr_mgr.add("CommandShouldWarnOfSimpleModCheck", g_CommandShouldWarnOfSimpleModCheck, "40 53 48 83 EC 30 48 8B 1D ? ? ? ? BA");
		ptr_mgr.add("rage::grcBegin", g_grcBegin, "48 89 5C 24 ? 57 48 83 EC ? 48 83 3D ? ? ? ? ? 8B DA");
		ptr_mgr.add("rage::grcVertex", g_grcVertex, "83 3D ? ? ? ? ? 48 8B 05 ? ? ? ? 74");
		ptr_mgr.add("rage::grcEnd", g_grcEnd, "40 53 48 83 EC ? 80 3D ? ? ? ? ? 74 ? 48 83 C4");
		ptr_mgr.add("rage::grcStateBlock::CreateRasterizerState", g_grcStateBlockCreateRasterizerState, "48 8B C4 55 48 8D 68 ? 48 81 EC ? ? ? ? F3 0F 10 01");
		ptr_mgr.add("rage::grcStateBlock::DestroyRasterizerState", g_grcStateBlockDestroyRasterizerState, "E8 ? ? ? ? F3 0F 10 4B ? F3 0F 10 03", "call");
		// This might come in handy later.
		//ptr_mgr.add("rage::grcStateBlock::SetRasterizerState", g_grcStateBlockSetRasterizerState, "48 8B C4 48 89 58 ? 48 89 78 ? 4C 89 70 ? 4C 89 78 ? 55 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 41 BE");
		
		ptr_mgr.add("rage::gameSkeleton::m_GameSkeleton", g_gameSkeleton, "48 8D 0D ? ? ? ? BA ? ? ? ? 74", "qword");
		ptr_mgr.add("rage::s_CommandHash", g_sCommandHash, "48 8D 0D ? ? ? ? 48 8B 14 FA", "qword");
		ptr_mgr.add("CNetworkAssetVerifier::sm_Instance", g_CNetworkAssetVerifiersmInstance, "48 8B 0D ? ? ? ? E8 ? ? ? ? B1", "qword");
		ptr_mgr.add("rage::grcDevice::sm_pSwapChain", g_pSwapChain, "48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8", "qword");
		ptr_mgr.add("rage::sysObfuscatedTypes::obfRand::next", g_sysObfRandNext, "8B 0D ? ? ? ? BA ? ? ? ? 69 C9", "dword");
		ptr_mgr.add("CExtraContentManager::sm_Instance", g_CExtraContentManagersmInstance, "48 8B 1D ? ? ? ? 8B F9 83 F9", "qword");
		ptr_mgr.add("rage::rlTitleId::g_rlTitleId", g_rlTitleId, "48 8B 0D ? ? ? ? 48 8B D8 E8 ? ? ? ? 41", "qword");
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
			u32& next = *cs::pointers::g_sysObfRandNext;
			next = next * 214013 + 2531011;
			return next;
		}
	}
}