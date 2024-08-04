#pragma once
#include "hooks_base.h"
#include "menu/menu.h"
#include "renderer/input.h"

namespace dwn::renderer
{
	inline LRESULT renderer::wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		commands::gui::flip_bit(uMsg == WM_KEYUP && wParam == VK_INSERT);

		input::g_input.m_keyboard.wndproc(hWnd, uMsg, wParam, lParam);

		return CallWindowProcA(g_renderer->m_wndproc, hWnd, uMsg, wParam, lParam);
	}
}
namespace dwn::hooking
{
	inline etc::hook_vtbl* g_grcSwapChain{};
	inline HRESULT grcSwapChainPresent(rage::grcSwapChain* current, u32 syncInterval, u32 flags)
	{
		if (g_running && renderer::get())
		{
			renderer::get()->begin_frame();
			if (commands::gui::is_open())
			{
				commands::draw_menu();
			}
			renderer::get()->end_frame();
		}
		return g_grcSwapChain->original<pointers::types::grcSwapChainPresentT>(8)(current, syncInterval, flags);
	}

	inline HRESULT grcSwapChainResizeBuffers(rage::grcSwapChain* current, u32 bufferCount, u32 width, u32 height, DXGI_FORMAT newFormat, u32 swapChainFlags)
	{
		HRESULT result{};
		renderer::destroy(true);
		result = g_grcSwapChain->original<pointers::types::grcSwapChainResizeBuffersT>(13)(current, bufferCount, width, height, newFormat, swapChainFlags);
		if (SUCCEEDED(result))
		{
			renderer::create();
		}

		return result;
	}
}