#pragma once
#include "hooks_base.h"

namespace cs::hooking
{
	inline etc::hook<pointers::types::netHttpRequestStartRequestT>* g_netHttpRequestStartRequest{};
	inline bool netHttpRequestStartRequest(rage::netHttpRequest* _this, const char* uri)
	{ 
		if (uri)
		{
			LOG_TO_STREAM("netHttpRequest::StartRequest: " << uri);
		}

		return g_netHttpRequestStartRequest->original()(_this, uri);
	}

	inline etc::hook<pointers::types::netHttpRequestUpdateT>* g_netHttpRequestUpdate{};
	inline void netHttpRequestUpdate(rage::netHttpRequest* _this)
	{
		g_netHttpRequestUpdate->original()(_this);
		if (_this && rage::fiIsValidHandle(_this->m_ResponseHandle) && _this->m_ResponseDevice)
		{
			LOG_TO_STREAM("netHttpRequest::Update: " << _this->m_RawUri << "://" << _this->m_RawHost << "" << _this->m_RawPath);
		}
	}

	inline etc::hook<pointers::types::netHttpRequestBeginT>* g_netHttpRequestBegin{};
	inline bool netHttpRequestBegin(rage::netHttpRequest* _this, const rage::netHttpVerb verb, const char* uri, const rage::netSocketAddress* proxyAddr, const u32 timeoutSecs, const rage::fiDevice* responseDevice, const rage::fiHandle responseHandle, const char* contextStr, rage::netHttpFilter* filter, rage::netStatus* status)
	{
		bool result = g_netHttpRequestBegin->original()(_this, verb, uri, proxyAddr, timeoutSecs, responseDevice, responseHandle, contextStr, filter, status);
		if (_this && rage::fiIsValidHandle(_this->m_ResponseHandle) && _this->m_ResponseDevice)
		{
			LOG_TO_STREAM("netHttpRequest::Begin: " << _this->m_RawUri << "://" << _this->m_RawHost << _this->m_RawPath);
		}
		return result;
	}
}