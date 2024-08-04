#pragma once
#include "pch/pch.h"
#include "abstract.h"
#include "renderer/renderer.h"

namespace dwn::commands
{
	using namespace renderer;
	class single_command : public cmd_data<single_command, single_command, single_command>
	{
	public:
		TYPE(single_command);
		single_command(const std::string& name, fptr<void(single_command*)> callback, fptr<void(single_command*)> tickCallback = nullptr, bool doesOnceActAsInit = false) :
			cmd_data(name, doesOnceActAsInit),
			m_callback(callback),
			m_tick_callback(tickCallback)
		{
			get_once<std::remove_pointer_t<decltype(this)>>()->set(&single_command::once);
			if (tickCallback)
			{
				get_tick<std::remove_pointer_t<decltype(this)>>()->set(&single_command::tick);
			}
			get_render<std::remove_pointer_t<decltype(this)>>()->set(&single_command::render);
		}
		void once()
		{
			if (m_callback)
			{
				m_callback(this);
			}
		}
		void tick()
		{
			if (m_tick_callback)
			{
				m_tick_callback(this);
			}
		}
		void render(bool selected)
		{
			get()->draw_rect({ 0.01f, g_drawbase }, { 0.13f, 0.02f }, selected ? directx::Color{ 255, 255, 255, 255 } : directx::Color{ 0, 0, 0, 130 });
			get()->draw_text(arial_22, m_name.data(), { 0.01f + 0.001f, g_drawbase + (0.02f / 2.f) }, selected ? directx::Color{ 0, 0, 0, 255 } : directx::Color{ 255, 255, 255, 255 });
			g_drawbase += 0.02f;
		}
	private:
		fptr<void(single_command*)> m_callback{};
		fptr<void(single_command*)> m_tick_callback{};
	};
	class group_command : public cmd_data<group_command, cmd_functions, group_command>
	{
	public:
		TYPE(group_command);
		group_command(const std::string& name, fptr<void(group_command*)> callback = nullptr, bool doesOnceActAsInit = false) :
			cmd_data(name, doesOnceActAsInit),
			m_callback(callback)
		{
			get_once<std::remove_pointer_t<decltype(this)>>()->set(&group_command::once);
			get_render<std::remove_pointer_t<decltype(this)>>()->set(&group_command::render);
		}
		void once()
		{
			if (m_callback)
			{
				m_callback(this);
			}
		}
		void render(bool selected)
		{
			get()->draw_rect({ 0.01f, g_drawbase }, { 0.13f, 0.02f }, selected ? directx::Color{ 255, 255, 255, 255 } : directx::Color{ 0, 0, 0, 130 });
			get()->draw_text(arial_22, m_name.data(), { 0.01f + 0.001f, g_drawbase + (0.02f / 2.f) }, selected ? directx::Color{ 0, 0, 0, 255 } : directx::Color{ 255, 255, 255, 255 });
			get()->draw_text(iconfont_24, "r", { 0.01f + (0.131f - 0.0005f), g_drawbase + (0.02f / 2.f) + 0.006f }, selected ? directx::Color{ 0, 0, 0, 255 } : directx::Color{ 255, 255, 255, 255 }, eJustify::Right);
			g_drawbase += 0.02f;
		}
	private:
		fptr<void(group_command*)> m_callback{};
	};
}