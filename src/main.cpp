#include "pch/pch.h"
#include "common/classes.h"
#include "exception_handler/handler.h"
#include "hooking/hooks.h"
#include "memory/pointers.h"
#include "build_number.h"
using namespace rage;

void game_speedup() {
	std::this_thread::sleep_for(2600ms);
	cs::memory::pointer_manager ptr_mgr{};
	cs::pointers::game_speedup_scan(ptr_mgr);
	g_game_speedup_active = true;

	while (g_game_speedup_active) {
		if (cs::pointers::g_LoadingScreenContext) {
			LoadingScreenContext& value = *cs::pointers::g_LoadingScreenContext;
			switch (value) {
				case LOADINGSCREEN_CONTEXT_INTRO_MOVIE: {
					std::this_thread::sleep_for(3000ms);
					cs::pointers::g_CLoadingScreensSetLoadingContext(LOADINGSCREEN_CONTEXT_LEGALMAIN);
				} break;
				case LOADINGSCREEN_CONTEXT_LANDING:
				case LOADINGSCREEN_CONTEXT_LEGALMAIN: {
					std::this_thread::sleep_for(2700ms);
					cs::pointers::g_CLoadingScreensSetLoadingContext(LOADINGSCREEN_CONTEXT_LOADGAME);
					cs::pointers::g_CLoadingScreensSetMovieContext(LOADINGSCREEN_SCALEFORM_CTX_GAMELOAD);
				} break;

				case LOADINGSCREEN_CONTEXT_LOADGAME: {
					std::this_thread::sleep_for(3900ms);
					cs::pointers::g_CLoadingScreensShutdown(0);
					g_game_speedup_active = false;
				} break;
			}
			if (value != LOADINGSCREEN_CONTEXT_NONE) {
				LOG_TO_STREAM("Loading screen state: " << (int)value);

				if (g_game_speedup_active && value == LOADINGSCREEN_CONTEXT_LOADGAME) {
					LOG_TO_STREAM("Game speedup currently active, the game will freeze after landing for about 5-10s");
				}
			}
		}

		std::this_thread::sleep_for(10ms);
	}
}
void game_death() {
	LOG_TO_STREAM("Game fast-exit thread created.");
	while (g_running) {
		if (GetAsyncKeyState(VK_F4)) {
			exit(0);
			abort();
		}

		std::this_thread::sleep_for(100ms);
	}
}
void routine() {
	g_was_injected_early = cs::memory::hmodule("socialclub.dll").exists(); //TODO: Fix
	if (g_was_injected_early) {
		std::this_thread::sleep_for(1000ms);
		std::thread(&game_speedup).detach();
	}

	cs::pointers::g_GetProcAddress = &GetProcAddress;
	make_hook_time_critical("GetProcAddress", GetProcAddress);

	if (g_was_injected_early) {
		std::this_thread::sleep_for(2000ms);
	}

	// haha fuck your timing LOL
	cs::konsole::create(cs::g_console, BASE_NAME " | " BASE_CANDIDATE " " BUILD, "log.txt");
	LOG_TO_STREAM("Console created.");

	cs::hooking::create();

	std::thread(&game_death).detach();

	if (!IsDebuggerPresent()) {
		cs::exception::attach_handler();
		LOG_TO_STREAM("Attached exception handler at " << HEX((u64)cs::exception::g_handle));
		LOG_TO_STREAM("Attached exception filter at " << HEX((u64)cs::exception::g_filter_handle));
	}
	else {
		LOG_TO_STREAM("WARNING: Exception handler not attached!");
		LOG_TO_STREAM("WARNING: Exception filter not attached!");
	}

	while (g_running) {
		if (GetAsyncKeyState(VK_F12)) {
			g_running = false;
		}

		std::this_thread::sleep_for(100ms);
	}

	cs::hooking::remove();

	LOG_TO_STREAM("Destroying console...");
	cs::konsole::destroy(cs::g_console);

	cs::exception::detach_handler();
}

DWORD WINAPI entry(void* hmod) {
	g_entry = reinterpret_cast<HMODULE>(hmod);
	std::thread(&routine).detach();

	while (g_running) {
		std::this_thread::sleep_for(100ms);
	}

	FreeLibraryAndExitThread(g_entry, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hmod, DWORD  reason, LPVOID) {
	switch (reason) {
		case DLL_PROCESS_ATTACH: {
			CreateThread(nullptr, NULL, &entry, hmod, NULL, nullptr);
		} break;
		case DLL_PROCESS_DETACH: {
			g_running = false;
		} break;
	}
	return TRUE;
}