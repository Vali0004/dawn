#pragma once
#include "hooks_base.h"

namespace dwn::hooking
{
	inline etc::hook<pointers::types::rlSysBattlEyeInitT>* g_rlSysBattlEyeInit{};
	inline bool rlSysBattlEyeInit()
	{
		return false;
		//return g_rlSysBattlEyeInit->original()();
	}

	inline etc::hook<pointers::types::CNetworkCheckNetworkAccessT>* g_CNetworkCheckNetworkAccess{};
	inline eNetworkAccessCode CNetworkCheckNetworkAccess(eNetworkAccessArea nAccessArea, uint64_t* endPosixTime)
	{
		return Access_Granted;
		//return g_CNetworkCheckNetworkAccess->original()(nAccessArea, endPosixTime);
	}

	inline etc::hook<pointers::types::CNetworkBailT>* g_CNetworkBail{};
	inline bool CNetworkBail(sBailParameters* bailParams, bool bSendScriptEvent)
	{
		return false;
		//return g_CNetworkBail->original()(bailParams, bSendScriptEvent);
	}
}