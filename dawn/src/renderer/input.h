#pragma once
#include "pch/pch.h"
#include "memory/pointers.h"

namespace dwn::input
{
	class keyboard
	{
	public:
        void update()
        {
            bool has_focus{ GetForegroundWindow() == pointers::g_hwnd };
            if (!has_focus)
            {
                memset(m_current_state, 0, 256);
            }
            memcpy(m_keys[m_active ^ 1], m_current_state, 256);
            m_active ^= 1;
        }

        u16 key_down(u16 key)
        {
            return m_keys[m_active][key];
        }

        u16 key_up(u16 key)
        {
            return !key_down(key);
        }

        u16 key_changed(u16 key)
        {
            return m_keys[0][key] ^ m_keys[1][key];
        }

        u16 key_pressed(u16 key)
        {
            return key_down(key) & key_changed(key);
        }

        u16 key_released(u16 key)
        {
            return m_keys[m_active ^ 1][key] & key_changed(key);
        }

        void set_key(u16 key, bool active)
        {
            m_current_state[key] = static_cast<u8>(active << 7);
        }

        void flush()
        {
            memset(m_keys[0], 0, 256);
            memset(m_keys[1], 0, 256);
            memset(m_current_state, 0, 256);
        }

        void wndproc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
        {
            switch (umsg)
            {
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                {
                    set_key(static_cast<u16>(wparam & 0xFFFF), true);
                    update();
                } break;

                case WM_KEYUP:
                case WM_SYSKEYUP:
                {
                    set_key(static_cast<u16>(wparam & 0xFFFF), false);
                    update();
                } break;
            }
        }

    private:
        u16 m_active{};
        u8 m_keys[2][256]{};
        u8 m_current_state[256]{};
	};
	class input
	{
	public:
		void update()
		{
			m_keyboard.update();
		}
		keyboard m_keyboard{};
	};
	inline input g_input{};
}