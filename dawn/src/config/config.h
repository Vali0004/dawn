#pragma once
#include "pch/framework.h"
#include "nlohmann/json.h"

namespace dwn::config
{
	class generic_config
	{
	public:

        generic_config(const std::fs::path& path, const std::string& filename) :
            m_config_path(path)
        {
            bool existed{ true };
            if (!std::fs::exists(m_config_path))
            {
                existed = false;
                std::fs::create_directories(m_config_path);
            }

            m_config_path /= filename;

            if (!existed)
            {
                save_to_file();
            }
        }
        virtual void from_json(nlohmann::json& json)
        {

        }
        virtual nlohmann::json to_json()
        {
            return m_json;
        }

        bool save_to_file()
        {
            std::ofstream file(m_config_path);
            if (!file.is_open())
            {
                return false;
            }

            nlohmann::json json = to_json();
            file << json.dump(4); // Pretty print with 4 spaces
            return true;
        }

        bool load_from_file()
        {
            std::ifstream file(m_config_path);
            if (!file.is_open())
            {
                return false;
            }

            nlohmann::json json{};
            file >> json;
            from_json(json);
            m_json = json;
            return true;
        }
	private:
		nlohmann::json m_json{};
		std::fs::path m_config_path{};
	};
}