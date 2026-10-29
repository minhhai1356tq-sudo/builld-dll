#include "etc_elements.h"

bool e_elements::begin_child_ex(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags)
{

        ImGuiContext& g = *GImGui;
        ImGuiWindow* parent_window = g.CurrentWindow;

        flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
        flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);  // Inherit the NoMove flag

        // Size
        const ImVec2 content_avail = ImGui::GetContentRegionAvail();
        ImVec2 size = ImFloor(size_arg);
        const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
        if (size.x <= 0.0f)
            size.x = ImMax(content_avail.x + size.x, 4.0f); // Arbitrary minimum child size (0.0f causing too much issues)
        if (size.y <= 0.0f)
            size.y = ImMax(content_avail.y + size.y, 4.0f);

        ImGui::SetNextWindowPos(ImVec2(parent_window->DC.CursorPos.x, parent_window->DC.CursorPos.y + 32.f));
        ImGui::SetNextWindowSize(size - ImVec2(0, 37));

        parent_window->DrawList->AddRectFilled(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x, 32), ImGui::GetColorU32(ImGuiCol_TitleBg), 3.f, ImDrawFlags_RoundCornersTop);
        parent_window->DrawList->AddRectFilled(parent_window->DC.CursorPos + ImVec2(0, size.y - 35), parent_window->DC.CursorPos + ImVec2(size.x, size.y), ImColor(15, 15, 15, 0), 3.f, ImDrawFlags_RoundCornersBottom);

        parent_window->DrawList->AddRectFilled(parent_window->DC.CursorPos + ImVec2(0, 32), parent_window->DC.CursorPos + ImVec2(size.x, size.y), ImColor(15, 15, 15, 0), 3.f, ImDrawFlags_RoundCornersTop);

        parent_window->DrawList->AddRect(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x, size.y), ImColor(24, 24, 24, 255), 3.f);

        parent_window->DrawList->AddLine(parent_window->DC.CursorPos + ImVec2(0, 32), parent_window->DC.CursorPos + ImVec2(size.x, 32), ImColor(24, 24, 24, 255));
        
        parent_window->DrawList->AddText(parent_window->DC.CursorPos + ImVec2(10, 7), ImGui::GetColorU32(ImGuiCol_Text), name);


        // Build up name. If you need to append to a same child from multiple location in the ID stack, use BeginChild(ImGuiID id) with a stable value.
        if (name)
            ImFormatString(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), "%s/%s_%08X", parent_window->Name, name, id);
        else
            ImFormatString(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), "%s/%08X", parent_window->Name, id);

        const float backup_border_size = g.Style.ChildBorderSize;
        if (!border)
            g.Style.ChildBorderSize = 0.0f;
        bool ret = ImGui::Begin(g.TempBuffer, NULL, flags);
        g.Style.ChildBorderSize = backup_border_size;

        ImGuiWindow* child_window = g.CurrentWindow;
        child_window->ChildId = id;
        child_window->AutoFitChildAxises = (ImS8)auto_fit_axises;



        // Set the cursor to handle case where the user called SetNextWindowPos()+BeginChild() manually.
        // While this is not really documented/defined, it seems that the expected thing to do.
        if (child_window->BeginCount == 1)
            parent_window->DC.CursorPos = child_window->Pos;

        // Process navigation-in immediately so NavInit can run on first frame
        if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavHasScroll))
        {
            ImGui::FocusWindow(child_window);
            ImGui::NavInitWindow(child_window, false);
            ImGui::SetActiveID(id + 1, child_window); // Steal ActiveId with another arbitrary id so that key-press won't activate child item
            g.ActiveIdSource = ImGuiInputSource_Nav;
        }
        return ret;
    
}

bool e_elements::begin_child(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    return e_elements::begin_child_ex(str_id, window->GetID(str_id), size_arg, border, extra_flags);
}

bool e_elements::begin_child(ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    IM_ASSERT(id != 0);
    return e_elements::begin_child_ex(NULL, id, size_arg, border, extra_flags);
}