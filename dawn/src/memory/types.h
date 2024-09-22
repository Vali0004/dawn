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
	using GetProcAddressT = fptr<FARPROC(HMODULE hModule, LPCSTR lpProcName)>;
	using CExtraContentManagerLoadContentT = fptr<void(CExtraContentManager* _This, bool executeChangeSet, bool executeEarlyStartup)>;
	using CExtraContentManagerEndEnumerateContentT = fptr<void(CExtraContentManager* _This, bool bWait, bool earlyStartup)>;
	using CExtraContentManagerAddContentFolderT = fptr<bool(CExtraContentManager* _This, const char* path)>;
	using CExtraContentManagerGetCRCT = fptr<u32(CExtraContentManager* _This, u32 initValue)>;
	using AESisTransformITKeyT = fptr<bool(u32 keyId)>;
	using AESTransformITDecryptT = fptr<void(u32 selector, void* data, u32 size)>;
	using AESDecryptT = fptr<void(u32 keyId, u32 selector, void* data, u32 size)>;
	using fiPackfileReInitT = fptr<bool(rage::fiPackfile* _This, const char* filename, bool readNameHeap, rage::fiPackEntry* headerData)>;
	using fwScriptGuidGetBaseFromGuidT = fptr<rage::fwExtensibleBase*(int guid)>;
	using rlSysBattlEyeInitT = fptr<bool()>;
	using CNetworkCheckNetworkAccessT = fptr<eNetworkAccessCode(eNetworkAccessArea nAccessArea, uint64_t* endPosixTime)>;
	using CNetworkBailT = fptr<bool(sBailParameters* bailParams, bool bSendScriptEvent)>;
}