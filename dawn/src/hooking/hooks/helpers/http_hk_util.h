#pragma once
#include "pch/framework.h"
#include "common/classes.h"

namespace dwn::hooking::helpers
{
	template <typename T>
	inline const char* build_url(T& hook, rage::rlHttpTask* _this, char* url, const u32 sizeofUrl)
	{
		const char* result = hook->original()(_this, url, sizeofUrl);
		if (result)
		{
			if (std::string(result).find("Telemetry.asmx/") != -1)
			{
				strcpy(const_cast<char*>(result), "");
			}
			//LOG_TO_STREAM("rlHttpTaskBuildUrl: " << result);
		}
		return result;
	}
}