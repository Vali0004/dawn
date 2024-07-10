#pragma once
#include "device.h"

namespace dwn::directx
{
	class buffer : public com_ptr<ID3D11Buffer>
	{
	public:
		void create(device& dev, uint32_t size, D3D11_BIND_FLAG bindFlag)
		{
			D3D11_BUFFER_DESC desc{};
			desc.ByteWidth = size;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = bindFlag;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = NULL;
			desc.StructureByteStride = 0;
			dev.create_buffer(desc, get_address());
		}
	};
}