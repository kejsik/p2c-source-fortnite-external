#include <d3d9.h>
#include <tchar.h>
#include <stdexcept>


#include "gui.h"
#include "../Xor/xor.hpp"
#include "../string.hpp"

#include "../Includes/settings.hpp"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imanigger.h"

#define MENU_TAB_AIMBOT 1
#define MENU_TAB_VISUAL 2
#define MENU_TAB_RADAR 3
#define MENU_TAB_COLOR 4
#define MENU_TAB_MISC 5
#define MENU_TAB_SETTINGS 6

#define WEAPON_TAB_DEFAULT 0
#define WEAPON_TAB_RIFLE 1
#define WEAPON_TAB_SHOTGUN 2
#define WEAPON_TAB_SMG 3
#define WEAPON_TAB_PISTOL 4
#define WEAPON_TAB_SNIPER 5

#define MENU_SETTINGS_SETTINGS 1


namespace menu {
	int tab = MENU_TAB_AIMBOT;
	int weapon_tab = WEAPON_TAB_DEFAULT;
	int settings_page = MENU_SETTINGS_SETTINGS;
}

namespace image {
	IDirect3DTexture9* scene = nullptr;
	bool style = false;
}

namespace ImGui
{
	IMGUI_API bool Tab(unsigned int index, const char* label, int* selected, float width = 60, float height = 23)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = ImGui::GetStyle().Colors;
		ImColor color = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
		ImColor colorActive = ImColor(79, 79, 81, 255); /*style.Colors[ImGuiCol_ButtonActive]*/;
		ImColor colorHover = ImColor(62, 62, 66, 255)/*style.Colors[ImGuiCol_ButtonHovered]*/;


		if (index > 0)
			ImGui::SameLine();

		if (index == *selected)
		{
			style.Colors[ImGuiCol_Button] = colorActive;
			style.Colors[ImGuiCol_ButtonActive] = colorActive;
			style.Colors[ImGuiCol_ButtonHovered] = colorActive;
		}
		else
		{
			style.Colors[ImGuiCol_Button] = color;
			style.Colors[ImGuiCol_ButtonActive] = colorActive;
			style.Colors[ImGuiCol_ButtonHovered] = colorHover;
		}

		if (ImGui::Button(label, ImVec2(width, height)))
			*selected = index;

		style.Colors[ImGuiCol_Button] = color;
		style.Colors[ImGuiCol_ButtonActive] = colorActive;
		style.Colors[ImGuiCol_ButtonHovered] = colorHover;

		return *selected == index; 
	}
}



