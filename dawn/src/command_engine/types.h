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
			get_once<std::remove_pointer_t<decltype(this)>>()->set(&single_command::run);
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
	class int_command : public cmd_data<int_command>
	{
	public:
		int_command(const std::string& name, fptr<void(int)> callback) :
			cmd_data(name),
			m_callback(callback)
		{
			get_once<std::remove_pointer_t<decltype(this)>, int>()->set(&int_command::run);
		}
		void run(int v)
		{
			if (m_callback)
			{
				m_callback(v);
			}
		}
	private:
		fptr<void(int)> m_callback{};
	};
	class float_command : public cmd_data<int_command>
	{
	public:
		float_command(const std::string& name, fptr<void(float)> callback) :
			cmd_data(name),
			m_callback(callback)
		{
			get_once<std::remove_pointer_t<decltype(this)>, float>()->set(&float_command::run);
		}
		void run(float v)
		{
			if (m_callback)
			{
				m_callback(v);
			}
		}
	private:
		fptr<void(float)> m_callback{};
	};
}