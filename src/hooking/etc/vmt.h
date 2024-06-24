#pragma once
#include "pch/framework.h"

namespace cs::hooking::etc::vtable {
	template <typename T>
	inline void* get(T* vtable, int idx) {
		u64* vtbl = *reinterpret_cast<u64**>(vtable);

		return reinterpret_cast<void*>(vtbl[idx]);
	}
	template <typename OutT, typename VtableT>
	inline void get(OutT output, VtableT* vtable, int idx) {
		u64* vtbl = *reinterpret_cast<u64**>(vtable);

		output = reinterpret_cast<OutT>(vtbl[idx]);
	}
}