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
			if (!std::fs::exists(m_config_path))
			{
				m_mark_as_create = true;
				std::fs::create_directories(m_config_path);
			}

			m_config_path /= filename;

			std::ifstream file(m_config_path);
			if (!file.is_open())
			{
				m_mark_as_create = true;
			}
			file.close();
		}

		virtual void from_json(nlohmann::json& json)
		{

		}

		virtual nlohmann::json to_json()
		{
			return m_json;
		}

		bool handle_init()
		{
			if (m_mark_as_create)
			{
				update_json(to_json());
				m_mark_as_create = save_to_file();
				return m_mark_as_create;
			}

			return load_from_file();
		}

		void update_json(nlohmann::json json)
		{
			m_json = json;
		}

		bool save_to_file()
		{
			std::ofstream file(m_config_path);
			if (!file.is_open())
			{
				return false;
			}

			file << m_json.dump(1, '	');
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
			update_json(json);
			return true;
		}
	private:
		nlohmann::json m_json{};
	public:
		bool m_mark_as_create{};
	private:
		std::fs::path m_config_path{};
	};
}