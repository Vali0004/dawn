#pragma once
#include "pch/framework.h"

namespace dwn
{
	template <typename R, typename T, typename ...A>
	class virtual_pure
	{
	public:
		using ReturnT = R;
		using InstanceT = T;
		template <typename C>
		constexpr bool is_class() const
		{
			return std::is_base_of_v<T, std::remove_pointer_t<C>>;
		}
		bool is_valid()
		{
			return m_function == nullptr ? true : false;
		}
		template <typename I>
		auto set(mfptr<R, I, A...> fn)
		{
			auto cached_function{ m_function };
			m_function = *reinterpret_cast<decltype(m_function)*>(&fn);
			return cached_function;
		}
		template <typename I>
		R call(I* instance, A&&... args)
		{
			if (m_function)
			{
				return (instance->*m_function)(std::forward<A>(args)...);
			}
			if constexpr (std::is_same_v<R, void>)
			{
				return;
			}
			else
			{
				return R{};
			}
		}
	public:
		mfptr<R, T, A...> m_function{};
	};
}