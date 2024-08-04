#pragma once
#include "pch/pch.h"
#include "abstract.h"
#include "types.h"

namespace dwn::commands
{
	class sub_manager
	{
	public:
		sub_manager(const std::string_view& name, u64 order_id = 1) :
			m_name(name),
			m_order_id(order_id),
			m_enabled(true)
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

		template <typename T = cmd_data<cmd_functions>>
		T* get_cmd(const std::string_view& id)
		{
			for (auto& cmd : m_commands)
			{
				if (!cmd->m_id.compare(id.data()))
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
			for (const auto& [id, sub] : m_subs)
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
			for (const auto& [id, sub] : m_subs)
			{
				sub->check_if_init_impl();
			}
			check_if_init_impl();
		}

		nlohmann::json to_json()
		{
			nlohmann::json json{
				{ "label", m_name },
				{ "enabled", m_enabled },
				{ "order_id", number_to_word(m_order_id, true) },
				{ "subs", nullptr },
				{ "options", nullptr }
			};

			if (!m_subs.empty())
			{
				nlohmann::json subs_json{};
				for (const auto& [id, sub] : m_subs)
				{
					subs_json[id] = sub->to_json();
				}
				json["subs"] = subs_json;
			}
			else
			{
				json["subs"] = nullptr;
			}

			if (!m_commands.empty())
			{
				json["options"] = nlohmann::json::array();
				for (auto& cmd : m_commands)
				{
					json["options"].push_back(cmd->to_json());
				}
			}
			else
			{
				json["options"] = nullptr;
			}

			return json;
		}

		void from_json(nlohmann::json& json)
		{
			m_name = json.value("label", m_name);
			m_enabled = json.value("enabled", m_enabled);
			m_order_id = json.value("order_id", m_order_id);

			if (json.contains("subs") && json["subs"].is_object())
			{
				for (auto& [id, sub_json] : json["subs"].items())
				{
					auto sub{ get_sub(id) };
					if (sub)
					{
						sub->from_json(sub_json);
					}
				}
			}

			if (json.contains("options") && json["options"].is_array())
			{
				for (auto& cmd_json : json["options"])
				{
					auto cmd{ get_cmd(cmd_json) };
					if (cmd)
					{
						cmd->from_json(cmd_json);
					}
				}
			}
		}

		void add_sub(sub_manager sub)
		{
			m_subs.insert(std::make_pair(sub.get_name(), new sub_manager(sub)));
			add_cmd<group_command>(sub.get_name());
		}

		void add_sub(const std::string_view& name, sub_manager sub)
		{
			sub.get_name() = name;
			m_subs.insert(std::make_pair(name, new sub_manager(sub)));
		}

		sub_manager* get_sub(const std::string_view& name)
		{
			for (const auto& [id, sub] : m_subs)
			{
				if (!id.compare(name.data()))
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

		u64 get_num_usable_commands()
		{
			size_t i{};
			for (auto& cmd : m_commands)
			{
				if (!cmd->m_does_once_act_as_init && cmd->m_enabled)
				{
					if (!cmd->is_once_of<cmd_functions>())
					{
						i++;
					}
				}
			}
			return i;
		}

		u64 num_commands()
		{
			return m_commands.size();
		}

		bool is_enabled()
		{
			return m_enabled;
		}
	private:
		bool m_enabled{};
		std::string m_name{};
		u64 m_order_id{};
		std::vector<cmd_data<cmd_functions>*> m_commands{};
		std::unordered_map<std::string, sub_manager*> m_subs{};
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
}