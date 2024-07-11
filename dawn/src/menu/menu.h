#pragma once
#include "memory/pointers.h"
#include "commands/list.h"
#include "commands/cmd_util.h"
#include "util.h"
#include "features/features.h"
#include "test_gui.h"

inline void print_sc_data()
{
	rage::rlPc& pc{ *dwn::pointers::g_rlPc };
	rage::rlTitleId& titleId{ *(*dwn::pointers::g_rlTitleId) };

	LOG_TO_STREAM("SocialClub Version: " << pc.m_SocialClubVersion);
	LOG_TO_STREAM("Additional Session Attribute: " << pc.m_AdditionalSessionAttr);
	LOG_TO_STREAM("Additional Join Attribute: " << pc.m_AdditionalJoinAttr);
	LOG_TO_STREAM("MetaData Path: " << pc.m_MetaDataPath);

	/*if (pc.m_Telemetry->HasAnyMetrics())
	{
		LOG_TO_STREAM("RGSC has metrics" << dwn::endl << "Flushing...");
		pc.m_Telemetry->Flush(true);
	}*/
	rgsc::Profile profile;
	pc.m_ProfileManager->GetSignInInfo(&profile);

	LOG_TO_STREAM("--------------------------------------------");

	LOG_TO_STREAM("RGSC SignIn Info (rgsc::Profile)");

	LOG_TO_STREAM("--------------------------------------------");

	LOG_TO_STREAM("Nickname: " << profile.GetNickname());
	LOG_TO_STREAM("Email: " << profile.GetSocialClubEmail());
	LOG_TO_STREAM("Password: " << profile.GetSocialClubPassword());
	LOG_TO_STREAM("Auth Token: " << profile.GetSocialClubAuthToken());
	LOG_TO_STREAM("Profile ID: " << profile.GetProfileId());

	LOG_TO_STREAM("--------------------------------------------");
	LOG_TO_STREAM("ROS Title ID");
	LOG_TO_STREAM("NOTE: Some data might be garbage due to the data not existing in process");

	LOG_TO_STREAM("--------------------------------------------");
	LOG_TO_STREAM("Title Name: " << titleId.m_RosTitleId.m_TitleName);
	LOG_TO_STREAM("Title Version: " << titleId.m_RosTitleId.m_TitleVersion);
	LOG_TO_STREAM("SC Version: " << titleId.m_RosTitleId.m_ScVersion);
	LOG_TO_STREAM("Title Secrets: " << titleId.m_RosTitleId.m_TitleSecretsBase64);
	LOG_TO_STREAM("Title Directory Name: " << titleId.m_RosTitleId.m_TitleDirectoryName);

	LOG_NEWLINE();
	LOG_TO_STREAM("const u8 rsa_key[" << titleId.m_RosTitleId.m_PublicRsaKeyLen << "] = {");
	for (u32 i{}; i != titleId.m_RosTitleId.m_PublicRsaKeyLen; i += 4)
	{
		for (u32 j{}; j != 4; ++j)
		{
			LOG_TO_STREAM_NO_IO_MANIP(HEX(titleId.m_RosTitleId.m_PublicRsaKey[i+j]) << ", ");
		}
		LOG_NEWLINE();
	}
	LOG_TO_STREAM("};");
}
namespace dwn::renderer
{
	namespace menu
	{
		inline void init()
		{
			submenu sub{ make_sub("Home", [](submenu& sub) {
				sub.add(submenu_option("Settings", [](submenu& sub) {
					sub.add(submenu_option("Command tests", [](submenu& sub) {
						sub.add(base_option("Change player model", [] {
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
						}));
						sub.add(base_option("Spawn a t20", [] {
							u32 hash{ "t20"_j };
							rage::run_as_thread("main_persistent"_j, [hash] {
								if (STREAMING::IS_MODEL_VALID(hash))
								{
									bool unload_model{ true };
									for (int i{}; i != 40000 && !STREAMING::HAS_MODEL_LOADED(hash); ++i)
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
						}));
					}));
					sub.add(submenu_option("Debug", [](submenu& sub) {
						sub.add(base_option("Print SocialClub data", [] {
							print_sc_data();
						}));
						sub.add(base_option("Print CRC data", [] {
							CNetworkAssetVerifier* asset_verifier = (*pointers::g_CNetworkAssetVerifiersmInstance);
							u32 file_checksum = asset_verifier->m_fileChecksum.Get();
							u32 num_files = asset_verifier->m_numFiles.Get();
							int crc = asset_verifier->m_CRC.Get();
							int static_crc = asset_verifier->m_StaticCRC.Get();
							LOG_TO_STREAM("File checksum: " << HEX(file_checksum));
							LOG_TO_STREAM("Num files: " << DEC(num_files));
							LOG_TO_STREAM("CRC: " << HEX(crc));
							LOG_TO_STREAM("Static CRC: " << DEC(static_crc));
						}));
						sub.add(base_option("Test load DLC", [] {
							CExtraContentManager* content_mgr = *pointers::g_CExtraContentManagersmInstance;
							if (pointers::g_CExtraContentManagerAddContentFolder(content_mgr, "C:/Users/Vali/Documents/Cherax/DLCs/rmodlp770/"))
							{
								LOG_TO_STREAM("Passed.");
							}
							pointers::g_CExtraContentManagerLoadContent(content_mgr, false, false);
						}));
					}));
					sub.add(submenu_option("Console", [](submenu& sub) {
						sub.add(base_option("Reconnect to console pipe", [] {
							reconnect_to_remote_console();
						}));
						sub.add(base_option("Connect to console pipe", [] {
							g_console->close_handles(true);
							make_remote_console();
						}));
						sub.add(base_option("Use attached console", [] {
							destroy_remote_console();
							g_console->close_handles(true);
							g_console->init_wapi(false);
							g_console->init_cout();
							g_console->set_console_title(g_console->m_title);
						}));
						sub.add(base_option("Free console", [] {
							destroy_remote_console();
							g_console->close_handles(true);
						}));
					}));
					sub.add(base_option("Exit game", [] {
						exit(0);
						abort();
					}));
					sub.add(base_option("Unload", [] {
						g_running = false;
					}));
				}));
				sub.add(base_option("Hello, world", [] {
					LOG_TO_STREAM("Hello!");
				}));
				sub.add(base_option("Hello, world", [] {
					LOG_TO_STREAM("Hello!");
				}));
			}) };
			push(sub);
			stdfs::path font_path{ std::getenv("appdata") };
			font_path /= BASE_NAME;
			font_path /= "Fonts";
			std::string str{ (font_path / "icon_font.ttf").string() };
			LOG_TO_STREAM("Path: " << str);
			iconfont_22 = directx::load_font(str.data(), 22.f, get()->m_device);
			iconfont_24 = directx::load_font(str.data(), 24.f, get()->m_device);
			arial_22 = directx::load_font("C:\\Windows\\Fonts\\arial.ttf", 22.f, get()->m_device);
			arial_23 = directx::load_font("C:\\Windows\\Fonts\\arial.ttf", 23.f, get()->m_device);
			arialbd_22 = directx::load_font("C:\\Windows\\Fonts\\arialbd.ttf", 22.f, get()->m_device);
			arialbd_23 = directx::load_font("C:\\Windows\\Fonts\\arialbd.ttf", 23.f, get()->m_device);
			ariali_22 = directx::load_font("C:\\Windows\\Fonts\\ariali.ttf", 22.f, get()->m_device);
			ariali_23 = directx::load_font("C:\\Windows\\Fonts\\ariali.ttf", 23.f, get()->m_device);
			arialbi_22 = directx::load_font("C:\\Windows\\Fonts\\arialbi.ttf", 22.f, get()->m_device);
			arialbi_23 = directx::load_font("C:\\Windows\\Fonts\\arialbi.ttf", 23.f, get()->m_device);
		}
		inline void uninit()
		{
			directx::destroy_font(iconfont_22);
			directx::destroy_font(iconfont_24);
			directx::destroy_font(arial_22);
			directx::destroy_font(arial_23);
			directx::destroy_font(arialbd_22);
			directx::destroy_font(arialbd_23);
			directx::destroy_font(ariali_22);
			directx::destroy_font(ariali_23);
			directx::destroy_font(arialbi_22);
			directx::destroy_font(arialbi_23);
		}

		int g_current_player{};
		namespace main
		{
			/*inline void render_settings()
			{
				tab_bar("settings_bar", [] {
					tab_item("Command tests", [] {
						button("Spawn a adder", [] {
							u32 hash{ "adder"_j };
							rage::run_as_thread("main_persistent"_j, [hash] {
								for (int i{}; i != 400 && !STREAMING::HAS_MODEL_LOADED(hash); ++i)
								{
									STREAMING::REQUEST_MODEL(hash);
								}
								if (!STREAMING::HAS_MODEL_LOADED(hash))
								{
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

								STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
							});
						});
					});
					tab_item("Debug", [] {
						button("Attempt to force load the game", [] {
							std::vector<std::string> strings;
							strings.push_back("Are you sure you want to do this?\n");
							callback_yesno_popup("Force load", strings, [] {
								LoadingScreenContext& value = *pointers::g_LoadingScreenContext;
								pointers::g_CLoadingScreensSetLoadingContext(LOADINGSCREEN_CONTEXT_LOADLEVEL);
								pointers::g_CLoadingScreensSetMovieContext(LOADINGSCREEN_SCALEFORM_CTX_BLANK);
								pointers::g_CLoadingScreensShutdown(0);
							});
						});
						button("Print SocialClub data", [] {
							print_sc_data();
						});
						button("Print CRC data", [] {
							CNetworkAssetVerifier* asset_verifier = (*pointers::g_CNetworkAssetVerifiersmInstance);
							u32 file_checksum = asset_verifier->m_fileChecksum.Get();
							u32 num_files = asset_verifier->m_numFiles.Get();
							int crc = asset_verifier->m_CRC.Get();
							int static_crc = asset_verifier->m_StaticCRC.Get();
							LOG_TO_STREAM("File checksum: " << HEX(file_checksum));
							LOG_TO_STREAM("Num files: " << DEC(num_files));
							LOG_TO_STREAM("CRC: " << HEX(crc));
							LOG_TO_STREAM("Static CRC: " << DEC(static_crc));
						});
						button("Test load DLC", [] {
							CExtraContentManager* content_mgr = *pointers::g_CExtraContentManagersmInstance;
							if (pointers::g_CExtraContentManagerAddContentFolder(content_mgr, "C:/Users/Vali/Documents/Cherax/DLCs/rmodlp770/"))
							{
								LOG_TO_STREAM("Passed.");
							}
							pointers::g_CExtraContentManagerLoadContent(content_mgr, false, false);
						});
					});
					tab_item("Console", [] {
						button("Reconnect to console pipe", [] {
							dwn::reconnect_to_remote_console();
						});
						button("Connect to console pipe", [] {
							g_console->close_handles(true);
							make_remote_console();
						});
						button("Use attached console", [] {
							destroy_remote_console();
							g_console->close_handles(true);
							g_console->init_wapi(false);
							g_console->init_cout();
							g_console->set_console_title(g_console->m_title);
						});
						button("Free console", [] {
							destroy_remote_console();
							g_console->close_handles(true);
						});
					});
					tab_button("Exit game", [] {
						exit(0);
						abort();
					});
					tab_button("Unload", [] {
						g_running = false;
					});
				});
			}*/
			inline void render()
			{
				draw_menu();
			}
		}

		inline void tick()
		{
			PAD::DISABLE_CONTROL_ACTION(0, 27, gui::is_open());
			features::on_tick();
		}

		inline void render()
		{
			main::render();
		}
	}
}