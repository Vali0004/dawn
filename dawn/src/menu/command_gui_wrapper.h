#pragma once
#include "pch/pch.h"
#include "common/virtual_pure.h"
#include "hooking/hooks.h"
#include "renderer/input.h"
#include "command_engine/manager.h"
#include "menu/timer.h"

namespace dwn::commands
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

	class submenu;
	inline submenu* g_submenu;
	extern void push(submenu sub);
	extern void pop();
	class submenu : public sub_manager
	{
	public:
		submenu(const std::string_view& name) : sub_manager(name)
		{}
		submenu(const sub_manager& cmd) : sub_manager(cmd)
		{}
		submenu() : sub_manager(std::string_view())
		{}
		void action(int type)
		{
			switch (type)
			{
				case 0:
				{
					if (get_num_commands())
					{
						auto cmd{ get_cmd(m_current) };
						if (!cmd->is_once_of<cmd_functions>() && cmd->is_enabled())
						{
							if (cmd->is_once_of<single_command>())
							{
								if (!cmd->m_does_once_act_as_init)
								{
									cmd->get_once()->call(cmd);
								}
							}
							else
							{
								cmd->get_once()->call(cmd);
								// This actually isn't type punting, or anything stupid. We do this properly : )
								submenu* sub = reinterpret_cast<submenu*>(get_sub(cmd->get_id()));
								if (sub && sub->is_enabled())
								{
									g_submenu = sub;
									push(*g_submenu);
								}
							}
						}
					}
				} break;
				case 1:
				{
					if (get_order_id(m_current) > 0)
						m_current = get_cmd_from_order_id(get_order_id(m_current) - 1);
					else
						m_current = get_cmd_from_order_id(get_num_usable_commands() - 1);
				} break;
				case 2:
				{
					if (get_order_id(m_current) < get_num_usable_commands() - 1)
						m_current = get_cmd_from_order_id(get_order_id(m_current) + 1);
					else
						m_current = get_cmd_from_order_id(0);
				} break;
			}
		}
		void handle()
		{
			for (size_t i{}; i != get_num_commands(); ++i)
			{
				auto cmd{ get_cmd(get_cmd_from_order_id(i)) };
				if (!cmd->m_does_once_act_as_init)
				{
					if (cmd->is_once_of<group_command>())
					{
						if (get_sub(cmd->get_id())->is_enabled())
						{
							cmd->get_render()->call(cmd, !cmd->get_id().compare(m_current.data()));
						}
					}
					else if (cmd->is_enabled())
					{
						cmd->get_render()->call(cmd, !cmd->get_id().compare(m_current.data()));
					}
				}
			}
			if (get_order_id(m_current) >= get_num_usable_commands())
			{
				m_current = get_cmd_from_order_id(0);
			}
		}
		std::string m_current{};
	};

	inline std::stack<submenu> g_submenus{};

	inline void push(submenu sub)
	{
		g_submenu = &sub;
		g_submenus.push(*g_submenu);
	}

	inline void push(submenu* sub)
	{
		g_submenu = sub;
		g_submenus.push(*g_submenu);
	}

	inline void push(sub_manager sub)
	{
		push(submenu(sub));
	}

	inline void push(sub_manager* sub)
	{
		push(submenu(*sub));
	}

	inline void pop()
	{
		if (g_submenus.size() > 1 && g_submenus.top().get_name() != "home")
		{
			g_submenus.pop();
		}
		else
		{
			gui::flip_bit(true);
		}
	}

	inline void draw_header()
	{
		get()->draw_rect({ g_pos_x, g_drawbase }, { 0.13f, 0.0694f }, { 0, 186, 255, 255 });
		g_drawbase += 0.0694f;
	}

	inline void draw_subtitle(submenu sub)
	{
		get()->draw_rect({ g_pos_x, g_drawbase }, { 0.13f, 0.02f }, { 0, 0, 0, 190 });
		get()->draw_text(arialbd_23, sub.get_name().data(), { g_pos_x + 0.001f, g_drawbase + (0.02f / 2.f) }, { 255, 255, 255, 255 });
		std::string pos{ std::format("{}/{}", sub.get_order_id(sub.m_current) + 1, sub.get_num_usable_commands()) };
		get()->draw_text(arialbd_23, pos.data(), { g_pos_x + 0.13f - 0.001f, g_drawbase + (0.02f / 2.f) }, { 255, 255, 255, 255 }, eJustify::Right);
		g_drawbase += 0.02f;
	}

	inline void draw_footer()
	{
		get()->draw_rect({ g_pos_x, g_drawbase }, { 0.13f, 0.02f }, { 0, 0, 0, 190 });
		std::string candidate{ BASE_CANDIDATE " ["  BASE_BRANCH "]" };
		get()->draw_text(arial_22, candidate.data(), { g_pos_x + 0.001f, g_drawbase + (0.02f / 2.f) }, { 255, 255, 255, 255 });
		get()->draw_text(arial_22, "v0.01", { g_pos_x + 0.13f - 0.001f, g_drawbase + (0.02f / 2.f) }, { 255, 255, 255, 255 }, eJustify::Right);
		g_drawbase += 0.02f;
	}

	inline void on_press(int key, size_t delay, fptr<void()> callback)
	{
		if (GetForegroundWindow() == pointers::g_hwnd)
		{
			static simple_timer t{};

			bool pressed{ input::g_input.m_keyboard.key_pressed(key) ? true : false };
			bool down{ input::g_input.m_keyboard.key_down(key) ? true : false };

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
		handle_input();
		g_drawbase = 0.01f;
		draw_header();
		draw_subtitle(g_submenus.top());
		g_submenus.top().handle();
		draw_footer();
	}

	inline void create_fonts()
	{
		std::fs::path font_path{ std::getenv("appdata") };
		font_path /= BASE_NAME;
		font_path /= "Fonts";
		std::string str{ (font_path / "icon_font.ttf").string() };
		iconfont_22 = directx::load_font(str.data(), 22.f, get()->m_device);
		iconfont_24 = directx::load_font(str.data(), 24.f, get()->m_device);
		arial_22 = directx::load_font("C:\\Windows\\Fonts\\arial.ttf", 22.f, get()->m_device);
		arial_23 = directx::load_font("C:\\Windows\\Fonts\\arial.ttf", 23.f, get()->m_device);
		arialbd_22 = directx::load_font("C:\\Windows\\Fonts\\arialbd.ttf", 22.f, get()->m_device);
		arialbd_23 = directx::load_font("C:\\Windows\\Fonts\\arialbd.ttf", 23.f, get()->m_device);
		ariali_22 = directx::load_font("C:\\Windows\\Fonts\\ariali.ttf", 22.f, get()->m_device);
		ariali_23 = directx::load_font("C:\\Windows\\Fonts\\ariali.ttf", 23.f, get()->m_device);
		arialbi_22 = directx::load_font("C:\\Windows\\Fonts\\arialbi.ttf", 22.f, get()->m_device);
		arialbi_23 = directx::load_font("C:\\Windows\\Fonts\\arialbi.ttf", 23.f, get()->m_device);
	}

	inline void destroy_fonts()
	{
		directx::destroy_font(iconfont_22);
		directx::destroy_font(iconfont_24);
		directx::destroy_font(arial_22);
		directx::destroy_font(arial_23);
		directx::destroy_font(arialbd_22);
		directx::destroy_font(arialbd_23);
		directx::destroy_font(ariali_22);
		directx::destroy_font(ariali_23);
		directx::destroy_font(arialbi_22);
		directx::destroy_font(arialbi_23);
	}
}