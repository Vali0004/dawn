#pragma once
#include "device.h"
#include "texture_2d.h"
#include "../vectors.h"

namespace dwn::directx
{
	class shader_resource_view : public com_ptr<ID3D11ShaderResourceView>
	{
	public:
		void create(device& dev, texture_2d& tex)
		{
			dev.create_shader_resource_view(tex.get(), tex.desc.Format, get());
		}
	};
}