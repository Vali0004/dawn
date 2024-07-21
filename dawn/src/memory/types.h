#pragma once
#include "pch/pch.h"
#include "common/classes.h"

namespace dwn::pointers::types
{
	using CLoadingScreensSetLoadingContextT = fptr<void(LoadingScreenContext eContext)>;
	using CLoadingScreensSetMovieContextT = fptr<void(LoadingScreenMovieContext iMovieContext)>;
	using CLoadingScreensShutdownT = fptr<void(u32 shutdownMode)>;
	using scrThreadRunT = fptr<rage::scrThread::State(rage::scrValue* stack, rage::scrValue* globals, rage::scrProgram* pt, rage::scrThread::Serialized* ser)>;
	using CommandShouldWarnOfSimpleModCheckT = fptr<bool()>;
	using grcSwapChainPresentT = fptr<HRESULT(rage::grcSwapChain* current, u32 syncInterval, u32 flags)>;
	using grcSwapChainResizeBuffersT = fptr<HRESULT(rage::grcSwapChain* current, u32 bufferCount, u32 width, u32 height, DXGI_FORMAT newFormat, u32 swapChainFlags)>;
	using RgscRetailLogRgscRetailMsgT = fptr<void(bool utf8, int severity, const char* fmt, char* args)>;
	using RgscInitT = fptr<rgsc::RGSC_HRESULT(void* _this, rgsc::IConfiguration* config, rgsc::ITitleId* titleId, rgsc::RgscLanguage language, rgsc::IRgscDelegate* dlgt)>;
	using rlHttpTaskBuildUrlT = fptr<const char* (rage::rlHttpTask* _this, char* url, const u32 sizeofUrl)>;
	using rlHttpTaskBuildUrlScT = fptr<const char* (rage::rlHttpTask* _this, char* url, const u32 sizeofUrl)>;
	using CreateTicketTaskBuildUrlScT = fptr<const char* (rage::rlHttpTask* _this, char* url, const u32 sizeofUrl)>;
	using netHttpRequestStartRequestT = fptr<bool(rage::netHttpRequest* _this, const char* uri)>;
	using netHttpRequestUpdateT = fptr<void(rage::netHttpRequest* _this)>;
	using netHttpRequestBeginT = fptr<bool(rage::netHttpRequest* _this, const rage::netHttpVerb verb, const char* uri, const rage::netSocketAddress* proxyAddr, const u32 timeoutSecs, const rage::fiDevice* responseDevice, const rage::fiHandle responseHandle, const char* contextStr, rage::netHttpFilter* filter, rage::netStatus* status)>;
	using rlRosHttpTaskProcessResponseT = fptr<bool(rage::rlRosHttpTask* _this, const char* response, int* resultCode)>;
	using rlRosHttpTaskProcessResponseScT = fptr<bool(rage::rlRosHttpTask* _this, const char* response, int* resultCode)>;
	using rlProfileStatsFlushTaskConfigureT = fptr<bool(rage::rlProfileStatsFlushTask* task, rage::rlProfileStatsClient* ctx, rage::rlProfileStatsDirtyIterator* flushIt, rage::netStatus* status)>;
	using GameTransactionBaseHttpTaskProcessResponseT = fptr<bool(void* _This, const char* response)>;
	using GetProcAddressT = fptr<FARPROC(HMODULE hModule, LPCSTR lpProcName)>;
	using CExtraContentManagerLoadContentT = fptr<void(CExtraContentManager* _This, bool executeChangeSet, bool executeEarlyStartup)>;
	using CExtraContentManagerEndEnumerateContentT = fptr<void(CExtraContentManager* _This, bool bWait, bool earlyStartup)>;
	using CExtraContentManagerAddContentFolderT = fptr<bool(CExtraContentManager* _This, const char* path)>;
	using CExtraContentManagerGetCRCT = fptr<u32(CExtraContentManager* _This, u32 initValue)>;
}