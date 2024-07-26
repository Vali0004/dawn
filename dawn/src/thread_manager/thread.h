#pragma once
#include "common/macros.h"
#include "common/data/types.h"
#include <corecrt_terminate.h>
#include <thread>

namespace dwn
{
	class thread
	{
	public:
		nodisc explicit thread(fptr<u32(void*)> fn, void* arg = nullptr)
		{
			_start(fn, arg);
		}
		thread() noexcept : m_data{}
		{}

		~thread() noexcept
		{
			if (m_data.id == nil)
			{
				terminate();
			}
		}

		void start(bool join = {})
		{
			if (m_data.id == nil)
			{
				return;
			}
			else
			{
				if (join)
				{
					if (m_data.id == _Thrd_id())
					{
						return;
					}
					else
					{
						_Thrd_join({ m_data.handle, m_data.id }, nil);
					}
				}
			}
		}

		nodisc u32 get_id() noexcept
		{
			return m_data.id;
		}

		nodisc void* handle()
		{
			return m_data.handle;
		}

	private:
		void _start(fptr<u32(void*)> fn, void* arg)
		{
			m_data.handle = reinterpret_cast<void*>(_beginthreadex(nilptr, nil, fn, arg, nil, &m_data.id));
			if (!m_data.handle)
			{
				m_data.id = 0;
			}
		}

		struct data
		{
			void* handle;
			u32 id;
		};
		data m_data;
	};
}