#pragma once
#include "hooks_base.h"

namespace dwn::hooking
{
	inline etc::hook<pointers::types::RgscRetailLogRgscRetailMsgT>* g_RgscRetailLogRgscRetailMsg{};
	inline void RgscRetailLogRgscRetailMsg(bool utf8, int severity, const char* fmt, char* args)
	{
		char message[2048];
		vsnprintf_s(message, 2048, 1024, fmt, args);
		LOG_TO_STREAM(message);
		return g_RgscRetailLogRgscRetailMsg->original()(utf8, severity, fmt, args);
	}

	inline etc::hook<pointers::types::RgscInitT>* g_RgscInit{};
	inline rgsc::RGSC_HRESULT RgscInit(void* _this, rgsc::IConfiguration* config, rgsc::ITitleId* titleId, rgsc::RgscLanguage language, rgsc::IRgscDelegate* dlgt)
	{
		rgsc::RGSC_HRESULT result = g_RgscInit->original()(_this, config, titleId, language, dlgt);
		pointers::defeat_arx();
		renderer::create();
		return result;
	}
}