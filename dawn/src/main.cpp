#include "pch/pch.h"
#include "common/classes.h"
#include "exception_handler/handler.h"
#include "hooking/hooks.h"
#include "memory/pointers.h"
#include "renderer/input.h"
#include "build_number.h"
#include "command_engine/manager.h"
#include "thread_manager/thread_manager.h"
#include "config/hotkey_config.h"
using namespace rage;

void game_speedup()
{
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	//std::this_thread::sleep_for(2600ms);
	/*dwn::memory::pointer_manager ptr_mgr{};
	dwn::pointers::game_speedup_scan(ptr_mgr);
	g_game_speedup_active = true;
	g_game_speedup_active = false;

	while (g_game_speedup_active)
	{
		if (dwn::pointers::g_LoadingScreenContext)
		{
			LoadingScreenContext& value = *dwn::pointers::g_LoadingScreenContext;
			switch (value)
			{
				case LOADINGSCREEN_CONTEXT_INTRO_MOVIE:
				{
					std::this_thread::sleep_for(3000ms);
					dwn::pointers::g_CLoadingScreensSetLoadingContext(LOADINGSCREEN_CONTEXT_LEGALMAIN);
				} break;

				case LOADINGSCREEN_CONTEXT_LANDING:
				case LOADINGSCREEN_CONTEXT_LEGALMAIN:
				{
					std::this_thread::sleep_for(2700ms);
					dwn::pointers::g_CLoadingScreensSetLoadingContext(LOADINGSCREEN_CONTEXT_LOADGAME);
					dwn::pointers::g_CLoadingScreensSetMovieContext(LOADINGSCREEN_SCALEFORM_CTX_GAMELOAD);
				} break;

				case LOADINGSCREEN_CONTEXT_LOADGAME:
				{
					std::this_thread::sleep_for(3900ms);
					dwn::pointers::g_CLoadingScreensShutdown(0);
					g_game_speedup_active = false;
				} break;
			}
			if (value != LOADINGSCREEN_CONTEXT_NONE)
			{
				LOG_TO_STREAM("Loading screen state: " << (int)value);

				if (g_game_speedup_active && value == LOADINGSCREEN_CONTEXT_LOADGAME)
				{
					LOG_TO_STREAM("Game speedup currently active, the game will freeze after landing for about 5-10s");
				}
			}
		}

		std::this_thread::sleep_for(10ms);
	}*/
}
void game_death()
{
	LOG_TO_STREAM("Game fast-exit thread created.");
	while (g_running)
	{
		if (dwn::config::g_hotkey_config.valid("game_exit"))
		{
			if (dwn::config::g_hotkey_config.is_key_pressed("game_exit"))
			{
				exit(0);
				abort();
			}
		}
		else
		{
			if (GetAsyncKeyState(VK_F6))
			{
				exit(0);
				abort();
			}
		}
		std::this_thread::sleep_for(100ms);
	}
}
void routine(dwn::thread* thr)
{
	g_was_injected_early = dwn::memory::hmodule("socialclub.dll").exists(); //TODO: Fix
	if (g_was_injected_early)
	{
		std::this_thread::sleep_for(1000ms);
		dwn::util::spawn_detached_thread(&game_speedup);
	}

	dwn::pointers::g_GetProcAddress = &GetProcAddress;
	make_hook_time_critical("GetProcAddress", GetProcAddress);

	if (g_was_injected_early)
	{
		std::this_thread::sleep_for(2000ms);
	}

	dwn::konsole::create(dwn::g_console, BASE_NAME " | " BASE_CANDIDATE " " BUILD, "log.txt");
	LOG_TO_STREAM("Console created.");
	LOG_TO_STREAM("Base address: " << HEX(dwn::memory::hmodule().begin().as<u64>()));

	dwn::util::spawn_detached_thread(&game_death);

	if (g_was_injected_early)
	{
		LOG_TO_STREAM("Early injection present, awaiting RGSC...");
	}

	if (!dwn::config::g_hotkey_config.valid("unload"))
	{
		LOG_TO_STREAM("Hotkey load failed, using defaults");
	}

	dwn::hooking::create();

	if (!IsDebuggerPresent())
	{
		dwn::exception::attach_handler();
		LOG_TO_STREAM("Attached exception handler at " << HEX((u64)dwn::exception::g_handle));
		LOG_TO_STREAM("Attached exception filter at " << HEX((u64)dwn::exception::g_filter_handle));
	}
	else
	{
		LOG_TO_STREAM("WARNING: Exception handler not attached!");
		LOG_TO_STREAM("WARNING: Exception filter not attached!");
	}

	dwn::renderer::menu::init();

	LOG_TO_STREAM("Init finished");

	while (g_running)
	{
		if (dwn::config::g_hotkey_config.valid("unload"))
		{
			if (dwn::config::g_hotkey_config.is_key_pressed("unload"))
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
		std::this_thread::sleep_for(100ms);
	}

	dwn::renderer::menu::uninit();

	dwn::hooking::remove();

	LOG_TO_STREAM("Destroying console...");
	dwn::konsole::destroy(dwn::g_console);

	dwn::exception::detach_handler();
}

DWORD WINAPI entry(void* hmod)
{
	g_entry = reinterpret_cast<HMODULE>(hmod);
	dwn::g_thread_manager.create_thread(&routine);

	while (g_running)
	{
		dwn::g_thread_manager.update_thread_status();
	}

	FreeLibraryAndExitThread(g_entry, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hmod, DWORD  reason, LPVOID)
{
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			CreateThread(nullptr, NULL, &entry, hmod, NULL, nullptr);
		} break;
		case DLL_PROCESS_DETACH:
		{
			g_running = false;
		} break;
	}
	return TRUE;
}