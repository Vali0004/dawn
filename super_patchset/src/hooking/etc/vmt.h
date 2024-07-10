#pragma once
#include "pch/framework.h"
#include "hooking/etc/base.h"

namespace cs::hooking::etc
{
	class hook_vtbl
	{
	public:
		hook_vtbl(const std::string& id, void* vtable, u64 num_funcs)
			: m_vtable(reinterpret_cast<u64**>(vtable)),
			m_original(*m_vtable),
			m_num_funcs(num_funcs)
		{
			m_vtable_new = std::make_unique<u64[]>(m_num_funcs);
			std::copy_n(m_original, m_num_funcs, m_vtable_new.get());
			g_vtbl_hooks.push_back(this);
		}
		~hook_vtbl()
		{
			remove();
		}

		template <typename T>
		T original(u64 index)
		{
			return reinterpret_cast<T>(m_original[index]);
		}

		template <typename T>
		void set_func(u64 index, T function)
		{
			m_vtable_new[index] = (u64)function;
		}
		void restore_func(u64 index)
		{
			m_vtable_new[index] = m_original[index];
		}

		void create()
		{
			*m_vtable = m_vtable_new.get();
		}
		void remove()
		{
			*m_vtable = m_original;
		}

	private:
		u64** m_vtable{};
		std::unique_ptr<u64[]> m_vtable_new{};
		u64* m_original{};
		u64 m_num_funcs{};
	};

	inline void remove_all_vmt()
	{
		if (!g_vtbl_hooks.empty())
		{
			for (auto& h : g_vtbl_hooks)
			{
				h->remove();
			}
			g_vtbl_hooks.clear();
		}
	}
}