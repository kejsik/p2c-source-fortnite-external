#pragma once

#include <dwmapi.h>
#include "../imgui/imgui_internal.h"
#include <stdio.h>
#include "../Includes/settings.hpp"
#include "../Includes/sdk.hpp"

#include "../Gui/gui.h"
#include <string>
#include "../Includes/sdk.hpp"
#include "../Includes/includes.hpp"
#include "../string.hpp"
#include "../aimbot/aimbot.h"
#include "../Gui/gui.h"

#include "../Includes/includes.hpp"
#include "../Includes/overlay.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"
#include "offsets.h"
#include "../Xor/xor.hpp"
ImFont* m_pFont;

namespace font {

	ImFont* FN;
}


DWORD_PTR Uworlds;
DWORD_PTR LocalPawns;
DWORD_PTR PlayerStates;
DWORD_PTR Localplayers;
DWORD_PTR Rootcomps;
DWORD_PTR PlayerControllers;
DWORD_PTR Persistentlevels;
DWORD_PTR Ulevels;
uint64_t MESH;
Vector3 lactorpossssition;
namespace Identifier {

	HANDLE DRIVERHANDLE;
	uint64_t ADDRESS;
	int PID;
}
int CloseRangeenemyID = 0;
#define VELLOCITY_OFFSET 0x170

__int64 TargetedBuild;
bool bTargetedBuild;

char* drawBuff = (char*)malloc(4096);


