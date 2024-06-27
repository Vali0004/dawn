#pragma once
#include "pch/framework.h"
#include "commands/crossmap.h"
#include "common/vectors.h"

// Keep this in the depths of hell where it belongs
namespace rage
{
	inline u64 CorrectHash(u64 hash)
	{
		for (auto& pair : g_crossmap)
		{
			if (pair.first == hash)
			{
				return pair.second;
			}
		}

		return NULL;
	}

	template <typename T>
	inline void SetArg(scrValue* arg, u64& argCount, T value)
	{
		arg[argCount++] = *reinterpret_cast<scrValue*>(&value);
	}

	template <typename T, typename ...Args>
	inline T Invoke(u64 hash, Args&&... args)
	{
		constexpr u64 paramCount{ sizeof...(Args) };
		scrValue result{};
		auto params{ std::make_unique<scrValue[]>(paramCount) };
		u64 argCount{};

		(SetArg(params.get(), argCount, std::forward<Args>(args)), ...);

		scrThread::Info inf{ &result, paramCount, params.get() };
		scrCmd cmd{ cs::pointers::g_sCommandHash->Lookup(CorrectHash(hash)) };
		cmd(&inf);
		inf.CopyReferencedParametersOut();

		if constexpr (!std::is_same_v<T, void>)
		{
			return *reinterpret_cast<T*>(&result);
		}
	}
}