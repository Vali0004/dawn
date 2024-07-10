#pragma once
#include "pch/pch.h"
#include "abstract.h"

namespace dwn::commands
{
	class manager
	{
	public:
		template <typename T, typename ...A>
		void add_cmd(A&&... args)
		{
			m_commands.push_back(reinterpret_cast<cmd_data<cmd_functions>*>(new T(args...)));
		}
	private:
		std::vector<cmd_data<cmd_functions>*> m_commands{};
	};
}