#pragma once
#include "device.h"

namespace dwn::directx
{
	class sampler_state : public com_ptr<ID3D11SamplerState>
	{
	public:
		void create(device& dev, Vector2 LOD)
		{
			D3D11_SAMPLER_DESC desc{};
			desc.Filter = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
			desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.MaxAnisotropy = NULL;
			desc.MipLODBias = 0.f;
			desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			desc.MinLOD = LOD.x;
			desc.MaxLOD = LOD.y;
			dev.create_sampler_state(desc, get_address());
		}
	};
}