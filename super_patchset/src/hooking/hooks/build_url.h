#pragma once
#include "hooks_base.h"
#include "helpers/http_hk_util.h"

namespace cs::hooking
{
	inline etc::hook<pointers::types::rlHttpTaskBuildUrlT>* g_rlHttpTaskBuildUrl{};
	inline const char* rlHttpTaskBuildUrl(rage::rlHttpTask* _this, char* url, const u32 sizeofUrl)
	{
		return helpers::build_url(g_rlHttpTaskBuildUrl, _this, url, sizeofUrl);
	}

	inline etc::hook<pointers::types::rlHttpTaskBuildUrlScT>* g_rlHttpTaskBuildUrlSc{};
	inline const char* rlHttpTaskBuildUrlSc(rage::rlHttpTask* _this, char* url, const u32 sizeofUrl)
	{
		return helpers::build_url(g_rlHttpTaskBuildUrlSc, _this, url, sizeofUrl);
	}

	inline etc::hook<pointers::types::CreateTicketTaskBuildUrlScT>* g_CreateTicketTaskBuildUrlSc{};
	inline const char* CreateTicketTaskBuildUrlSc(rage::rlHttpTask* _this, char* url, const u32 sizeofUrl)
	{
		return helpers::build_url(g_CreateTicketTaskBuildUrlSc, _this, url, sizeofUrl);
	}
}