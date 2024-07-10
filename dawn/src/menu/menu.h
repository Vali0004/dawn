#pragma once
#include "memory/pointers.h"
#include "menu/fonts.h"
#include "menu/elements.h"
#include "commands/list.h"
#include "commands/cmd_util.h"
#include "util.h"
#include "features/features.h"

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
	namespace gui
	{
		inline bool g_open{};
		inline bool is_open()
		{
			return g_open;
		}

		inline void flip_bit(bool trigger_flip = false)
		{
			if (trigger_flip)
			{
				g_open ^= 1;
			}
		}
	}

	namespace menu
	{
		inline void init()
		{
			static auto&& io{ ImGui::GetIO() };
			static auto&& style{ ImGui::GetStyle() };
			static auto&& colors{ style.Colors };

			io.MouseDrawCursor = gui::g_open;
			style.WindowTitleAlign = { 0.f, 0.5f };
			style.WindowRounding = 5.5f;
			style.ChildRounding = 0.1f;
			style.FrameRounding = 4.f;
			style.GrabRounding = 3.f;
			style.PopupRounding = 5.5f;
			style.ScrollbarRounding = 12.f;
			style.ScrollbarSize = 9.f;
			style.FrameBorderSize = 0.f;
			style.WindowBorderSize = 1.f;
			style.IndentSpacing = 27.f;
			style.AntiAliasedLines = true;
			style.AntiAliasedFill = true;
			style.AntiAliasedLinesUseTex = true;
			style.WindowMenuButtonPosition = ImGuiDir_Right;

			colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_TextSelectedBg] = ImColor(49, 151, 253, 255);
			colors[ImGuiCol_TextDisabled] = ImColor(40, 40, 40, 255);
			colors[ImGuiCol_WindowBg] = ImColor(15, 15, 15, 255);
			colors[ImGuiCol_ChildBg] = ImColor(20, 20, 20, 255);
			colors[ImGuiCol_PopupBg] = ImColor(20, 20, 20, 255);
			colors[ImGuiCol_Border] = ImColor(255, 95, 140, 255);
			colors[ImGuiCol_BorderShadow] = ImColor(41, 41, 41, 255);
			colors[ImGuiCol_FrameBg] = ImColor(41, 41, 41, 255);
			colors[ImGuiCol_FrameBgHovered] = ImColor(41, 41, 41, 255);
			colors[ImGuiCol_FrameBgActive] = ImColor(41, 41, 41, 255);
			colors[ImGuiCol_TitleBg] = ImColor(43, 43, 43, 255);
			colors[ImGuiCol_TitleBgActive] = ImColor(49, 49, 49, 255);
			colors[ImGuiCol_TitleBgCollapsed] = ImColor(0, 0, 0, 130);
			colors[ImGuiCol_MenuBarBg] = ImColor(30, 30, 30, 255);
			colors[ImGuiCol_ScrollbarBg] = ImColor(50, 50, 50, 255);
			colors[ImGuiCol_ScrollbarGrab] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_ScrollbarGrabActive] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_SliderGrab] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_SliderGrabActive] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_Button] = ImColor(41, 41, 41, 255);
			colors[ImGuiCol_ButtonHovered] = ImColor(41, 41, 41, 255);
			colors[ImGuiCol_ButtonActive] = ImColor(41, 41, 41, 255);
			colors[ImGuiCol_Header] = ImColor(94, 94, 94, 130);
			colors[ImGuiCol_HeaderHovered] = ImColor(97, 97, 97, 94);
			colors[ImGuiCol_HeaderActive] = ImColor(94, 94, 94, 130);
			colors[ImGuiCol_Separator] = ImColor(41, 41, 41, 255);
			colors[ImGuiCol_SeparatorHovered] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_SeparatorActive] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_ResizeGrip] = ImColor(66, 66, 66, 255);
			colors[ImGuiCol_ResizeGripHovered] = ImColor(80, 80, 80, 255);
			colors[ImGuiCol_ResizeGripActive] = ImColor(90, 90, 90, 255);
			colors[ImGuiCol_Tab] = ImColor(50, 50, 50, 255);
			colors[ImGuiCol_TabHovered] = ImColor(80, 80, 80, 255);
			colors[ImGuiCol_TabActive] = ImColor(80, 80, 80, 255);
			colors[ImGuiCol_TabUnfocused] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_TabUnfocusedActive] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_PlotLines] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_PlotLinesHovered] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_PlotHistogram] = ImColor(230, 179, 0, 255);
			colors[ImGuiCol_PlotHistogramHovered] = ImColor(255, 153, 0, 255);
			colors[ImGuiCol_DragDropTarget] = ImColor(255, 255, 0, 230);
			colors[ImGuiCol_NavHighlight] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_NavWindowingHighlight] = ImColor(255, 255, 255, 179);
			colors[ImGuiCol_NavWindowingDimBg] = ImColor(204, 204, 204, 50);
			colors[ImGuiCol_ModalWindowDimBg] = ImColor(204, 204, 204, 90);

			ImFontConfig cfg{};
			cfg.FontDataOwnedByAtlas = false;

			stdfs::path win_fonts{ R"(C:\Windows\Fonts)" };
			ImFont* font = io.Fonts->AddFontFromFileTTF((win_fonts / "arial.ttf").string().c_str(), 19.f, &cfg);

			//fonts::g_font_mgr.add_font_to_atlas(io.Fonts, fonts::font(win_fonts / "arial.ttf", 19.f, cfg));
			//fonts::g_font_mgr.add_font_to_atlas(io.Fonts, fonts::font(win_fonts / "arialbd.ttf", 19.f, cfg));
		}

		int g_current_player{};
		namespace main
		{
			inline void render_self()
			{
				tab_bar("self_bar", [] {
					tab_item("Movement", [] {
						checkbox("No Clip", features::g_features.m_no_clip.m_toggle);
					});
				});
			}

			inline void render_network()
			{
				tab_bar("network_bar", [] {
					tab_item("Players", [] {
						list_box("##player_listbox", ImVec2(200.f, 220.f), [] {
							for (int i{}; i != NETWORK::NETWORK_GET_NUM_CONNECTED_PLAYERS(); ++i)
							{
								selectable(PLAYER::GET_PLAYER_NAME(i), g_current_player == i, [i] {
									g_current_player = i;
								});
							}
						});
					});
				});
			}

			inline void render_settings()
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
			}

			inline void render_bar()
			{
				tab_item("Self", &render_self);
				tab_item("Network", &render_network);
				tab_item("Settings", &render_settings);
			}

			inline void render()
			{
				window("dawn.", gui::g_open, [] {
					//tab_bar("main_bar", &render_bar);
				}, ImGuiWindowFlags_NoDecoration);
			}
		}

		inline void tick()
		{
			/*if (!util::gta::in_loading_screen())
			{
				rage::run_as_thread("main_persistent"_j, [] {

					features::on_tick();
				});
			}
			else
			{
				features::on_tick();
			}*/
			features::on_tick();
		}

		inline void render()
		{
			PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
			set_next_window({ 50.f, 50.f, 650.f - 15.f, 410.f }, ImGuiCond_Once);
			main::render();
		}
	}
}