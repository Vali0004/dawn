#pragma once
#include "pch/pch.h"

namespace cs::renderer::fonts
{
	class font
	{
	public:
		font(const stdfs::path& path, const f32& size) :
			m_font(nullptr), 
			m_path(path),
			m_type(0),
			m_size(size)
		{
			m_name = path.filename().string();
		}

		font(const stdfs::path& path, const f32& size, ImFontConfig cfg) :
			m_config(cfg),
			m_font(nullptr),
			m_path(path),
			m_type(0),
			m_size(size)
		{
			m_name = path.filename().string();
		}

		font(const std::string& name, const f32& size, u8* memory, u64 memory_size) :
			m_font(nullptr),
			m_name(name),
			m_type(1),
			m_size(size)
		{
			m_mem.resize(memory_size);
			memcpy(m_mem.data(), memory, m_mem.size());
		}

		font(const std::string& name, const f32& size, const u8* memory, u64 memory_size) :
			m_font(nullptr),
			m_name(name),
			m_type(1),
			m_size(size)
		{
			m_mem.resize(memory_size);
			memcpy(m_mem.data(), memory, m_mem.size());
		}

		font(const std::string& name, const f32& size, u8* memory, u64 memory_size, ImFontConfig cfg) :
			m_config(cfg),
			m_font(nullptr),
			m_name(name),
			m_type(1),
			m_size(size)
		{
			m_mem.resize(memory_size);
			memcpy(m_mem.data(), memory, m_mem.size());
		}

		font(const std::string& name, const f32& size, const u8* memory, u64 memory_size, ImFontConfig cfg) :
			m_config(cfg),
			m_font(nullptr),
			m_name(name),
			m_type(1),
			m_size(size)
		{
			m_mem.resize(memory_size);
			memcpy(m_mem.data(), memory, m_mem.size());
		}

		explicit font(const f32& size) :
			m_font(nullptr),
			m_name("default"),
			m_type(2),
			m_size(size)
		{}
		font(const f32& size, ImFontConfig cfg) :
			m_config(cfg),
			m_font(nullptr),
			m_name("default"),
			m_type(2),
			m_size(size)
		{}

		ImFont* register_in_atlas(ImFontAtlas* atlas)
		{
			switch (m_type)
			{
				case 0:
				{
					strcpy_s(m_config.Name, m_name.data());
					m_font = atlas->AddFontFromFileTTF(m_path.string().data(), m_size, &m_config, m_config.GlyphRanges);
				} break;
				case 1:
				{
					strcpy_s(m_config.Name, m_name.data());
					LOG(Info, "Registering mem font {} (type: {}, size: {})", m_name, m_type, m_mem.size());
					if (m_mem.data())
					{
						m_font = atlas->AddFontFromMemoryTTF(m_mem.data(), static_cast<int>(m_mem.size()), m_size, &m_config);
					}
					else if (!m_mem.data() || !m_font)
					{
						LOG(Error, "Failed to load font!");
					}
				} break;
				case 2:
				{
					m_font = atlas->AddFontDefault(&m_config);
				} break;
			}

			return m_font;
		}

		ImFont*& imfont()
		{
			return m_font;
		}
		ImFontConfig& imfontconfig()
		{
			return m_config;
		}

		const stdfs::path& path()
		{
			return m_path;
		}

		const std::string& name()
		{
			return m_name;
		}

		f32& size()
		{
			return m_size;
		}
	private:
		ImFontConfig m_config;
		ImFont* m_font;
		std::vector<u8> m_mem;
		stdfs::path m_path;
		std::string m_name;
		s32 m_type;
		f32 m_size;
	};
	class font_mgr
	{
	public:
		font_mgr()
		{}
		~font_mgr()
		{
			m_fonts.clear();
		}
		ImFont* add_font_to_atlas(ImFontAtlas* atlas, f32 size = 19.f)
		{
			font f(size);
			m_fonts.push_back(f);
			return f.register_in_atlas(atlas);
		}
		ImFont* add_font_to_atlas(ImFontAtlas* atlas, font f)
		{
			m_fonts.push_back(f);
			return f.register_in_atlas(atlas);
		}
		ImFont* add_font_to_atlas(ImFontAtlas* atlas, const std::filesystem::path& path, f32 size = 19.f)
		{
			font f(path.string(), size);
			m_fonts.push_back(f);
			return f.register_in_atlas(atlas);
		}
		ImFont* add_font_to_atlas(ImFontAtlas* atlas, const std::string& name, u8* memory, u64 memory_size, f32 size = 19.f)
		{
			font f(name, size, memory, memory_size);
			m_fonts.push_back(f);
			return f.register_in_atlas(atlas);
		}
		ImFont* add_font_to_atlas(ImFontAtlas* atlas, const std::string& name, const u8* memory, u64 memory_size, f32 size = 19.f)
		{
			font f(name, size, memory, memory_size);
			m_fonts.push_back(f);
			return f.register_in_atlas(atlas);
		}
		void append_to_previous_font(ImFontAtlas* atlas, const std::filesystem::path& name, const ImWchar* ranges = nullptr, f32 size = 19.f)
		{
			ImFontConfig cfg{};
			cfg.MergeMode = true;
			cfg.PixelSnapH = true;
			cfg.GlyphMinAdvanceX = size;
			cfg.GlyphRanges = ranges;

			font f(name, size, cfg);
			m_fonts.push_back(f);
			f.register_in_atlas(atlas);
		}
		void update(ImFontAtlas* atlas)
		{
			if (!should_update())
			{
				return;
			}

			ImGui_ImplDX11_InvalidateDeviceObjects();

			atlas->Clear();
			for (auto& f : m_fonts)
			{
				m_fonts_backup.push_back(f);
				f.imfont() = f.register_in_atlas(atlas);
			}

			ImGui_ImplDX11_CreateDeviceObjects();
		}

		// vali: Fix to poor performance, because anything else is fucking horrible.
		bool should_update() const
		{
			return m_fonts_backup.empty() || m_fonts.size() != m_fonts_backup.size();
		}
		font* get_font(const std::string& name)
		{
			for (auto& f : m_fonts)
			{
				if (!f.name().compare(name))
				{
					return &f;
				}
			}

			return nullptr;
		}
	private:
		bool m_init_once{};
		std::vector<font> m_fonts{};
		std::vector<font> m_fonts_backup{};
	};
	inline font_mgr g_font_mgr{};
}