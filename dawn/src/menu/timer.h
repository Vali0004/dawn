#pragma once
#include "pch/framework.h"

namespace dwn
{
	class simple_timer
	{
	public:
		void start(uint64_t ticks)
		{
			if (m_tick)
			{
				m_ready_at = GetTickCount64() + ticks;
				m_tick = false;
			}
		}
		bool is_ready()
		{
			return GetTickCount64() > m_ready_at;
		}
		void reset()
		{
			m_tick = true;
		}
	public:
		u64 m_ready_at{};
		bool m_tick{};
	};
}