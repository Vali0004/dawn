#pragma once
#include "hooks_base.h"
#include "menu/menu.h"
#include "renderer/input.h"
#include "scripthookv/scripthookv.h"

namespace dwn::shv
{
	inline void wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (g_keyboard_functions.empty())
		{
			return;
		}

		if (uMsg == WM_KEYDOWN || uMsg == WM_KEYUP || uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP)
		{
			for (auto& f : g_keyboard_functions)
			{
				bool with_alt{ uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP };
				bool extended{ (lParam >> 24) & 1 ? true : false };
				bool was_down_before{ (lParam >> 30) & 1 ? true : false };
				bool is_up{ uMsg == WM_SYSKEYUP || uMsg == WM_KEYUP };
				f.second(static_cast<ul32>(wParam), lParam & 0xFFFF, (lParam >> 16) & 0xFF, extended, with_alt, was_down_before, is_up);
			}
		}
	}
	inline void loop_callbacks(IDXGISwapChain* sc)
	{
		if (g_present_callbacks.empty())
		{
			return;
		}

		for (auto& present_cb : g_present_callbacks)
		{
			present_cb.second(reinterpret_cast<void*>(sc));
		}
	}
}
namespace dwn::renderer
{
	inline LRESULT renderer::wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		commands::gui::flip_bit(uMsg == WM_KEYUP && wParam == VK_INSERT);

		input::g_input.m_keyboard.wndproc(hWnd, uMsg, wParam, lParam);

		shv::wndproc(hWnd, uMsg, wParam, lParam);

		return CallWindowProcA(g_renderer->m_wndproc, hWnd, uMsg, wParam, lParam);
	}
}
namespace dwn::hooking
{
	inline etc::hook_vtbl* g_grcSwapChain{};
	inline HRESULT grcSwapChainPresent(rage::grcSwapChain* current, u32 syncInterval, u32 flags)
	{
		if (g_running)
		{
			shv::loop_callbacks(current);
		}
		if (g_running && renderer::get() && !renderer::get()->m_no_render)
		{
			if (commands::gui::is_open())
			{
				renderer::get()->begin_frame();
				commands::draw_menu();
				renderer::get()->end_frame();
			}
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