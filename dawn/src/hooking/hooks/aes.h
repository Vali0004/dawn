#pragma once
#include "hooks_base.h"

namespace dwn::hooking
{
	bool g_spoofValues{ true };
	inline etc::hook<pointers::types::CExtraContentManagerGetCRCT>* g_CExtraContentManagerGetCRC{};
	inline u32 CExtraContentManagerGetCRC(CExtraContentManager* _This, u32 initValue)
	{
		if (g_spoofValues)
		{
			return g_CExtraContentManagerGetCRC->original()(_This, 0);
		}
		return g_CExtraContentManagerGetCRC->original()(_This, initValue);
	}
}