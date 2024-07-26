#pragma once
#include "hooks_base.h"

namespace dwn::hooking
{
	bool g_spoofValues{ true };
	inline etc::hook<pointers::types::CExtraContentManagerGetCRCT>* g_CExtraContentManagerGetCRC{};
	inline u32 CExtraContentManagerGetCRC(CExtraContentManager* _This, u32 initValue)
	{
		if (g_spoofValues)
		{
			if ((u32)(*pointers::g_CExtraContentManagersmInstance) != (*pointers::g_CNetworkAssetVerifiersmInstance)->m_CRC.Get())
			{
				LOG_TO_STREAM("Cloud data hash mismatch.");
				LOG_TO_STREAM("Init value: " << initValue);
			}
			return g_CExtraContentManagerGetCRC->original()(_This, 0);
		}
		return g_CExtraContentManagerGetCRC->original()(_This, initValue);
	}

	inline u32 g_keyId{};

	inline etc::hook<pointers::types::AESisTransformITKeyT>* g_AESisTransformITKey{};
	inline bool AESisTransformITKey(u32 keyId)
	{
		g_keyId = keyId;
		return g_AESisTransformITKey->original()(keyId);
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

	inline etc::hook<pointers::types::fiPackfileReInitT>* g_fiPackfileReInit{};
	inline bool fiPackfileReInit(rage::fiPackfile* _This, const char* filename, bool readNameHeap, rage::fiPackEntry* headerData)
	{
		bool result = g_fiPackfileReInit->original()(_This, filename, readNameHeap, headerData);
		if (result)
		{
			for (uint32_t i = 0; i < _This->m_EntryCount; ++i)
			{
				fiPackEntry* entry = &_This->m_Entries[i];
				if (entry->IsResource() && entry->m_NameOffset > 0 && entry->u.file.m_Encrypted)
				{
					if (g_keyId == 0x4E45504F) // OPEN
					{
						entry->u.file.m_Encrypted = 0xFEFFFFF;
					}
				}
			}

			if (g_keyId == 0x4E45504F) // OPEN
			{
				_This->m_KeyId = 0xFEFFFFF;
			}
		}
		return result;
	}
}