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
		template <typename T>
		T* get_cmd(const std::string_view& cmdName)
		{
			for (auto& cmd : m_commands)
			{
				if (!cmd->m_name.compare(cmdName.data()))
				{
					return reinterpret_cast<T*>(cmd);
				}
			}

			return nullptr;
		}
	private:
		std::vector<cmd_data<cmd_functions>*> m_commands{};
	};
	inline manager g_manager{};
}