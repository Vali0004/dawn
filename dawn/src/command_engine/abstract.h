#pragma once
#include "pch/pch.h"
#include "common/virtual_pure.h"

namespace dwn::commands
{
	class cmd_container
	{
	public:
		template <typename T>
		void allocate(T data = T{})
		{
			if constexpr (std::is_pointer_v<T>)
			{
				if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, char*>)
				{
					m_ptr = new std::string(data);
				}
				else
				{
					m_reference = true;
					m_ptr = (void*)data;
				}
			}
			else
			{
				m_ptr = new T(data);
			}
		}
		void deallocate()
		{
			if (!m_reference)
			{
				delete m_ptr;
			}
		}
		template <typename T>
		T& get()
		{
			if constexpr (std::is_pointer_v<T>)
			{
				return reinterpret_cast<T&>(m_ptr);
			}
			return *reinterpret_cast<T*>(m_ptr);
		}
		void* m_ptr{};
		bool m_reference{};
	};
	class cmd_functions
	{
	public:
		void once()
		{}
		void tick()
		{}
		void render()
		{}
	};
	template <typename T1 = cmd_functions, typename T2 = cmd_functions, typename T3 = cmd_functions>
	class cmd_data
	{
	public:
		cmd_data(const std::string& name) : m_name(name)
		{
			m_once = new std::remove_pointer_t<decltype(m_once)>();
			m_tick = new std::remove_pointer_t<decltype(m_tick)>();
			m_render = new std::remove_pointer_t<decltype(m_render)>();
			if constexpr (std::is_same_v<T1, cmd_functions>)
				m_once->set<cmd_functions>(&cmd_functions::once);
			if constexpr (std::is_same_v<T2, cmd_functions>)
				m_tick->set<cmd_functions>(&cmd_functions::tick);
			if constexpr (std::is_same_v<T3, cmd_functions>)
				m_render->set<cmd_functions>(&cmd_functions::render);
		}
		~cmd_data()
		{
			delete m_once;
			delete m_tick;
			delete m_render;
		}
		// This is to satisfy our templating bullshit
		void once() {}
		void tick() {}
		void render() {}
		template <typename T = T1, typename ...A>
		auto get_once() { return reinterpret_cast<commands::virtual_pure<void, T, A...>*>(m_once); }
		template <typename T = T1, typename ...A>
		auto get_tick() { return reinterpret_cast<commands::virtual_pure<void, T, A...>*>(m_tick); }
		template <typename T = T1, typename ...A>
		auto get_render() { return reinterpret_cast<commands::virtual_pure<void, T, A...>*>(m_render); }

		template <typename C>
		bool is_once_of() const { return m_once.template is_class<C>(); }
		template <typename C>
		bool is_tick_of() const { return m_tick.template is_class<C>(); }
		template <typename C>
		bool is_render_of() const { return m_render.template is_class<C>(); }

		std::string m_name{};
		virtual_pure<void, T1>* m_once;
		virtual_pure<void, T2>* m_tick;
		virtual_pure<void, T3>* m_render;
		std::vector<cmd_container> m_containers{};
	};
}