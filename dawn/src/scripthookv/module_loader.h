#pragma once
#include "pch/framework.h"
#include "common/macros.h"

namespace dwn::shv
{
	class shv_module
	{
	public:
		shv_module(const std::string& name, const std::string& path_append = "ScriptHookV") :
			m_path(std::fs::path(std::getenv("appdata")) / "Dawn")
		{
			if (!path_append.empty())
			{
				m_path /= path_append;
			}
			load(name);
		}
		~shv_module()
		{
			unload();
		}

		nodisc void load(const std::string& name)
		{
			m_name = name;
			m_path = m_path.append(m_name);
			std::string path{ m_path.string() };
			LOG_TO_STREAM("Path: " << path);
			m_handle = LoadLibraryA(path.data());
		}

		nodisc void unload() noexcept
		{
			if (!m_handle)
			{
				return;
			}

			FreeLibrary(m_handle);
		}

		nodisc std::string get_name() noexcept
		{
			return m_name;
		}
	private:
		std::string m_name{};
		std::fs::path m_path{};
		HMODULE m_handle{};
	};

	class module_loader
	{
	public:
		void load(const std::string& name)
		{
			if (!m_scripthook_module.get())
			{
				m_scripthook_module = std::make_unique<shv_module>("ScriptHookV.dll", std::string());
			}

			m_modules.push_back(new shv_module(name));
		}

		void unload(const std::string& name)
		{
			for (auto& module : m_modules)
			{
				if (!module->get_name().compare(name.data()))
				{
					delete module;
					break;
				}
			}
		}

		void unload_all()
		{
			for (auto& module : m_modules)
			{
				delete module;
			}

			if (m_scripthook_module.get())
			{
				m_scripthook_module.reset();
			}
		}
	private:
		std::unique_ptr<shv_module> m_scripthook_module{};
		std::vector<shv_module*> m_modules{};
	};
	inline module_loader g_module_loader{};
}