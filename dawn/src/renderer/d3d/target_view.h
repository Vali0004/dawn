#pragma once
#include "device.h"
#include "../DXGI/swapchain.h"

namespace dwn::directx
{
	class target_view : public com_ptr<ID3D11RenderTargetView>
	{
	public:
		void create(swapchain& sc, device& dev)
		{
			ID3D11Texture2D* back_buffer{};
			sc.get_buffer(back_buffer);

			dev.create_render_target(back_buffer, get_address());
			back_buffer->Release();
		}
	};
}