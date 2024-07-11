#pragma once
#include "memory/pointers.h"
#include "menu/menu.h"

namespace dwn::renderer
{
	class renderer;
	inline std::unique_ptr<renderer> g_renderer{};

	class renderer final
	{
	public:
		void create_ctx()
		{
			if (!ImGui::GetCurrentContext())
			{
				ImGuiContext* ctx = ImGui::CreateContext();
				ImGui::SetCurrentContext(ctx);
			}
		}

		void create()
		{
			m_wndproc = WNDPROC(SetWindowLongPtrA(pointers::g_hwnd, GWLP_WNDPROC, LONG_PTR(&wndproc)));
			m_swapchain = *pointers::g_pSwapChain;

			m_swapchain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(m_device.GetAddressOf()));
			m_device->GetImmediateContext(m_context.GetAddressOf());

			ImGui::CreateContext();

			ImGui_ImplDX11_Init(m_device.Get(), m_context.Get());
			ImGui_ImplWin32_Init(pointers::g_hwnd);

			menu::init();
		}

		void destroy()
		{
			SetWindowLongPtrA(pointers::g_hwnd, GWLP_WNDPROC, LONG_PTR(m_wndproc));

			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();

			if (ImGui::GetCurrentContext())
			{
				ImGui::DestroyContext();
			}
		}

		static LRESULT wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			gui::flip_bit(uMsg == WM_KEYUP && wParam == VK_INSERT);

			if (ImGui::GetCurrentContext())
			{
				ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
			}

			return CallWindowProcA(g_renderer->m_wndproc, hWnd, uMsg, wParam, lParam);
		}

		void present()
		{
			static ImGuiIO& io{ ImGui::GetIO() };
			io.MouseDrawCursor = gui::g_open;
			create_ctx();

			ImGui_ImplWin32_NewFrame();
			ImGui_ImplDX11_NewFrame();

			//fonts::g_font_mgr.update(io.Fonts);

			ImGui::NewFrame();
			callbacks::run();
			//menu::tick();
			if (gui::is_open())
			{
				menu::render();
			}
			ImGui::EndFrame();

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapchain{};
		Microsoft::WRL::ComPtr<ID3D11Device> m_device{};
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context{};
	private:
		WNDPROC m_wndproc{};
	};

	inline void create()
	{
		g_renderer = std::make_unique<renderer>();
		g_renderer->create();
	}

	inline renderer* get()
	{
		return g_renderer.get();
	}

	inline void destroy()
	{
		g_renderer->destroy();
		g_renderer.reset();
	}
}