#pragma once
#include "d3d/device.h"
#include "d3d/context.h"
#include "com_ptr.h"

namespace dwn::directx
{
	template<typename T>
	inline void** get_as_iunknown(T** pp)
	{
		static_cast<IUnknown*>(*pp);
		return reinterpret_cast<void**>(pp);
	}

	class swapchain : public com_ptr<IDXGISwapChain>
	{
	public:
		void create(HWND window, device& dev, context& ctx, bool debug)
		{
			DXGI_SWAP_CHAIN_DESC sd{};
			sd.BufferCount = 2;
			sd.BufferDesc.Width = 0;
			sd.BufferDesc.Height = 0;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = window;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = TRUE;
			sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			UINT create_device_flags{};
			if (debug)
				create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;

			D3D_FEATURE_LEVEL feature_level{};
			const D3D_FEATURE_LEVEL feature_level_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

			D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, create_device_flags, feature_level_array, 2, D3D11_SDK_VERSION, &sd, get_address(), dev.get_address(), &feature_level, ctx.get_address());
		}

		void set(IDXGISwapChain* sc, device& dev, context& ctx)
		{
			get() = sc;
			get_device(dev.get());
			dev.get_context(ctx.get_address());
		}

		void present(bool vsync)
		{
			get()->Present(vsync ? 1 : 0, 0);
		}

		void resize(Vector2 size)
		{
			get()->ResizeBuffers(0, (uint32_t)size.x, (uint32_t)size.y, DXGI_FORMAT_UNKNOWN, NULL);
		}

		void get_buffer_impl(const IID& id, void** surface)
		{
			get()->GetBuffer(0, id, (void**)surface);
		}
		template <typename t>
		void get_buffer(t& surface)
		{
			get_buffer_impl(__uuidof(**(&surface)), get_as_iunknown(&surface));
		}

		void get_device_impl(const IID& id, void** surface)
		{
			get()->GetDevice(id, (void**)surface);
		}
		template <typename t>
		void get_device(t& device)
		{
			get_device_impl(__uuidof(**(&device)), get_as_iunknown(&device));
		}
	};
}