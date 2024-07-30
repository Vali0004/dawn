#pragma once
#include "pch/framework.h"
#include "memory/pointers.h"
#include "commands/invoker.h"
using KeyboardFunctionT = fptr<void(ul32 key, u16 repeats, u8 scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)>;
using PresentCallbackT = fptr<void(void*)>;

namespace dwn::shv
{
	namespace dx
	{
		// I don't want the old super complex code, esp if I can just slap it into a dxthread and call it a day
		struct texture
		{
			int m_time{};
			bool m_enabled{ true };
		};
		inline std::unordered_map<int, texture> g_textures{};
		inline void update_textures()
		{
			for (auto& tex : g_textures)
			{

			}
		}
	}
	enum eGameVersion : int
	{
		VER_CURRENT = 74,
		VER_SIZE,
		VER_UNK = -1
	};
	inline u64 g_hash{};
	inline std::vector<u64> g_args{};
	inline std::unordered_map<KeyboardFunctionT, KeyboardFunctionT> g_keyboard_functions{};
	inline std::unordered_map<PresentCallbackT, PresentCallbackT> g_present_callbacks{};

	inline void scriptRegister(HMODULE module, void(*LP_SCRIPT_MAIN)())
	{

	}

	inline void scriptRegisterAdditionalThread(HMODULE module, void(*LP_SCRIPT_MAIN)())
	{

	}

	inline void scriptUnregister(HMODULE module)
	{

	}

	inline void scriptUnregisterDepricated(void(*LP_SCRIPT_MAIN)())
	{

	}

	inline void nativeInit(u64 hash)
	{
		g_hash = hash;
	}

	inline void nativePush64(u64 val)
	{
		g_args.push_back(val);
	}

	inline u64* nativeCall()
	{
		u64 paramCount{ g_args.size() };
		scrValue result{};
		auto params{ std::make_unique<scrValue[]>(paramCount) };
		u64 argCount{};
		for (auto& arg : g_args)
		{
			SetArg(params.get(), argCount, arg);
		}

		scrThread::Info inf{ &result, paramCount, params.get() };
		scrCmd cmd{ dwn::pointers::g_sCommandHash->Lookup(CorrectHash(g_hash)) };
		cmd(&inf);
		inf.CopyReferencedParametersOut();

		return reinterpret_cast<u64*>(&result);
	}
	inline u64* getGlobalPtr(int index)
	{
		rage::scrValue** globalBlocks{ &(*pointers::g_smGlobals)[0] };
		rage::scrValue* global{ &globalBlocks[index >> rage::scrProgram::MAX_GLOBAL_BLOCKS_SHIFT][index & rage::scrProgram::GLOBAL_SIZE_MASK] };
		return reinterpret_cast<u64*>(global);
	}

	inline int createTexture(const char* fileName)
	{
		return 0;
	}

	inline void drawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a)
	{
		return;
	}

	inline void scriptWait(DWORD waitTime)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
	}

	inline void keyboardHandlerRegister(KeyboardFunctionT function)
	{
		g_keyboard_functions.insert(std::make_pair(function, function));
	}

	inline void keyboardHandlerUnregister(KeyboardFunctionT function)
	{
		g_keyboard_functions.erase(function);
	}

	inline eGameVersion getGameVersion()
	{
		return VER_CURRENT;
	}

	inline u8* getScriptHandleBaseAddress(int handle)
	{
		return reinterpret_cast<u8*>(pointers::g_fwScriptGuidGetBaseFromGuid(handle));
	}

	inline void presentCallbackRegister(PresentCallbackT cb)
	{
		g_present_callbacks.insert(std::make_pair(cb, cb));
	}

	inline void presentCallbackUnregister(PresentCallbackT cb)
	{
		g_present_callbacks.erase(cb);
	}

	// TODO:
	inline int worldGetAllVehicles(int* arr, int arrSize)
	{
		return 0;
	}

	inline int worldGetAllPeds(int* arr, int arrSize)
	{
		return 0;
	}

	inline int worldGetAllObjects(int* arr, int arrSize)
	{
		return 0;
	}

	inline int worldGetAllPickups(int* arr, int arrSize)
	{
		return 0;
	}
}