void game_loop()
{

	if (GetAsyncKeyState(VK_F5)) {
		system("pause");
	}
	if (settings::aimbot::renderfps)
	{
		char fps[256]; // 64
		sprintf_s(fps, "discord.gg/Thrilla", ImGui::GetIO().Framerate); 
		DrawString(19, ImGui::GetIO().DisplaySize.x / 20, 20, ImColor(255, 255, 255), true, true, fps);
	}
	gui::render();
	Vector2 screen_center1 = { (double)settings::width / 8, (double)settings::height / 8 };
	auto RGB = ImGui::GetColorU32({ color_red, color_green, color_blue, 255 });
	auto RG1B = ImGui::GetColorU32({ color_red, color_green, color_blue, .1 });
	ImGui::PushFont(FN);

	if (settings::aimbot::show_fov)
	{
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), settings::aimbot::fov - ((fov - 80) * 2.5), ImColor({ settings::visuals::FovColor[0], settings::visuals::FovColor[1], settings::visuals::FovColor[2], settings::visuals::FovColor[3] }));
	}

	if (settings::visuals::settingsStreamproof) {
		SetWindowDisplayAffinity(win, WDA_EXCLUDEFROMCAPTURE);
	}
	if (!settings::visuals::settingsStreamproof) {
		SetWindowDisplayAffinity(win, !WDA_EXCLUDEFROMCAPTURE);
	}

	pointer::uworld = drv->Read<uintptr_t>(baseaddy + offsets::Uworld);
	//if (!pointer::uworld) {
	//	int injection = MessageBoxA(0, ("Failed To find Uworld"), ("Thrilla Slotted :)"), MB_OK);
	//	switch (injection)
	//	{
	//		return; 
	//	}
	//	ShowWindow(::GetConsoleWindow(), SW_HIDE);

	//}
	/*cout << pointer::uworld;*/

	pointer::game_instance = drv->Read<uintptr_t>(pointer::uworld + offsets::OwningGameInstance);

	/*cout << pointer::game_instance;*/

	pointer::local_players = drv->Read<uintptr_t>(drv->Read<uintptr_t>(pointer::game_instance + offsets::local_players));

	pointer::player_controller = drv->Read<uintptr_t>(pointer::local_players + offsets::PlayerController);

	pointer::local_pawn = drv->Read<uintptr_t>(pointer::player_controller + offsets::AcknowledgedPawn);

	pointer::root_component = drv->Read<uintptr_t>(pointer::local_pawn + offsets::RootComponent);

	pointer::player_state = drv->Read<uintptr_t>(pointer::local_pawn + offsets::PlayerState);

	pointer::game_state = drv->Read<uintptr_t>(pointer::uworld + offsets::GameState);

	pointer::player_array = drv->Read<uintptr_t>(pointer::game_state + offsets::PlayerArray);
	//cout << pointer::player_array;

	pointer::AFortPlayerState = drv->Read<uint64_t>(pointer::local_pawn + offsets::PlayerState);

	pointer::local_relative_location = drv->Read<Vector3>(pointer::root_component + offsets::RelativeLocation);

	pointer::ULevel = drv->Read<uint64_t>(baseaddy + Offsets::PersistentLevel);

	pointer::Actors = drv->Read<uintptr_t>(pointer::ULevel + offsets::AActor);

	pointer::closest_distance = FLT_MAX;

	pointer::closest_pawn = NULL;

	int count = drv->Read<int>(pointer::game_state + (0x02a0 + sizeof(uintptr_t)));
	for (int i = 0; i < count; i++)
	{
		auto current_actor = drv->Read<uint64_t>(pointer::Actors + i * 0x8);

		int PlayerState = drv->Read<uint64_t>(current_actor + offsets::PlayerState);

		uintptr_t current_player_state = drv->Read<uintptr_t>(pointer::player_array + (i * sizeof(uintptr_t)));

		if (!current_player_state) continue;

		int my_team_id = drv->Read<int>(pointer::player_state + offsets::TeamID); //TeamIndex

		int current_actor_team_id = drv->Read<int>(current_player_state + offsets::TeamID); //TeamIndex

		uintptr_t current_local_pawn_private = drv->Read<uintptr_t>(current_player_state + 0x300);
		if (!current_local_pawn_private) continue;

		if (current_local_pawn_private == pointer::local_pawn) continue;

		uintptr_t current_mesh = drv->Read<uintptr_t>(current_local_pawn_private + offsets::mesh);
		if (!current_mesh) continue;



		int Actor = current_actor;

		int screenx = 0;
		int screeny = 0;

		Vector3 head3d = GetBoneWithRotation(current_mesh, 106);
		Vector3 head2d = ProjectWorldToScreen(head3d);
		Vector3	HeadBone = GetBoneWithRotation(current_mesh, 68);
		Vector3	BaseBone = GetBoneWithRotation(current_mesh, 0);
		Vector3 HeadBoneOutput = ProjectWorldToScreen(HeadBone);
		Vector3 BaseBoneOutput = ProjectWorldToScreen(BaseBone);
		Vector3 bottom3d = GetBoneWithRotation(current_mesh, 0);
		Vector3 bottom2d = ProjectWorldToScreen(bottom3d);
		Vector3 bone0 = GetBoneWithRotation(current_mesh, 0);
		Vector3 Rootbox = ProjectWorldToScreen(Vector3(bottom3d.x, bottom3d.y, bottom3d.z - 10));
		Vector3 Headbox1 = ProjectWorldToScreen(Vector3(head3d.x, head3d.y, head3d.z + 15));
		Vector3 bottom2 = ProjectWorldToScreen(Vector3(bottom3d.x, bottom3d.y, bottom3d.z + 0));
	
		float corner_height = abs(head2d.y - bottom2d.y);
		float corner_width = corner_height * 0.60;
		double dx = head2d.x - (settings::width / 2);
		double dy = head2d.y - (settings::height / 2);
		float dist = sqrtf(dx * dx + dy * dy);
		Vector3 localactorpos;
		uint64_t CurActorRootComponent = drv->Read<uint64_t>(Actor + offsets::RootComponent);
		localactorpos = drv->Read<Vector3>(pointer::root_component + offsets::RelativeLocation);
		Vector3 Headpos = GetBoneWithRotation(current_mesh, 106);
		Vector3 vRootBone = GetBoneWithRotation(current_mesh, fn::bones::root);
		float distance = localactorpos.Distance(HeadBone) / fov;
		double player_distance = vCamera.Location.Distance(vRootBone);
		Vector3 vRootBox = Vector3(vRootBone.x, vRootBone.y, vRootBone.z - 5);
		uint64_t Mesh2 = drv->Read<uint64_t>(current_actor + mesh);
		if (dist < settings::aimbot::fov && dist < pointer::closest_distance)
		{
			pointer::closest_distance = dist;
			pointer::closest_pawn = current_local_pawn_private;
		}

		auto ColorSkeleton = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		bool isvisible;

		if (IsVisible(current_mesh))
		{
			visennemy += 1;
			isvisible = true;
		}
		else
		{
			isvisible = false;

		}

		if (settings::visuals::enable)
		{
			int bottom_offset = 7, top_offset = 17;
			ImColor color = ImColor(255, 0, 255, 255);
			ImColor color1 = ImColor(255, 0, 255, 255);
			auto Black = ImGui::ColorConvertFloat4ToU32(ImVec4(0 / 255.0, 0 / 255.0, 0 / 255.0, 255 / 255.0));
			static int size[2] = { 200, 200 };
			static int pos[2] = { 100, 100 };

			if (settings::visuals::box)
			{
				if (settings::visuals::vischeck)
				{
					if (IsVisible(current_mesh))
					{
						DrawNormalBox(Headbox1.x - corner_width / 2, Headbox1.y - 1, corner_width, corner_height, 1.0f, ImColor({ settings::visuals::VisCOlor[0], settings::visuals::VisCOlor[1], settings::visuals::VisCOlor[2], settings::visuals::VisCOlor[3] }), settings::visuals::boxwidth);
					
					}
					else
					{
						DrawNormalBox(Headbox1.x - corner_width / 2, Headbox1.y - 1, corner_width, corner_height, 1.0f, ImColor({ settings::visuals::NOtVIs[0], settings::visuals::NOtVIs[1], settings::visuals::NOtVIs[2], settings::visuals::NOtVIs[3] }), settings::visuals::boxwidth);
						
					}
				}
				else
					draw_cornered_box(Headbox1.x - (corner_width / 2), Headbox1.y, corner_width, corner_height, ImColor({ settings::visuals::box_color[0], settings::visuals::box_color[1], settings::visuals::box_color[2], settings::visuals::box_color[3] }), 1.5, 0);
			}

			if (settings::visuals::roundedboxes)
			{

				if (settings::visuals::vischeck)
				{
					if (IsVisible(current_mesh))
					{


						float BoxHeight = Rootbox.y - Headbox1.y;
						float BoxWidth = BoxHeight / 1.9f;
						box_esp(4, 4, Rootbox.x - (BoxWidth / 2), Headbox1.y,
							BoxWidth, BoxHeight, ImColor({ settings::visuals::VisCOlor[0], settings::visuals::VisCOlor[1], settings::visuals::VisCOlor[2], settings::visuals::VisCOlor[3] }), settings::visuals::boxwidth,
							settings::visuals::fill, settings::visuals::Outline);

					}
					else
					{
					
						float BoxHeight = Rootbox.y - Headbox1.y;
						float BoxWidth = BoxHeight / 1.9f;
						box_esp(4, 4, Rootbox.x - (BoxWidth / 2), Headbox1.y,
							BoxWidth, BoxHeight, ImColor({ settings::visuals::NOtVIs[0], settings::visuals::NOtVIs[1], settings::visuals::NOtVIs[2], settings::visuals::NOtVIs[3] }), settings::visuals::boxwidth,
							settings::visuals::fill, settings::visuals::Outline);
					}

				}
				else

					draw_rect(Headbox1.x - (corner_width / 2), Headbox1.y, corner_width, corner_height, ImColor({ settings::visuals::box_color[0], settings::visuals::box_color[1], settings::visuals::box_color[2], settings::visuals::box_color[3] }), 2.5f);
			}


			if (settings::visuals::gayskeleton)
			{
				skeleton(current_mesh, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }));
			}

			if (settings::visuals::distance)
			{
				float BoxHeight = vRootBox.y - Headbox1.y;
				float BoxWidth = BoxHeight / 1.9f;
				std::string text_distance = "(" + std::to_string(player_distance) + "m)";
				ImVec2 text_size = ImGui::CalcTextSize(text_distance.c_str());
				DrawNormalText(font::FN, ImVec2(Headbox1.x - (BoxWidth / 2), (Headbox1.y + 40) - (text_size.x / 2)), ImColor(255, 255, 255), text_distance.c_str());
			}

			if (settings::visuals::cornered_box)
			{
				if (settings::visuals::vischeck)
				{
					if (IsVisible(current_mesh))
					{
						color = ImColor(255, 0, 255, 255);
						draw_cornered_box(Headbox1.x - (corner_width / 2), Headbox1.y, corner_width, corner_height, ImColor({ settings::visuals::VisCOlor[0], settings::visuals::VisCOlor[1], settings::visuals::VisCOlor[2], settings::visuals::VisCOlor[3] }), settings::visuals::boxwidth, 0);
					}
					else
					{
						color = ImColor(255, 140, 0, 255);
						draw_cornered_box(Headbox1.x - (corner_width / 2), Headbox1.y, corner_width, corner_height, ImColor({ settings::visuals::NOtVIs[0], settings::visuals::NOtVIs[1], settings::visuals::NOtVIs[2], settings::visuals::NOtVIs[3] }), settings::visuals::boxwidth, 0);
					}
				}
				else
					draw_cornered_box(Headbox1.x - (corner_width / 2), Headbox1.y, corner_width, corner_height, ImColor({ settings::visuals::box_color[0], settings::visuals::box_color[1], settings::visuals::box_color[2], settings::visuals::box_color[3] }), settings::visuals::boxwidth, 0);

				if (settings::visuals::niggaboxfilled) {
					DrawFilledRect(Headbox1.x - (corner_width / 2), Headbox1.y, corner_width, corner_height, ImColor(10, 10, 10, 100));
				}
			}
		

			if (settings::visuals::line)
			{
				if (settings::visuals::vischeck)
				{
					if (IsVisible(current_mesh))
					{
						color = ImColor(255, 0, 255, 255);
						draw_line(bottom2d, ImColor(255, 255, 255), settings::visuals::boxwidth);
					}
					else
					{
						color = ImColor(255, 140, 0, 255);
						draw_line(bottom2d, ImColor(255, 255, 255), settings::visuals::boxwidth);
					}
				}
				else

					draw_line(bottom2d, ImColor({ settings::visuals::box_color[0], settings::visuals::box_color[1], settings::visuals::box_color[2], settings::visuals::box_color[3] }), settings::visuals::boxwidth);
			}
		}


		if (settings::aimbot::aimbot12)
		{
			if (GetAsyncKeyState(VK_SHIFT))
			{
				aimbot1(pointer::closest_pawn, pointer::closest_distance);
			}
		}
	}
}

