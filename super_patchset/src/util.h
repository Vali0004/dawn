#pragma once
#include "pch/framework.h"

namespace cs::util
{
	inline void spawn_detached_thread(std::function<void()> cb)
	{
		std::thread(cb).detach();
	}
}