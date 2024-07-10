#pragma once
#include "pch/pch.h"
#include "common/joaat.h"

namespace dwn::ragesec
{
	inline constexpr std::array<u32, 5> g_name_hash
	{
		"fwClothMeshing"_j,
		"CCreditsText"_j,
		"rageSecEngine"_j,
		"rageSecGamePluginManager"_j,
		"TamperActions"_j
	};

	inline constexpr bool invalid_hash(u32 hash)
	{
		for (auto& nh : g_name_hash)
		{
			if (nh == hash)
			{
				return true;
			}
		}
		return false;
	}
}