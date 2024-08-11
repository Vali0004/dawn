#pragma once
#include "framework.h"
#include "json.h"

struct hotkey
{
	std::vector<uint8_t> m_keys{};
	bool m_combine{};
};

class config
{
public:
	config() :
		m_path(std::filesystem::path(std::getenv("appdata")).append("Dawn"))
	{
		if (!std::filesystem::exists(m_path))
		{
			std::filesystem::create_directories(m_path);
		}
		m_path = m_path.append("hotkey_config.json");
	}

	bool load_from_file()
	{
		std::ifstream file(m_path);
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

	void from_json(nlohmann::json& json)
	{
		for (auto& [id, hotkey_json] : json.items())
		{
			std::vector<uint8_t> keys{};
			if (hotkey_json.contains("keys") && hotkey_json["keys"].is_array())
			{
				for (auto& key_json : hotkey_json["keys"])
				{
					std::string key{ key_json.get<std::string>() };
					if (key.find("F"))
					{
						int fkey{ stoi(key.substr(1)) };
						keys.push_back((VK_F1 + fkey) - 1);
					}
					else
					{
						keys.push_back(key[0]);
					}
				}
			}

			hotkey hotkey{ keys, hotkey_json["combine_keys"].get<bool>() };
			m_hotkeys.insert(std::make_pair(id, hotkey));
		}
	}

	bool is_key_pressed(const std::string& id)
	{
		if (!valid(id))
		{
			return false;
		}

		hotkey hotkey{ m_hotkeys[id] };
		if (hotkey.m_keys.empty())
		{
			return false;
		}

		bool pressed{};
		for (auto& key : hotkey.m_keys)
		{
			if (hotkey.m_combine)
			{
				pressed = GetAsyncKeyState(key);
			}
			else
			{
				if (GetAsyncKeyState(key))
				{
					return true;
				}
			}
		}

		return pressed;
	}

	bool valid(const std::string& id)
	{
		if (!m_hotkeys.empty())
		{
			return false;
		}

		hotkey hotkey{ m_hotkeys[id] };
		if (hotkey.m_keys.empty())
		{
			return false;
		}

		return true;
	}
private:
	nlohmann::json m_json{};
	std::filesystem::path m_path{};
	std::unordered_map<std::string, hotkey> m_hotkeys{};
};
inline config g_config{};