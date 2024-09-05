#include "pch.h"
#include "primitive_config.h"
#define EXPORT __declspec(dllexport)

typedef void(*TKeyboardFn)(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);
typedef void(*PresentCallback)(void*);

enum eGameVersion : int
{
	VER_SIZE,
	VER_UNK = -1
};

EXPORT void scriptRegister(HMODULE module, void(*LP_SCRIPT_MAIN)())
{
	auto fn{ (decltype(&scriptRegister))GetProcAddress(g_module, __FUNCTION__) };
	if (fn)
	{
		fn(module, LP_SCRIPT_MAIN);
	}
}
EXPORT void scriptRegisterAdditionalThread(HMODULE module, void(*LP_SCRIPT_MAIN)())
{
	auto fn{ (decltype(&scriptRegisterAdditionalThread))GetProcAddress(g_module, __FUNCTION__) };
	if (fn)
	{
		fn(module, LP_SCRIPT_MAIN);
	}
}
EXPORT void scriptUnregister(HMODULE module)
{
	auto fn{ (void(*)(HMODULE module))GetProcAddress(g_module, __FUNCTION__) };
	if (fn)
	{
		fn(module);
	}
}
EXPORT void scriptUnregister(void(*LP_SCRIPT_MAIN)())
{
	auto fn{ (void(*)(void(*LP_SCRIPT_MAIN)()))GetProcAddress(g_module, __FUNCTION__"Depricated") };
	if (fn)
	{
		fn(LP_SCRIPT_MAIN);
	}
}
EXPORT void nativeInit(uint64_t hash)
{
	auto fn{ (decltype(&nativeInit))GetProcAddress(g_module, __FUNCTION__) };
	if (fn)
	{
		fn(hash);
	}
}
EXPORT void nativePush64(uint64_t val)
{
	auto fn{ (decltype(&nativePush64))GetProcAddress(g_module, __FUNCTION__) };
	if (fn)
	{
		fn(val);
	}
}
EXPORT PUINT64 nativeCall()
{
	auto fn{ (decltype(&nativeCall))GetProcAddress(g_module, __FUNCTION__) };
	return fn ? fn() : NULL;
}
EXPORT PUINT64 getGlobalPtr(int index)
{
	auto fn{ (decltype(&getGlobalPtr))GetProcAddress(g_module, __FUNCTION__) };
	return fn ? fn(index) : NULL;
}
EXPORT int createTexture(const char* fileName)
{
	auto fn{ (decltype(&createTexture))GetProcAddress(g_module, __FUNCTION__) };
	return fn ? fn(fileName) : NULL;
}
EXPORT void drawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a)
{
	auto fn{ (decltype(&drawTexture))GetProcAddress(g_module, __FUNCTION__) };
	if (fn)
	{
		fn(id, index, level, time, sizeX, sizeY, centerX, centerY, posX, posY, rotation, screenHeightScaleFactor, r, g, b, a);
	}
}
EXPORT void registerRawStreamingFile(const std::string& fileName, const std::string& registerAs)
{
	auto fn{ (decltype(&registerRawStreamingFile))GetProcAddress(g_module, __FUNCTION__) };
	if (fn)
	{
		fn(fileName, registerAs);
	}
}
EXPORT void scriptWait(unsigned long waitTime)
{
	auto fn{ (decltype(&scriptWait))GetProcAddress(g_module, __FUNCTION__) };
	if (fn)
	{
		fn(waitTime);
	}
}
EXPORT void keyboardHandlerRegister(TKeyboardFn function)
{
	auto fn{ (decltype(&keyboardHandlerRegister))GetProcAddress(g_module, __FUNCTION__) };
	if (fn)
	{
		fn(function);
	}
}
EXPORT void keyboardHandlerUnregister(TKeyboardFn function)
{
	auto fn{ (decltype(&keyboardHandlerUnregister))GetProcAddress(g_module, __FUNCTION__) };
	if (fn)
	{
		fn(function);
	}
}
EXPORT eGameVersion getGameVersion()
{
	auto fn{ (decltype(&getGameVersion))GetProcAddress(g_module, __FUNCTION__) };
	return fn ? fn() : eGameVersion::VER_UNK;
}
EXPORT BYTE* getScriptHandleBaseAddress(int handle)
{
	auto fn{ (decltype(&getScriptHandleBaseAddress))GetProcAddress(g_module, __FUNCTION__) };
	return fn ? fn(handle) : NULL;
}
EXPORT void presentCallbackUnregister(PresentCallback cb)
{
	auto fn{ (decltype(&presentCallbackUnregister))GetProcAddress(g_module, __FUNCTION__) };
	if (fn)
	{
		fn(cb);
	}
}
EXPORT void presentCallbackRegister(PresentCallback cb)
{
	auto fn{ (decltype(&presentCallbackRegister))GetProcAddress(g_module, __FUNCTION__) };
	if (fn)
	{
		fn(cb);
	}
}
EXPORT int worldGetAllVehicles(int* arr, int arrSize)
{
	auto fn{ (decltype(&worldGetAllVehicles))GetProcAddress(g_module, __FUNCTION__) };
	return fn ? fn(arr, arrSize) : NULL;
}
EXPORT int worldGetAllPeds(int* arr, int arrSize)
{
	auto fn{ (decltype(&worldGetAllPeds))GetProcAddress(g_module, __FUNCTION__) };
	return fn ? fn(arr, arrSize) : NULL;
}
EXPORT int worldGetAllObjects(int* arr, int arrSize)
{
	auto fn{ (decltype(&worldGetAllObjects))GetProcAddress(g_module, __FUNCTION__) };
	return fn ? fn(arr, arrSize) : NULL;
}
EXPORT int worldGetAllPickups(int* arr, int arrSize)
{
	auto fn{ (decltype(&worldGetAllPickups))GetProcAddress(g_module, __FUNCTION__) };
	return fn ? fn(arr, arrSize) : NULL;
}

BOOL APIENTRY DllMain(HMODULE hmod, DWORD  reason, LPVOID)
{
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			CreateThread(nullptr, NULL, [](void* hmodule) -> DWORD WINAPI {
				printf("Loaded SHV dll\n");
				g_config.load_from_file();
				while (g_running)
				{
					if (g_config.valid("unload"))
					{
						if (g_config.is_key_pressed("unload"))
						{
							g_running = false;
						}
					}
					else
					{
						if (GetAsyncKeyState(VK_F12))
						{
							g_running = false;
						}
					}
				}
				printf("Unloaded SHV dll\n");
				FreeLibraryAndExitThread(HMODULE(hmodule), 0);
				return 0;
			}, hmod, NULL, nullptr);
		} break;
		case DLL_PROCESS_DETACH:
		{
			g_running = false;
		} break;
	}
	return TRUE;
}