WPARAM render_loop()
{
	static RECT old_rc;
	ZeroMemory(&messager, sizeof(MSG));
	while (messager.message != WM_QUIT)
	{
		if (PeekMessage(&messager, win, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&messager);
			DispatchMessage(&messager);
		}
		HWND hwnd_active = GetForegroundWindow();
		if (GetAsyncKeyState(VK_END) & 1)
			exit(8);
		if (hwnd_active == game_wnd)
		{
			HWND hwnd_test = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(win, hwnd_test, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		RECT rc;
		POINT xy;
		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(game_wnd, &rc);
		ClientToScreen(game_wnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;
		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = game_wnd;
		io.DeltaTime = 1.0f / 60.0f;
		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;
		if (GetAsyncKeyState(0x1))
		{
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
		{
			io.MouseDown[0] = false;
		}
		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{
			old_rc = rc;
			settings::width = rc.right;
			settings::height = rc.bottom;
			p_params.BackBufferWidth = settings::width;
			p_params.BackBufferHeight = settings::height;
			SetWindowPos(win, (HWND)0, xy.x, xy.y, settings::width, settings::height, SWP_NOREDRAW);
			p_device->Reset(&p_params);
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		game_loop();

		ImGui::EndFrame();

		p_device->SetRenderState(D3DRS_ZENABLE, false);
		p_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		p_device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		p_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		if (p_device->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			p_device->EndScene();
		}
		HRESULT result = p_device->Present(NULL, NULL, NULL, NULL);
		if (result == D3DERR_DEVICELOST && p_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_device->Reset(&p_params);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	cleanup_d3d();
	DestroyWindow(win);
	return messager.wParam;
}