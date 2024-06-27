#pragma once
#include "pch/framework.h"
#include "commands/list.h"
#include "memory/pointers.h"

namespace cs::util
{
	inline void spawn_detached_thread(std::function<void()> cb)
	{
		std::thread(cb).detach();
	}
}

namespace cs::util::math
{
	constexpr long double PI{ 3.141592653589793238462643383279502884L };

	inline float degrees_to_radians(float degrees)
	{
		//Formula: x * PI/180 = y rad
		return static_cast<float>((static_cast<long double>(degrees) * PI) / 180.L);
	}

	inline float distance_3D(Vector3 a, Vector3 b)
	{
		return (float)sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
	}

	inline Vector3 rotation_to_direction(const Vector3 rot)
	{
		float xRadian{ degrees_to_radians(rot.x) };
		float zRadian{ degrees_to_radians(rot.z) };

		float absoluteXRadian{ fabs(cos(xRadian)) };

		return { (-sin(zRadian)) * absoluteXRadian, cos(zRadian) * absoluteXRadian, sin(xRadian) };
	}

	inline float distance_between_vectors(Vector3 a, Vector3 b)
	{
		return static_cast<float>(sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2)));
	}
}

namespace cs::util::gta
{
	inline bool request_model(u32 hash)
	{
		for (int i{}; i != 2; ++i)
		{
			for (int i{}; i != 200 && !STREAMING::HAS_MODEL_LOADED(hash); ++i)
			{
				STREAMING::REQUEST_MODEL(hash);
			}
		}

		return STREAMING::HAS_MODEL_LOADED(hash);
	}

	inline bool in_loading_screen()
	{
		return *pointers::g_LoadingScreenContext != LOADINGSCREEN_CONTEXT_NONE;
	}
}