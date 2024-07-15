#pragma once
#include "pch/pch.h"
#include "common/virtual_pure.h"
#include "hooking/hooks.h"
#include "renderer/input.h"

namespace dwn::renderer
{
	namespace gui
	{
		inline bool g_open{};
		inline bool is_open()
		{
			return g_open;
		}

		inline void flip_bit(bool trigger_flip = false)
		{
			if (trigger_flip)
			{
				g_open ^= 1;
			}
		}
	}
	inline void draw_text_right(directx::font& f, const char* text, const Vector2 position, const Color color)
	{
		const Vector2 string_size{ get_string_size(f, text) / get()->m_display_size };
		const Vector2 pos{ position.x - string_size.x - 0.0005f, position.y };
		get()->draw_text(f, text, pos, color);
	}
	inline float g_drawbase{};
	inline directx::font iconfont_22{};
	inline directx::font iconfont_24{};
	inline directx::font arial_22{};
	inline directx::font arial_23{};
	inline directx::font arialbd_22{};
	inline directx::font arialbd_23{};
	inline directx::font ariali_22{};
	inline directx::font ariali_23{};
	inline directx::font arialbi_22{};
	inline directx::font arialbi_23{};
	class abstract_option
	{
	public:
		abstract_option(const std::string& name) : m_name(name)
		{
		}
		virtual void action(int type) {}
		virtual void draw(bool selected) {}
		std::string m_name{};
	};
	class submenu
	{
	public:
		submenu(std::string name, fptr<void(submenu&)> action) : m_name(name), m_action(action)
		{}
		submenu() : submenu(std::string(), {})
		{}
		~submenu()
		{
			m_options.clear();
		}
		template <typename T>
		void add(T option)
		{
			m_options.emplace_back(reinterpret_cast<abstract_option*>(new T(option)));
		}
		void action(int type)
		{
			if (type == 1)
			{
				if (m_current > 0)
					m_current--;
				else
					m_current = m_options.size() - 1;
			}
			else if (type == 2)
			{
				if (m_current < m_options.size() - 1)
					m_current++;
				else
					m_current = 0;
			}
			if (!m_options.empty())
				m_options[m_current]->action(type);
		}
		void call()
		{
			if (m_action)
			{
				m_action(*this);
			}
		}
		void handle(fptr<void()> no_option_cb)
		{
			if (m_options.empty())
			{
				no_option_cb();
			}
			for (size_t i{}; i != m_options.size(); ++i)
			{
				m_options[i]->draw(m_current == i);
			}
			if (m_current >= m_options.size())
			{
				m_current = 0;
			}
		}
		std::string m_name{};
		std::vector<std::shared_ptr<abstract_option>> m_options{};
		u64 m_current{};
		fptr<void(submenu&)> m_action{};
	};
	inline submenu make_sub(std::string str, fptr<void(submenu&)> action)
	{
		submenu s{ str, action };
		s.call();
		return s;
	}
	inline submenu g_submenu{};
	inline std::stack<submenu> g_submenus{};
	inline void push(submenu sub)
	{
		g_submenu = sub;
		g_submenus.push(sub);
	}
	inline void pop()
	{
		if (g_submenus.size() > 1 && g_submenus.top().m_name != "Home")
		{
			g_submenus.pop();
		}
		else
		{
			gui::flip_bit(true);
		}
	}
	class base_option : public abstract_option
	{
	public:
		base_option(std::string name, fptr<void()> callback = {}) :
			abstract_option(name),
			m_callback(callback)
		{
		}
		void action(int type)
		{
			if (type == 0)
			{
				if (m_callback)
				{
					m_callback();
				}
			}
		}
		void draw(bool selected)
		{
			get()->draw_rect({ 0.01f, g_drawbase }, { 0.13f, 0.02f }, selected ? directx::Color{ 255, 255, 255, 255 } : directx::Color{ 0, 0, 0, 130 });
			get()->draw_text(arial_22, m_name.data(), { 0.01f + 0.001f, g_drawbase + (0.02f / 2.f) }, selected ? directx::Color{ 0, 0, 0, 255 } : directx::Color{ 255, 255, 255, 255 });
			g_drawbase += 0.02f;
		}
		fptr<void()> m_callback{};
	};
	class submenu_option : public abstract_option
	{
	public:
		submenu_option(std::string name, fptr<void(submenu&)> callback = {}) :
			abstract_option(name),
			m_callback(callback)
		{
		}
		void action(int type)
		{
			if (type == 0)
			{
				if (m_callback)
				{
					g_submenu = make_sub(m_name, m_callback);
					push(g_submenu);
				}
			}
		}
		void draw(bool selected)
		{
			get()->draw_rect({ 0.01f, g_drawbase }, { 0.13f, 0.02f }, selected ? directx::Color{ 255, 255, 255, 255 } : directx::Color{ 0, 0, 0, 130 });
			get()->draw_text(arial_22, m_name.data(), { 0.01f + 0.001f, g_drawbase + (0.02f / 2.f) }, selected ? directx::Color{ 0, 0, 0, 255 } : directx::Color{ 255, 255, 255, 255 });
			draw_text_right(iconfont_24, "r", { 0.01f + (0.131f - 0.0005f), g_drawbase + (0.02f / 2.f) + 0.006f }, selected ? directx::Color{ 0, 0, 0, 255 } : directx::Color{ 255, 255, 255, 255 });
			g_drawbase += 0.02f;
		}
		submenu m_submenu{};
		fptr<void(submenu&)> m_callback{};
	};
	inline void draw_header()
	{
		get()->draw_rect({ 0.01f, g_drawbase }, { 0.13f, 0.0694f }, { 0, 186, 255, 255 });
		g_drawbase += 0.0694f;
	}
	inline void draw_subtitle(submenu sub)
	{
		get()->draw_rect({ 0.01f, g_drawbase }, { 0.13f, 0.02f }, { 0, 0, 0, 190 });
		get()->draw_text(arialbd_23, sub.m_name.data(), { 0.01f + 0.001f, g_drawbase + (0.02f / 2.f) }, { 255, 255, 255, 255 });
		std::string pos{ std::format("{}/{}", sub.m_current + 1, sub.m_options.size()) };
		draw_text_right(arialbd_23, pos.data(), { 0.01f + 0.13f - 0.001f, g_drawbase + (0.02f / 2.f) }, { 255, 255, 255, 255 });
		g_drawbase += 0.02f;
	}
	inline void draw_footer()
	{
		get()->draw_rect({ 0.01f, g_drawbase }, { 0.13f, 0.02f }, { 0, 0, 0, 190 });
		std::string candidate{ BASE_CANDIDATE " ["  BASE_BRANCH "]" };
		get()->draw_text(arial_22, candidate.data(), { 0.01f + 0.001f, g_drawbase + (0.02f / 2.f) }, { 255, 255, 255, 255 });
		draw_text_right(arial_22, "v0.01", { 0.01f + 0.13f - 0.001f, g_drawbase + (0.02f / 2.f) }, { 255, 255, 255, 255 });
		g_drawbase += 0.02f;
	}
	class simple_timer
	{
	public:
		void start(uint64_t ticks)
		{
			if (m_tick)
			{
				m_ready_at = GetTickCount64() + ticks;
				m_tick = false;
			}
		}
		bool is_ready()
		{
			return GetTickCount64() > m_ready_at;
		}
		void reset()
		{
			m_tick = true;
		}
	public:
		u64 m_ready_at{};
		bool m_tick{};
	};
	inline void on_press(int key, size_t delay, fptr<void()> callback)
	{
		if (GetForegroundWindow() == pointers::g_hwnd)
		{
			static simple_timer t{}; // Update the input states

			bool pressed = input::g_input.m_keyboard.key_pressed(key);
			bool down = input::g_input.m_keyboard.key_down(key);

			if (pressed)
			{
				if (callback)
				{
					callback();
				}
			}
			else if (down)
			{
				t.start(delay * 100);
				if (t.is_ready())
				{
					if (callback)
					{
						callback();
					}
				}
			}
			else
			{
				t.reset();
			}
		}
	}
	inline void handle_input()
	{
		on_press(VK_RETURN, 20, [] {
			g_submenus.top().action(0);
		});
		on_press(VK_BACK, 20, [] {
			pop();
		});
		on_press(VK_UP, 10, [] {
			g_submenus.top().action(1);
		});
		on_press(VK_DOWN, 10, [] {
			g_submenus.top().action(2);
		});
		input::g_input.update();
	}
	inline void draw_menu()
	{
		rage::run_as_thread("main_persistent"_j, [] {
			GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT("commonmenu", FALSE);
			auto dictionary{ pointers::g_TxdStore->Get("commonmenu") };
			if (dictionary)
			{
				for (auto& txd : dictionary->m_Entries)
				{
					//LOG_TO_STREAM("Name: " << std::string(txd->m_Name));
				}
			}
		});
		handle_input();
		g_drawbase = 0.01f;
		draw_header();
		draw_subtitle(g_submenus.top());
		g_submenus.top().handle([] {
			g_submenus.top().add(base_option("No options!"));
		});
		draw_footer();
	}
}