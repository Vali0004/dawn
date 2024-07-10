#pragma once
#include "com_ptr.h"
#include <d3dcompiler.h>

namespace dwn::directx
{
	class blob : public com_ptr<ID3D10Blob>
	{
	public:
		void create(size_t size)
		{
			D3DCreateBlob(size, get_address());
		}

		void* get_buffer()
		{
			return get()->GetBufferPointer();
		}
		size_t get_buffer_size()
		{
			return get()->GetBufferSize();
		}
	};
}