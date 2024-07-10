#pragma once
#include "pch/pch.h"
#include "abstract.h"

namespace dwn::commands
{
	class manager
	{
	public:

	private:
		std::vector<cmd_data<cmd_functions>> m_commands{};
	};
}