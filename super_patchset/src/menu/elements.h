#pragma once
#include "pch/pch.h"
#include "menu/fonts.h"

namespace cs::renderer
{
	inline std::vector<class callbacks> g_callbacks{};
	class callbacks
	{
	public:
		callbacks(bool active, std::function<void(bool&)> fn) :
			m_active(active),
			m_fn(fn)
		{}
		callbacks(std::function<void(bool&)> fn) :
			m_active(true),
			m_fn(fn)
		{}
	public:
		void invoke()
		{
			likely if (m_fn)
			{
				if (m_active)
				{
					m_fn(m_active);
				}
			}
		}

		static void run()
		{
			for (auto& c : g_callbacks)
				c.invoke();
		}

		operator bool() const
		{
			return m_active;
		}
	private:
		bool m_active;
		std::function<void(bool&)> m_fn;
	};

	namespace menu
	{
		inline void dummy(const ImVec2& size) { ImGui::Dummy(size); }
		inline void push_width(const float& size) { ImGui::PushItemWidth(size); }
		inline void same_line(float offset = 0.f) { ImGui::SameLine(offset); }
		inline void set_window(const ImVec4& data, ImGuiCond condition = NULL)
		{
			ImGui::SetWindowPos({ data.x, data.y }, condition);
			ImGui::SetWindowSize({ data.z, data.w }, condition);
		}
		inline void set_next_window(const ImVec4& data, ImGuiCond condition = NULL, const ImVec2& pivot = {})
		{
			ImGui::SetNextWindowPos({ data.x, data.y }, condition, pivot);
			ImGui::SetNextWindowSize({ data.z, data.w }, condition);
		}
		inline ImVec2 cursor_pos() { return ImGui::GetCursorPos(); }
		inline ImGuiWindow* get_window() { return ImGui::GetCurrentWindow(); }
		inline const ImVec4& get_window_data(ImGuiWindow* window)
		{
			return { window->Pos.x, window->Pos.y, window->Size.x, window->Size.y };
		}
		inline const ImVec4& get_window_data() { return get_window_data(get_window()); }
		inline void separator() { ImGui::Separator(); }
		inline void open_popup(const std::string& id) { ImGui::OpenPopup(id.c_str()); }
		inline void close_current_popup() { ImGui::CloseCurrentPopup(); }
		inline void set_item_default_focus() { ImGui::SetItemDefaultFocus(); }
		inline void set_keyboard_focus_here() { ImGui::SetKeyboardFocusHere(); }
		inline void font(const std::string& id, std::function<void()> cb = {})
		{
			auto font{ fonts::g_font_mgr.get_font(id) };
			ImGui::PushFont(font->imfont());
			if (cb) cb();
			ImGui::PopFont();
		}
		template <typename ...t>
		inline void text(const std::string& fmt, t... args)
		{
			std::string str{ std::vformat(fmt, std::make_format_args(args...)) };
			ImGui::Text(str.c_str());
		}
		template <typename ...t>
		inline void text_unformatted(const std::string& fmt, t... args)
		{
			std::string str{ std::vformat(fmt, std::make_format_args(args...)) };
			ImGui::TextUnformatted(str.c_str());
		}
		inline void window(const std::string& title, bool& value, std::function<void()> cb = {}, ImGuiWindowFlags flags = NULL)
		{
			if (ImGui::Begin(title.c_str(), &value, flags))
			{
				if (cb) cb();
				ImGui::End();
			}
		}
		inline void popup_modal(const std::string& title, std::function<void()> cb = {}, ImGuiWindowFlags flags = NULL)
		{
			if (ImGui::BeginPopupModal(title.c_str(), NULL, flags))
			{
				if (cb) cb();
				ImGui::EndPopup();
			}
		}
		inline void primitive_window(const std::string& title, std::function<void()> cb = {}, ImGuiWindowFlags flags = NULL)
		{
			static bool t{ true };
			window(title, t, cb, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
		}
		inline void list_box(const std::string& title, ImVec2 size = {}, std::function<void()> cb = {})
		{
			if (ImGui::BeginListBox(title.c_str(), size))
			{
				if (cb) cb();
				ImGui::EndListBox();
			}
		}
		inline void child(const std::string& label, ImVec2 size = {}, bool border = false, std::function<void()> cb = {}, ImGuiWindowFlags flags = NULL)
		{
			if (ImGui::BeginChild(label.c_str(), size, border, flags))
			{
				if (cb) cb();
				ImGui::EndChild();
			}
		}
		inline void primitive_child(const std::string& id, ImVec2 size = {}, std::function<void()> cb = {})
		{
			child("##" + id, size, false, cb, ImGuiWindowFlags_NoBackground);
		}
		inline void tab_bar(const std::string& id, std::function<void()> cb = {})
		{
			if (ImGui::BeginTabBar(id.c_str()))
			{
				if (cb) cb();
				ImGui::EndTabBar();
			}
		}
		inline void tab_item(const std::string& name, std::function<void()> cb = {})
		{
			if (ImGui::BeginTabItem(name.c_str()))
			{
				primitive_child(name, {}, cb);
				ImGui::EndTabItem();
			}
		}
		inline void tab_button(const std::string& name, std::function<void()> cb = {})
		{
			if (ImGui::TabItemButton(name.c_str()))
			{
				if (cb) cb();
			}
		}
		inline void button(const std::string& label, std::function<void()> cb = {}, ImVec2 size = {}, bool continueLine = false)
		{
			if (ImGui::Button(label.c_str(), size))
			{
				if (cb) cb();
			}

			if (continueLine)
				same_line();
		}
		inline void checkbox(const std::string& label, bool& value, std::function<void()> cb = {})
		{
			if (ImGui::Checkbox(label.c_str(), &value))
			{
				if (cb) cb();
			}
		}
		inline void selectable(const std::string& label, bool selected, std::function<void()> cb = {})
		{
			if (ImGui::Selectable(label.c_str(), selected))
			{
				if (cb) cb();
			}
		}
		inline void popup(const std::string& id, std::function<void()> cb = {})
		{
			if (ImGui::BeginPopup(id.c_str()))
			{
				if (cb) cb();
				ImGui::EndPopup();
			}
		}
		inline void callback_close_popup(const std::string& windowName, std::vector<std::string> strings, float buttonOffset = 410.f)
		{
			g_callbacks.push_back(callbacks([&](bool& active) {
				ImGui::OpenPopup(windowName.c_str());
				ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
				ImVec2 size{ 470.f, 275.f };
				set_next_window({ center.x - (size.x * .5f), center.y - (size.y * .5f), size.x, size.y }, ImGuiCond_Always);
				popup_modal(windowName, [&] {
					if (GetAsyncKeyState(VK_ESCAPE))
					{
						close_current_popup();
						active = false;
					}
					for (auto& s : strings)
					{
						text(s);
					}
					same_line(buttonOffset);
					set_item_default_focus();
					button("Close", [&active] {
						close_current_popup();
						active = false;
					});
				});
			}));
		}
		inline void callback_yesno_popup(const std::string& windowName, std::vector<std::string> strings, std::function<void()> cb = {}, float buttonOffset = 410.f)
		{
			g_callbacks.push_back(callbacks([&](bool& active) {
				ImGui::OpenPopup(windowName.c_str());
				ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
				ImVec2 size{ 470.f, 275.f };
				set_next_window({ center.x - (size.x * .5f), center.y - (size.y * .5f), size.x, size.y }, ImGuiCond_Always);
				popup_modal(windowName, [&] {
					if (GetAsyncKeyState(VK_ESCAPE))
					{
						close_current_popup();
						active = false;
					}
					for (auto& s : strings)
					{
						text(s);
					}
					same_line(buttonOffset);
					set_item_default_focus();
					button("Yes", [&active, &cb] {
						if (cb)
						{
							cb();
						}

						close_current_popup();
						active = false;
					});
					same_line();
					button("No", [&active] {
						close_current_popup();
						active = false;
					});
				});
			}));
		}
	}
}