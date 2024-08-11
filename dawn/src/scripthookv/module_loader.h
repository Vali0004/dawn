#pragma once
#include "pch/framework.h"
#include "common/macros.h"

namespace dwn::shv
{
	class shv_module
	{
	public:
		shv_module() :
			m_path(stdfs::path(std::getenv("appdata")).append("Dawn").append("ScriptHookV"))
		{

		}
		~shv_module()
		{
			unload();
		}

		nodisc void load(const std::string& name) noexcept
		{
			m_name = name;
			m_path = m_path.append(m_name);
			std::string path{ m_path.string() };
			m_handle = LoadLibraryA(path.data());
		}

		nodisc void unload() noexcept
		{
			FreeLibrary(m_handle);
		}
	private:
		std::string m_name{};
		stdfs::path m_path{};
		HMODULE m_handle{};
	};
	class module_loader
	{
	public:

	private:
		std::vector<shv_module*> m_modules{};
	};
	inline module_loader g_module_loader{};
}