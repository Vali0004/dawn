#pragma once
#include "config.h"
#include "command_engine/manager.h"

namespace dwn::config
{
	// I'll do a actual hotkey manager at some point
	class command_config : public generic_config
	{
	public:
		command_config() :
			generic_config(std::fs::path(std::getenv("appdata")) / "Dawn", "command_config.json")
		{}

		nlohmann::json to_json() override
		{
			return commands::g_manager.to_json();
		}

		void from_json(nlohmann::json& json) override
		{
			commands::g_manager.from_json(json);
			if (json.contains("subs") && json["subs"].is_object())
			{

			}
			update_json(json);
			save_to_file();
		}
	};
	inline command_config g_command_config{};
}