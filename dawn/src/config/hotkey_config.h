#pragma once
#include "config.h"

namespace dwn::config
{
	struct hotkey
	{
		std::vector<u8> m_keys{};
		bool m_combine{};
	};

	class hotkey_config : public generic_config
	{
	public:
		hotkey_config() :
			generic_config(std::fs::path(std::getenv("appdata")) / "Dawn", "hotkey_config.json")
		{
			add_hotkey("unload", VK_F12);
			add_hotkey("game_exit", VK_F4);
		}

		nlohmann::json to_json() override
		{
			nlohmann::json json{};
			for (auto& hotkey : m_hotkeys)
			{
				nlohmann::json key_json{
					{ "keys", nlohmann::json::array() },
					{ "combine_keys", false }
				};
				if (!hotkey.second.m_keys.empty())
				{
					for (auto& hk : hotkey.second.m_keys)
					{
						std::string key{};
						switch (hk)
						{
							case VK_END:
							{
								key = "END";
							} break;
							case VK_RETURN:
							{
								key = "ENTER";
							} break;
							case VK_BACK:
							{
								key = "BACK";
							} break;
							case VK_UP:
							{
								key = "UP";
							} break;
							case VK_DOWN:
							{
								key = "DOWN";
							} break;
							case VK_LEFT:
							{
								key = "LEFT";
							} break;
							case VK_RIGHT:
							{
								key = "RIGHT";
							} break;
							default:
							{
								if (hk < VK_F1)
								{
									key.push_back(hk);
									key.push_back('\0');
								}
								else
								{
									key = "F" + std::to_string((hk - VK_F1) + 1);
								}
							} break;
						}
						key_json["keys"].push_back(key);
					}
				}
				else
				{
					key_json["keys"] = nullptr;
				}
				json[hotkey.first] = key_json;
			}
			return json;
		}

		void from_json(nlohmann::json& json) override
		{
			for (auto& [id, hotkey_json] : json.items())
			{
				std::vector<u8> keys{};
				if (hotkey_json.contains("keys") && hotkey_json["keys"].is_array())
				{
					for (auto& key_json : hotkey_json["keys"])
					{
						std::string key{ key_json.get<std::string>() };
						switch (atStringHash(key))
						{
							case "END"_j:
							{
								keys.push_back(VK_END);
							} break;
							case "ENTER"_j:
							{
								keys.push_back(VK_RETURN);
							} break;
							case "BACK"_j:
							{
								keys.push_back(VK_BACK);
							} break;
							case "UP"_j:
							{
								keys.push_back(VK_UP);
							} break;
							case "DOWN"_j:
							{
								keys.push_back(VK_DOWN);
							} break;
							case "LEFT"_j:
							{
								keys.push_back(VK_LEFT);
							} break;
							case "RIGHT"_j:
							{
								keys.push_back(VK_RIGHT);
							} break;
							default:
							{
								if (key.find("F"))
								{
									s32 fkey{ stoi(key.substr(1)) };
									keys.push_back((VK_F1 + fkey) - 1);
								}
								else
								{
									keys.push_back(key[0]);
								}
							} break;
						}
					}
				}

				hotkey hotkey{ keys, hotkey_json["combine_keys"].get<bool>() };
				m_hotkeys.insert(std::make_pair(id, hotkey));
			}
		}

		void add_hotkey(const std::string& id, const u8 key, bool combine = false)
		{
			hotkey hotkey{ { key }, combine };
			m_hotkeys.insert(std::make_pair(id, hotkey));
		}

		void add_hotkey(const std::string& id, const std::vector<u8>& keys, bool combine = false)
		{
			hotkey hotkey{ keys, combine };
			m_hotkeys.insert(std::make_pair(id, hotkey));
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
		std::unordered_map<std::string, hotkey> m_hotkeys{};
	};
	inline hotkey_config g_hotkey_config{};
}