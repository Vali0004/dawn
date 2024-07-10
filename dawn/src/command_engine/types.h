#pragma once
#include "pch/pch.h"
#include "abstract.h"

namespace dwn::commands
{
	class single_command : public cmd_data<single_command>
	{
	public:
		single_command(const std::string& name, fptr<void()> callback) :
			cmd_data(name),
			m_callback(callback)
		{
			m_once.set(&single_command::run);
		}
		void run()
		{
			if (m_callback)
			{
				m_callback();
			}
		}
	private:
		fptr<void()> m_callback{};
	};
}