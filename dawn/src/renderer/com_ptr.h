#pragma once
#include <cstdint>

namespace dwn::directx
{
	template <typename T>
	class com_ptr
	{
	public:
		T*& get()
		{
			return m_handle;
		}
		T** get_address()
		{
			return &get();
		}

		auto release()
		{
			auto addr{ get_address() };
			if (addr && *addr)
			{
				(*addr)->Release();
			}
			return addr;
		}

		bool valid()
		{
			return m_handle != nullptr;
		}

	private:
		T* m_handle{};
	};
}