#pragma once
#include "device.h"

namespace dwn::directx
{
	class depth_stencil_state : public com_ptr<ID3D11DepthStencilState>
	{
	public:
		void create(device& dev, bool read, bool write, D3D11_COMPARISON_FUNC depth)
		{
			D3D11_DEPTH_STENCIL_DESC desc{};
			desc.DepthEnable = read ? TRUE : FALSE;
			desc.DepthWriteMask = write ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.DepthFunc = depth;
			desc.StencilEnable = FALSE;
			desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace = desc.FrontFace;
			dev.create_depth_stencil_state(desc, get_address());
		}
	};
}