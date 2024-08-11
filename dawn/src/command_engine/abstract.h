#pragma once
#include "pch/pch.h"
#include "common/virtual_pure.h"
#include "nlohmann/json.h"
#include "common/word_tokenizer.h"
#define TYPE(c) enum { Type = #c##_j }

namespace dwn::commands
{
	class cmd_container
	{
	public:
		cmd_container(const std::string_view& name) :
			m_name(name)
		{}
		cmd_container() :
			m_name("")
		{}

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

		std::string& get_name()
		{
			return m_name;
		}
	private:
		std::string m_name{}; // Name to reference a value by.
		void* m_ptr{};
		bool m_reference{};
	};

	class cmd_functions
	{
	public:
		TYPE(cmd_functions);
		void once()
		{}
		void tick()
		{}
		void render(bool selected)
		{}
	};

	template <typename T1 = cmd_functions, typename T2 = cmd_functions, typename T3 = cmd_functions>
	class cmd_data
	{
	public:
		cmd_data(const std::string& name, bool doesOnceActAsInit = false) :
			m_name(name),
			m_id(name),
			m_enabled(true),
			m_does_once_act_as_init(doesOnceActAsInit)
		{
			m_once = new std::remove_pointer_t<decltype(m_once)>();
			m_tick = new std::remove_pointer_t<decltype(m_tick)>();
			m_render = new std::remove_pointer_t<decltype(m_render)>();
			m_class_name = T1::Type;
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
		void render(bool selected) {}
		template <typename T = T1, typename ...A>
		auto get_once() { return reinterpret_cast<virtual_pure<void, T, A...>*>(m_once); }
		template <typename T = T1, typename ...A>
		auto get_tick() { return reinterpret_cast<virtual_pure<void, T, A...>*>(m_tick); }
		template <typename T = T1, typename ...A>
		auto get_render() { return reinterpret_cast<virtual_pure<void, T, bool, A...>*>(m_render); }

		template <typename C>
		bool is_once_of() const { return (*reinterpret_cast<virtual_pure<void, T1>*>(m_once)).template is_class<C>(); }
		template <typename C>
		bool is_tick_of() const { return (*reinterpret_cast<virtual_pure<void, T1>*>(m_tick)).template is_class<C>(); }
		template <typename C>
		bool is_render_of() const { return (*reinterpret_cast<virtual_pure<void, T1, bool>*>(m_render)).template is_class<C>(); }

		cmd_container* get_container(const std::string_view& name)
		{
			for (auto& container : m_containers)
			{
				if (!container->get_name().compare(name.data()))
				{
					return container;
				}
			}

			return nullptr;
		}

		cmd_container* add_container(const std::string_view& name)
		{
			cmd_container* container{ new cmd_container(name) };
			m_containers.push_back(container);
			return container;
		}

		template <typename T>
		cmd_container* add_container_preallocate(const std::string_view& name, T value)
		{
			cmd_container* container{ new cmd_container(name) };
			container->allocate<T>(value);
			m_containers.push_back(container);
			return container;
		}

		const std::string& get_id()
		{
			return m_id;
		}

		std::string& get_name()
		{
			return m_name;
		}

		bool is_enabled()
		{
			return m_enabled;
		}

		nlohmann::json to_json()
		{
			nlohmann::json json{
				{ "id", m_id }, // If a user changes this, we will just override it. : )
				{ "label", m_name },
				{ "enabled", m_enabled },
				{ "order_id", number_to_word(m_order_id, true) },
				{ "values", nullptr },
			};

			// Containers is TODO
			json["values"] = nullptr;

			return json;
		}

		void from_json(nlohmann::json& json)
		{
			json["id"] = m_id;

			if (json["label"].is_string())
			{
				m_name = json["label"].get<std::string>();
			}

			if (json["enabled"].is_boolean())
			{
				m_enabled = json["enabled"].get<bool>();
			}

			if (json["order_id"].is_string())
			{
				m_order_id = word_to_number(json["order_id"].get<std::string>());
			}

			m_containers;
		}

		bool m_enabled{};
		std::string m_name{}, m_id{};
		u64 m_order_id{};
		bool m_does_once_act_as_init{};
		u32 m_class_name{};
		virtual_pure<void, T1>* m_once{};
		virtual_pure<void, T2>* m_tick{};
		virtual_pure<void, T3, bool>* m_render{};
		std::vector<cmd_container*> m_containers{};
	};
}