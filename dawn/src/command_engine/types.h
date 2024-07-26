#pragma once
#include "pch/pch.h"
#include "abstract.h"

namespace dwn::commands
{
	class single_command : public cmd_data<single_command>
	{
	public:
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
	private:
		fptr<void(single_command*)> m_callback{};
		fptr<void(single_command*)> m_tick_callback{};
	};
}