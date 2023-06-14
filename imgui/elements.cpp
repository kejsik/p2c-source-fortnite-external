#include "elements.h"


//bool elements::tab(const char* name, bool boolean)
//{
//    ImGuiWindow* window = ImGui::GetCurrentWindow();
//    if (window->SkipItems)
//        return false;
//
//    ImGuiContext& g = *GImGui;
//    const ImGuiStyle& style = g.Style;
//    const ImGuiID id = window->GetID(name);
//    const ImVec2 label_size = medium->CalcTextSizeA(15.0f, FLT_MAX, 0.0f, name);
//    ImVec2 pos = window->DC.CursorPos;
//
//    const ImRect rect(pos, ImVec2(pos.x + 43, pos.y + 31));
//    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y + 5), style.FramePadding.y);
//    if (!ImGui::ItemAdd(rect, id))
//        return false;
//
//    bool hovered, held;
//    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);
//
//    static std::map <ImGuiID, tab_element> anim;
//    auto it_anim = anim.find(id);
//    if (it_anim == anim.end()) {
//        anim.insert({ id, { 0.0f, 0.0f, 0.0f } });
//        it_anim = anim.find(id);
//    }
//
//    it_anim->second.element_opacity = ImLerp(it_anim->second.element_opacity, (boolean ? 0.04f : hovered ? 0.01f : 0.0f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));
//    it_anim->second.rect_opacity = ImLerp(it_anim->second.rect_opacity, (boolean ? 1.0f : 0.0f), 0.15f * (1.0f - ImGui::GetIO().DeltaTime));
//    it_anim->second.text_opacity = ImLerp(it_anim->second.text_opacity, (boolean ? 1.0f : hovered ? 0.5f : 0.3f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));
//
//    window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(1.0f, 1.0f, 1.0f, it_anim->second.element_opacity), 3.0f);
//    window->DrawList->AddText(medium, 15.0f, (rect.Min + rect.Max - label_size) / 2, ImColor(1.0f, 1.0f, 1.0f, it_anim->second.text_opacity), name);
//
//    window->DrawList->AddLine(ImVec2(rect.Max.x, rect.Min.y + 3), ImVec2(rect.Max.x, rect.Max.y - 3), ImColor(10 / 255.0f, 10 / 255.0f, 255 / 255.0f, it_anim->second.rect_opacity), 2.0f);
//
//    return pressed;
//}
//
//bool elements::subtab(const char* name, bool boolean)
//{
//    ImGuiWindow* window = ImGui::GetCurrentWindow();
//    if (window->SkipItems)
//        return false;
//
//    ImGuiContext& g = *GImGui;
//    const ImGuiStyle& style = g.Style;
//    const ImGuiID id = window->GetID(name);
//    const ImVec2 label_size = ImGui::CalcTextSize(name);
//    ImVec2 pos = window->DC.CursorPos;
//
//    const ImRect rect(pos, ImVec2(pos.x + 145, pos.y + 32));
//    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y + 3), style.FramePadding.y);
//    if (!ImGui::ItemAdd(rect, id))
//        return false;
//
//    bool hovered, held;
//    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);
//
//    static std::map <ImGuiID, subtab_element> anim;
//    auto it_anim = anim.find(id);
//    if (it_anim == anim.end()) {
//        anim.insert({ id, { 0.0f, 0.0f, 0.0f } });
//        it_anim = anim.find(id);
//    }
//
//    it_anim->second.element_opacity = ImLerp(it_anim->second.element_opacity, (boolean ? 0.04f : 0.0f), 0.09f * (1.0f - ImGui::GetIO().DeltaTime));
//    it_anim->second.rect_opacity = ImLerp(it_anim->second.rect_opacity, (boolean ? 1.0f : 0.0f), 0.20f * (1.0f - ImGui::GetIO().DeltaTime));
//    it_anim->second.text_opacity = ImLerp(it_anim->second.text_opacity, (boolean ? 1.0f : 0.3f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));
//
//    window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(1.0f, 1.0f, 1.0f, it_anim->second.element_opacity), 3.0f);
//    window->DrawList->AddText(ImVec2(rect.Min.x + 15, (rect.Min.y + rect.Max.y) / 2 - label_size.y / 2), ImColor(1.0f, 1.0f, 1.0f, it_anim->second.text_opacity), name);
//
//    window->DrawList->AddRectFilled(ImVec2(rect.Max.x + 5, rect.Min.y + 9), ImVec2(rect.Max.x + 8, rect.Max.y - 9), ImColor(10 / 255.0f, 10 / 255.0f, 255 / 255.0f, it_anim->second.rect_opacity), 10.0f, ImDrawFlags_RoundCornersLeft);
//
//    return pressed;
//}
//
//bool elements::begin_child_ex(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags)
//{
//    ImGuiContext& g = *GImGui;
//    ImGuiWindow* parent_window = g.CurrentWindow;
//
//    flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
//    flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);  // Inherit the NoMove flag
//
//    // Size
//    const ImVec2 content_avail = GetContentRegionAvail();
//    ImVec2 size = ImFloor(size_arg);
//    const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
//    if (size.x <= 0.0f)
//        size.x = ImMax(content_avail.x + size.x, 4.0f); // Arbitrary minimum child size (0.0f causing too much issues)
//    if (size.y <= 0.0f)
//        size.y = ImMax(content_avail.y + size.y, 4.0f);
//
//    SetNextWindowPos(ImVec2(parent_window->DC.CursorPos.x, parent_window->DC.CursorPos.y + 34.0f));
//    SetNextWindowSize(size - ImVec2(0, 36));
//
//    parent_window->DrawList->AddRectFilled(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x, size.y), ImColor(24, 24, 26), 4.0f);
//    parent_window->DrawList->AddRect(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x, size.y), ImColor(1.0f, 1.0f, 1.0f, 0.03f), 4.0f);
//    parent_window->DrawList->AddLine(parent_window->DC.CursorPos + ImVec2(1, 18), parent_window->DC.CursorPos + ImVec2(size.x - 1, 18), ImColor(1.0f, 1.0f, 1.0f, 0.03f));
//    //parent_window->DrawList->AddRectFilledMultiColorRounded(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x - 115, 32), ImColor(24, 24, 26), ImColor(190, 66, 66, 50), ImColor(190, 66, 66, 0), ImColor(207, 56, 56, 0), ImColor(207, 56, 56, 50), 4.0f, ImDrawCornerFlags_TopLeft);
//    parent_window->DrawList->AddText(bold, 15.0f, parent_window->DC.CursorPos + ImVec2(8, 1), ImColor(10, 10, 255), name);
//
//    // Build up name. If you need to append to a same child from multiple location in the ID stack, use BeginChild(ImGuiID id) with a stable value.
//    const char* temp_window_name;
//    if (name)
//        ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id);
//    else
//        ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%08X", parent_window->Name, id);
//
//    const float backup_border_size = g.Style.ChildBorderSize;
//    if (!border)
//        g.Style.ChildBorderSize = 0.0f;
//    bool ret = Begin(temp_window_name, NULL, flags);
//    g.Style.ChildBorderSize = backup_border_size;
//
//    ImGuiWindow* child_window = g.CurrentWindow;
//    child_window->ChildId = id;
//    child_window->AutoFitChildAxises = (ImS8)auto_fit_axises;
//
//    // Set the cursor to handle case where the user called SetNextWindowPos()+BeginChild() manually.
//    // While this is not really documented/defined, it seems that the expected thing to do.
//    if (child_window->BeginCount == 1)
//        parent_window->DC.CursorPos = child_window->Pos;
//
//    // Process navigation-in immediately so NavInit can run on first frame
//    if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavHasScroll))
//    {
//        FocusWindow(child_window);
//        NavInitWindow(child_window, false);
//        SetActiveID(id + 1, child_window); // Steal ActiveId with another arbitrary id so that key-press won't activate child item
//        g.ActiveIdSource = ImGuiInputSource_Nav;
//    }
//    return ret;
//}
//
//bool elements::begin_child(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
//{
//    ImGuiWindow* window = GetCurrentWindow();
//    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 11));
//    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
//    return begin_child_ex(str_id, window->GetID(str_id), size_arg, border, extra_flags | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoMove);
//}
//
//bool elements::begin_child(ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
//{
//    IM_ASSERT(id != 0);
//    return begin_child_ex(NULL, id, size_arg, border, extra_flags);
//}
//
//void elements::end_child()
//{
//    ImGuiContext& g = *GImGui;
//    ImGuiWindow* window = g.CurrentWindow;
//
//    IM_ASSERT(g.WithinEndChild == false);
//    IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);   // Mismatched BeginChild()/EndChild() calls
//
//    g.WithinEndChild = true;
//    if (window->BeginCount > 1)
//    {
//        End();
//    }
//    else
//    {
//        ImVec2 sz = window->Size;
//        if (window->AutoFitChildAxises & (1 << ImGuiAxis_X)) // Arbitrary minimum zero-ish child size of 4.0f causes less trouble than a 0.0f
//            sz.x = ImMax(4.0f, sz.x);
//        if (window->AutoFitChildAxises & (1 << ImGuiAxis_Y))
//            sz.y = ImMax(4.0f, sz.y);
//        End();
//
//        ImGuiWindow* parent_window = g.CurrentWindow;
//        ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
//        ItemSize(sz);
//        if ((window->DC.NavLayersActiveMask != 0 || window->DC.NavHasScroll) && !(window->Flags & ImGuiWindowFlags_NavFlattened))
//        {
//            ItemAdd(bb, window->ChildId);
//            RenderNavHighlight(bb, window->ChildId);
//
//            // When browsing a window that has no activable items (scroll only) we keep a highlight on the child (pass g.NavId to trick into always displaying)
//            if (window->DC.NavLayersActiveMask == 0 && window == g.NavWindow)
//                RenderNavHighlight(ImRect(bb.Min - ImVec2(2, 2), bb.Max + ImVec2(2, 2)), g.NavId, ImGuiNavHighlightFlags_TypeThin);
//        }
//        else
//        {
//            // Not navigable into
//            ItemAdd(bb, 0);
//        }
//        if (g.HoveredWindow == window)
//            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
//    }
//    ImGui::PopStyleVar(2);
//    g.WithinEndChild = false;
//    g.LogLinePosY = -FLT_MAX; // To enforce a carriage return
//}

void elements::color_picker(const char* name, float color[3], float alpha = 1.0f)
{
    ImGui::TextUnformatted(name);
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 10);

    ImGui::PushID(name);
    bool openPopup = ImGui::ColorButton("##btn", ImVec4{ color[0], color[1], color[2], alpha }, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_AlphaPreview);
    if (ImGui::BeginDragDropTarget()) {

        if (const auto payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
            std::copy((float*)payload->Data, (float*)payload->Data + 3, color);
        if (const auto payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
            std::copy((float*)payload->Data, (float*)payload->Data + 3, color);

        ImGui::EndDragDropTarget();
    }

    if (openPopup)
        ImGui::OpenPopup("##popup");

    if (ImGui::BeginPopup("##popup", ImGuiWindowFlags_NoMove)) {
        ImGui::PushItemWidth(120.0f); // default size too big and looks ugly
        ImGui::ColorPicker3("##picker", color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
        ImGui::EndPopup();
    }
    ImGui::PopID();
}
