#pragma once
#include "memory/pointers.h"
#include "commands/list.h"
#include "commands/cmd_util.h"
#include "util.h"
#include "command_gui_wrapper.h"
#include "config/command_config.h"
#include "scripthookv/module_loader.h"

namespace dwn::renderer
{
	namespace menu
	{
		inline void init()
		{
			dwn::config::g_command_config.handle_init();
			commands::sub_manager settings_command_tests_sub{ "settings_command_tests" };
			settings_command_tests_sub.add_cmd("change_player_model", [](commands::single_command* command) {
				u32 hash{ "player_zero"_j };
				rage::run_as_thread_callback("main_persistent"_j, [hash] {
					for (int i{}; i != 400 && !STREAMING::HAS_MODEL_LOADED(hash); ++i)
					{
						STREAMING::REQUEST_MODEL(hash);
					}
					if (!STREAMING::HAS_MODEL_LOADED(hash))
					{
						LOG_TO_STREAM("Model does not exist in memory yet.");
					}
					Ped ped{ PLAYER::PLAYER_PED_ID() };
					Player player{ PLAYER::PLAYER_ID() };
					PLAYER::SET_PLAYER_MODEL(player, hash);
					PED::SET_PED_DEFAULT_COMPONENT_VARIATION(ped);

					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
				});
			});
			settings_command_tests_sub.add_cmd("spawn_a_t20", [](commands::single_command* command)  {
				u32 hash{ "t20"_j };
				rage::run_as_thread_callback("main_persistent"_j, [hash] {
					if (STREAMING::IS_MODEL_VALID(hash))
					{
						bool unload_model{ true };
						for (int i{}; i != 400 && !STREAMING::HAS_MODEL_LOADED(hash); ++i)
						{
							STREAMING::REQUEST_MODEL(hash);
						}
						if (!STREAMING::HAS_MODEL_LOADED(hash))
						{
							unload_model = false;
							LOG_TO_STREAM("Model does not exist in memory yet.");
						}
						Ped ped{ PLAYER::PLAYER_PED_ID() };
						rage::scrVector pos{ ENTITY::GET_ENTITY_COORDS(ped, true) };
						Vehicle vehicle{ VEHICLE::CREATE_VEHICLE(hash, pos, 0.f, true, true, false) };

						if (NETWORK::NETWORK_IS_SESSION_STARTED())
						{
							DECORATOR::DECOR_SET_INT(vehicle, "MPBitset", 0);
						}

						PED::SET_PED_INTO_VEHICLE(ped, vehicle, -1);
						if (unload_model)
						{
							STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
						}
					}
					else
					{
						LOG_TO_STREAM("Not a valid model");
					}
				});
			});
			commands::sub_manager settings_debug_sub{ "settings_debug" };
			settings_debug_sub.add_cmd("test_load_asi", [](commands::single_command* command) {
				shv::g_module_loader.load("NativeTrainer.asi");
			});
			settings_debug_sub.add_cmd("test_unload_asi", [](commands::single_command* command) {
				shv::g_module_loader.unload("NativeTrainer.asi");
			});
			settings_debug_sub.add_cmd("test_unload_all", [](commands::single_command* command) {
				shv::g_module_loader.unload_all();
			});
			commands::sub_manager settings_console_sub{ "settings_console" };
			settings_console_sub.add_cmd("reconnect_to_console_pipe", [](commands::single_command* command) {
				reconnect_to_remote_console();
			});
			settings_console_sub.add_cmd("connect_to_console_pipe", [](commands::single_command* command) {
				g_console->close_handles(true);
				make_remote_console();
			});
			settings_console_sub.add_cmd("use_attached_console", [](commands::single_command* command) {
				destroy_remote_console();
				g_console->close_handles(true);
				g_console->init_wapi(false);
				g_console->init_cout();
				g_console->set_console_title(g_console->m_title);
			});
			settings_console_sub.add_cmd("free_console", [](commands::single_command* command) {
				destroy_remote_console();
				g_console->close_handles(true);
			});
			commands::sub_manager settings_sub{ "settings" };
			settings_sub.add_sub(settings_command_tests_sub);
			settings_sub.add_sub(settings_debug_sub);
			settings_sub.add_sub(settings_console_sub);
			settings_sub.add_cmd("exit_game", [](commands::single_command* command) {
				exit(0);
				abort();
			});
			settings_sub.add_cmd("unload", [](commands::single_command* command) {
				g_running = false;
			});
			commands::sub_manager home_sub{ "home" };
			home_sub.add_sub(settings_sub);
			home_sub.add_cmd("do_a_funny", [](commands::single_command* command) {
				LOG_TO_STREAM("Do a funny.");
				rage::strLocalIndex idx = pointers::g_StreamedScripts->Register("testscript");
				LOG_TO_STREAM("Index: " << idx.Get());
				//if (pointers::g_StreamedScripts->LoadFile(idx, R"(X:\gta5_old\script\dev_ng\BGNG\BGScript\RELEASE\testscript.ysc)"))
				//{
				//	LOG_TO_STREAM("The funny has happened.");
				//}
			});
			home_sub.add_cmd("hello_world", [](commands::single_command* command) {
				LOG_TO_STREAM("Hello, world!");
			});
			home_sub.check_if_init();
			commands::g_manager.push_back(home_sub);
			dwn::config::g_command_config.handle_init();
			commands::sub_manager* sub{ commands::g_manager.get_sub_at(0) };
			push(sub);
			commands::create_fonts();
			commands::gui::flip_bit(true);
		}

		inline void uninit()
		{
			commands::destroy_fonts();
		}

		inline void tick()
		{
			PAD::DISABLE_CONTROL_ACTION(0, 27, commands::gui::is_open());
		}

		inline void render()
		{
			commands::draw_menu();
		}
	}
}