#pragma once
#include "device.h"
#include <type_traits>

namespace dwn::directx
{
	class rasterizer_state : public com_ptr<ID3D11RasterizerState>
	{
	public:
		void create(device dev, D3D11_FILL_MODE fill)
		{
			D3D11_RASTERIZER_DESC desc{};
			desc.FillMode = fill;
			desc.CullMode = D3D11_CULL_NONE;
			desc.FrontCounterClockwise = FALSE;
			desc.DepthBias = 0;
			desc.DepthBiasClamp = 0.f;
			desc.SlopeScaledDepthBias = 0.f;
			desc.DepthClipEnable = TRUE;
			desc.ScissorEnable = TRUE;
			desc.MultisampleEnable = FALSE;
			desc.AntialiasedLineEnable = TRUE;
			dev.create_rasterizer_state(desc, get_address());
		}
	};
}