void gui::render(/*LPDIRECT3DDEVICE9 D3dDevice*/)
{
	if (GetAsyncKeyState(VK_INSERT) & 1) gui::is_open = !gui::is_open;

	if (!gui::is_setup || !gui::is_open) return; // don't really need this but i like sanity checks

	static bool i_open = true;
	static int m_size_x = 670;
	static int m_size_y = 500;
	static int m_pos_x;
	static int m_pos_y;
	ImVec4* m_colors = ImGui::GetStyle().Colors;
	auto& m_style = ImGui::GetStyle();
	ImGuiIO& m_io = ImGui::GetIO(); (void)m_io;
	auto menu_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
	auto m_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration;
	auto tab_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
	auto child_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
	auto Header_Flags = ImGuiTreeNodeFlags_NoTreePushOnOpen;
	auto color_flags = ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs;
	int MenuTab;
	static const char* combo_items[3] = { "Head", "Neck", "Boobs" };
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
	colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
	colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

	ImGui::SetNextWindowSize({ 759.f,459.f }, ImGuiWindowFlags_NoResize);
	ImGui::Begin("Thrilla", &i_open, menu_flags);


	ImGui::Begin("Thrilla", &i_open, menu_flags);
	ImGui::SetCursorPos({ 4.f,20.f });
	if (ImGui::Button("Aim", { 132.f,46.f }))
		menu::tab = MENU_TAB_AIMBOT;
	{

	}
	ImGui::SetCursorPos({ 154.f,20.f });
	if (ImGui::Button("Visuals", { 132.f,46.f }))
		menu::tab = MENU_TAB_VISUAL;
	{

	}
	ImGui::SetCursorPos({ 308.f,20.f });
	if (ImGui::Button("Color", { 132.f,46.f }))
		menu::tab = MENU_TAB_COLOR;
	{

	}
	ImGui::SetCursorPos({ 462.f,20.f });
	if (ImGui::Button("Misc", { 135.f,46.f }))
		menu::tab = MENU_TAB_SETTINGS;
	{

	}if (menu::tab == MENU_TAB_AIMBOT) {
		ImGui::SetCursorPos({ 9.f,75.f });
		ImGui::Checkbox("Enable aimbot", &settings::aimbot::aimbot12);
		ImGui::SetCursorPos({ 9.f,100.f });
		ImGui::Checkbox("Triggerbot", &settings::aimbot::bTargetedBuild);
		ImGui::SetCursorPos({ 9.f,131.f });
		ImGui::Checkbox("Show FOV", &settings::aimbot::show_fov);
		ImGui::SetCursorPos({ 8.f,159.f });
		ImGui::PushItemWidth(496.000000);
		ImGui::SliderInt("Aim FOV", &settings::aimbot::fov, 1, 500);
		ImGui::PopItemWidth();
		ImGui::SetCursorPos({ 8.f,187.f });
		ImGui::PushItemWidth(482.000000);
		ImGui::SliderInt("Smooth", &settings::aimbot::smoothness, 2, 20);
		ImGui::SetCursorPos({ 8.f,212.f });
		ImGui::Combo(("Hitbox"), &settings::aimbot::hitbox, combo_items, IM_ARRAYSIZE(combo_items));
		ImGui::SetCursorPos({ 8.f,237.f });
		ImGui::Checkbox("Skip Knocked", &settings::aimbot::bIgnoreDead);


		ImGui::PopItemWidth();
	}
	if (menu::tab == MENU_TAB_VISUAL) {

		ImGui::SetCursorPos({ 9.f,75.f });
		ImGui::Checkbox("2D Box", &settings::visuals::box);
		ImGui::SetCursorPos({ 9.f,100.f });
		ImGui::Checkbox("Rounding Box", &settings::visuals::roundedboxes);
		ImGui::SetCursorPos({ 9.f,125.f });
		ImGui::Checkbox("Outline", &settings::visuals::Outline);
		ImGui::SetCursorPos({ 9.f,150.f });
		ImGui::Checkbox("Corner Box", &settings::visuals::cornered_box);
		ImGui::SetCursorPos({ 9.f,175.f });
		ImGui::Checkbox("Distance", &settings::visuals::distance);
		ImGui::SetCursorPos({ 9.f,200.f });
		ImGui::Checkbox("Snap lines", &settings::visuals::line);
		ImGui::SetCursorPos({ 9.f,225.f });
		ImGui::Checkbox("Visible Check", &settings::visuals::vischeck);
		ImGui::SetCursorPos({ 9.f,250.f });
		ImGui::Checkbox("Skeleton", &settings::visuals::gayskeleton);
		ImGui::SetCursorPos({ 9.f,275.f });
		ImGui::Checkbox("Show Team", &settings::aimbot::bIgnoreTeam);
		ImGui::SetCursorPos({ 9.f,300.f });
		ImGui::Checkbox("Name Esp", &settings::visuals::nameesp);


		ImGui::EndChild();

	}
	if (menu::tab == MENU_TAB_COLOR) {

		ImGui::Separator();
		ImGui::PushItemWidth(130.0f);
		ImGui::Spacing();
		ImGui::SetCursorPos({ 10.f,75.f });
		ImGui::ColorPicker3(("Visible "), settings::visuals::VisCOlor);
		ImGui::Spacing();
		ImGui::SetCursorPos({ 250.f,75.f });
		ImGui::ColorPicker3(("InVisible "), settings::visuals::NOtVIs);
		ImGui::Spacing();
		ImGui::SetCursorPos({ 500.f,75.f });
		ImGui::ColorPicker3(("Skeleton "), settings::visuals::skeletoncolor);
		ImGui::EndChild();


	}
	if (menu::tab == MENU_TAB_SETTINGS) {

		ImGui::SetCursorPos({ 9.f, 75.f });
		if (ImGui::Button("Discord"))
		{
			system("start https://discord.com/invite/Thrilla ");
		}
		ImGui::SetCursorPos({ 9.f,125.f });
		if (ImGui::Button("YouTube"))
		{
			system("start https://www.youtube.com/channel/UCiZNPyx4zh-WZxRQi-gr1DQ ");
		}
		ImGui::SetCursorPos({ 9.f,150.f });
		ImGui::Checkbox("Stream Proof", &settings::visuals::settingsStreamproof);
		ImGui::SetCursorPos({ 9.f,175.f });
		ImGui::Checkbox("Watermark", &settings::aimbot::renderfps);
		ImGui::SetCursorPos({ 9.f,200.f });
		ImGui::SliderInt("Thickness", &settings::visuals::boxwidth, 1, 5);
		ImGui::EndChild();


	}
	
}

	

