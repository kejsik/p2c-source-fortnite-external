#pragma once

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

extern ImFont* bold;
extern ImFont* medium;

using namespace ImGui;

namespace elements { // so much easier than adding/modifiying current imgui functions :/
    bool tab(const char* icon, bool boolean);
    bool subtab(const char* name, bool boolean);
    bool begin_child_ex(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags);
    bool begin_child(const char* str_id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    bool begin_child(ImGuiID id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    void end_child();
    void color_picker(const char* name, float color[3], float alpha);
}

struct tab_element {
    float element_opacity, rect_opacity, text_opacity;
};

struct subtab_element {
    float element_opacity, rect_opacity, text_opacity;
};
