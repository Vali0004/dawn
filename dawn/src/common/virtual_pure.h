#pragma once
#include "pch/framework.h"

namespace dwn
{
	template <typename R, typename T, typename ...A>
	class virtual_pure
	{
	public:
		template <typename C>
		constexpr bool is_class() const
		{
			return C::Type == m_type;
		}
		bool is_valid()
		{
			return m_function == nullptr ? true : false;
		}
		template <typename I>
		auto set(mfptr<R, I, A...> fn)
		{
			auto cached_function{ m_function };
			m_type = I::Type;
			m_function = *reinterpret_cast<decltype(m_function)*>(&fn);
			return cached_function;
		}
		template <typename I>
		R call(I* instance, A&&... args)
		{
			if (m_function)
			{
				return (instance->*reinterpret_cast<mfptr<R, I, A...>>(m_function))(std::forward<A>(args)...);
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
		u32 m_type{};
		mfptr<R, T, A...> m_function{};
	};
}