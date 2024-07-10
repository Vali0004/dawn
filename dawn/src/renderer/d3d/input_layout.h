#pragma once
#include "device.h"
#include "blob.h"

namespace dwn::directx
{
	class input_layout : public com_ptr<ID3D11InputLayout>
	{
	public:
		void create(device& dev, blob& blob)
		{
			dev.create_input_layout(blob.get_buffer(), blob.get_buffer_size(), get_address());
		}
	};
}