#pragma once
#include "device.h"
#include "context.h"
#include "../vectors.h"

namespace dwn::directx
{
	class texture_2d : public com_ptr<ID3D11Texture2D>
	{
	public:
		void create(device& dev, Vector2 size, const void* data, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, uint8_t bpp = 4)
		{
			desc.Width = static_cast<int>(size.x);
			desc.Height = static_cast<int>(size.y);
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = format;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA subResource{};
			subResource.pSysMem = data;
			subResource.SysMemPitch = desc.Width * bpp;
			subResource.SysMemSlicePitch = 0;
			dev.create_texture_2d(desc, subResource, get());
		}
		void update(context& ctx, const void* data, Vector2 size, uint8_t bpp = 4)
		{
			ctx.update_subresource(reinterpret_cast<ID3D11Resource*&>(get()), data, size, bpp);
		}

		D3D11_TEXTURE2D_DESC desc{};
	};
}