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
			m_path(std::getenv("appdata")),
			generic_config(m_path.append("Dawn"), "command_config.json")
		{}
		nlohmann::json to_json() override
		{
			nlohmann::json json{};
			return json;
		}
		void from_json(nlohmann::json& json) override
		{

		}

	private:
		stdfs::path m_path{};
	};
}