#pragma once
#include "hooks_base.h"

namespace dwn::hooking
{
	inline u32 g_keyId{};

	inline etc::hook<pointers::types::AESisTransformITKeyT>* g_AESisTransformITKey{};
	inline bool AESisTransformITKey(u32 keyId)
	{
		g_keyId = keyId;
		return (keyId & 0xFF00000) == 0xFE00000;
	}

	inline etc::hook<pointers::types::AESTransformITDecryptT>* g_AESTransformITDecrypt{};
	inline void AESTransformITDecrypt(u32 selector, void* data, u32 size)
	{
		if (g_keyId == 0x4E45504F) // OpenIV's enc id
		{
			return;
		}

		return g_AESTransformITDecrypt->original()(selector, data, size);
	}

	inline etc::hook<pointers::types::AESDecryptT>* g_AESDecrypt{};
	inline void AESDecrypt(u32 keyId, u32 selector, void* data, u32 size)
	{
		if (keyId == 0x4E45504F) // OpenIV's enc id
		{
			return;
		}

		return g_AESDecrypt->original()(keyId, selector, data, size);
	}
}