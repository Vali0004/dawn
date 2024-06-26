#pragma once
#include "hooks_base.h"

namespace cs::hooking
{
	inline etc::hook_vtbl* g_grcSwapChain{};
	inline HRESULT grcSwapChainPresent(rage::grcSwapChain* current, u32 syncInterval, u32 flags)
	{
		if (renderer::get())
		{
			renderer::get()->present();
		}

		return g_grcSwapChain->original<pointers::types::grcSwapChainPresentT>(8)(current, syncInterval, flags);
	}

	inline HRESULT grcSwapChainResizeBuffers(rage::grcSwapChain* current, u32 bufferCount, u32 width, u32 height, DXGI_FORMAT newFormat, u32 swapChainFlags)
	{
		HRESULT result{};
		ImGui_ImplDX11_InvalidateDeviceObjects();
		result = g_grcSwapChain->original<pointers::types::grcSwapChainResizeBuffersT>(13)(current, bufferCount, width, height, newFormat, swapChainFlags);
		if (SUCCEEDED(result))
		{
			ImGui_ImplDX11_CreateDeviceObjects();
		}

		return result;
	}
}