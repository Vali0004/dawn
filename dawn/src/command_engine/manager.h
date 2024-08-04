#pragma once
#include "pch/pch.h"
#include "abstract.h"
#include "types.h"

namespace dwn::commands
{
	class sub_manager
	{
	public:
		sub_manager(const std::string_view& name) :
			m_name(name)
		{}
		sub_manager() :
			sub_manager("")
		{}

		template <typename T = single_command, typename ...A>
		void add_cmd(A&&... args)
		{
			m_commands.push_back(reinterpret_cast<cmd_data<cmd_functions>*>(new T(args...)));
		}

		cmd_data<cmd_functions>* get_cmd_at(u64 index)
		{
			if (index >= num_commands())
			{
				return nullptr;
			}

			return m_commands.at(index);
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

		void check_if_init_impl()
		{
			for (auto& cmd : m_commands)
			{
				if (cmd->m_does_once_act_as_init)
				{
					if (!cmd->is_once_of<cmd_functions>())
					{
						// Function pointer will point towards whatever, once will always be void(this) for type so no need.
						cmd->get_once<cmd_functions>()->call(cmd);
					}
				}
			}
		}

		void loop_commands_impl()
		{
			for (auto& cmd : m_commands)
			{
				if (!cmd->is_tick_of<cmd_functions>())
				{
					cmd->get_tick<cmd_functions>()->call(cmd);
				}
			}
		}

		void loop_commands()
		{
			for (auto& sub : m_subs)
			{
				sub->loop_commands_impl();
			}
			loop_commands_impl();
		}

		void render_commands()
		{
			for (auto& cmd : m_commands)
			{
				if (!cmd->m_does_once_act_as_init)
				{
					if (!cmd->is_render_of<cmd_functions>())
					{
						cmd->get_render<cmd_functions>()->call(cmd, false);
					}
				}
			}
		}

		void check_if_init()
		{
			for (auto& sub : m_subs)
			{
				sub->check_if_init_impl();
			}
			check_if_init_impl();
		}

		void add_sub(sub_manager sub)
		{
			m_subs.push_back(new sub_manager(sub));
			add_cmd<group_command>(sub.get_name());
		}

		void add_sub(const std::string_view& name, sub_manager sub)
		{
			sub.get_name() = name;
			m_subs.push_back(new sub_manager(sub));
		}

		sub_manager* get_sub(const std::string_view& name)
		{
			for (auto& sub : m_subs)
			{
				if (!sub->get_name().compare(name.data()))
				{
					return sub;
				}
			}

			return nullptr;
		}

		std::string& get_name()
		{
			return m_name;
		}

		u64 num_commands()
		{
			return m_commands.size();
		}

	private:
		std::string m_name{};
		std::vector<cmd_data<cmd_functions>*> m_commands{};
		std::vector<sub_manager*> m_subs{};
	};

	class manager
	{
	public:
		void push_back(const std::string_view& name)
		{
			m_subs.push_back(new sub_manager(name));
		}

		void push_back(sub_manager sub)
		{
			m_subs.push_back(new sub_manager(sub));
		}

		void push_back(const std::string_view& name, sub_manager sub)
		{
			sub.get_name() = name;
			m_subs.push_back(new sub_manager(sub));
		}

		sub_manager* get_sub(const std::string_view& name)
		{
			for (auto& cmd : m_subs)
			{
				if (!cmd->get_name().compare(name.data()))
				{
					return cmd;
				}
			}

			return nullptr;
		}
	private:
		std::vector<sub_manager*> m_subs{};
	};
	inline manager g_manager{};

	inline void no_clip_init(single_command* cmd)
	{
		cmd->add_container_preallocate("fly_multiplier", 1.f);
		cmd->add_container_preallocate("fly_toggle", true);
	}

	inline void no_clip_tick(single_command* cmd)
	{
		cmd_container& fly_multipier{ *cmd->get_container("fly_multiplier") };
		cmd_container& fly_toggle{ *cmd->get_container("fly_toggle") };
		if (fly_toggle.get<bool>())
		{
			LOG_TO_STREAM("Fly? Fly.");
		}
		LOG_TO_STREAM("Multiplier: " << fly_multipier.get<float>());
	}

	// group managers together.
	inline void test()
	{
		sub_manager self_movement{};
		self_movement.add_cmd<single_command>("no_clip", &no_clip_init, &no_clip_tick, true);
		sub_manager self{};
		self.add_sub(self_movement);
		self.add_cmd<single_command>("test_option", [](single_command* cmd) {
			auto& value = *cmd->add_container_preallocate("test_multiplier", 1.f);
			LOG_TO_STREAM("Container value: " << value.get<float>());
		});
		self.add_cmd<single_command>("test_option2", [](single_command* cmd) {

		});
		self.add_cmd<single_command>("test_option3", [](single_command* cmd) {

		});
		g_manager.push_back("self", self);
	}
}