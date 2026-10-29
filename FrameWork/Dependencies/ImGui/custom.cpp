#include "custom.hpp"

#include "imgui_tricks.hpp"
#include <map>
#include "color.h"
#include <random>

std::vector<ImVec2> circles_pos;
std::vector<ImVec2> circles_dir;
std::vector<int> circles_radius;

using namespace ImGui;


struct selection_anim
{
    int hovered_anim;
    int active_anim;
};

struct tab_element {
    float element_opacity, rect_opacity, text_opacity;
};

bool Custom::tab(const char* name, bool boolean)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = tab_icons->CalcTextSizeA(15.0f, FLT_MAX, 0.0f, name);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + 31, pos.y + 31));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y + 5), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, tab_element> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f, 0.0f, 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.element_opacity = ImLerp(it_anim->second.element_opacity, (boolean ? 0.04f : hovered ? 0.01f : 0.0f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.rect_opacity = ImLerp(it_anim->second.rect_opacity, (boolean ? 1.0f : 0.0f), 0.15f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.text_opacity = ImLerp(it_anim->second.text_opacity, (boolean ? 1.0f : hovered ? 0.5f : 0.3f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(0.0f, 0.0f, 0.0f, it_anim->second.element_opacity), 3.0f);
    window->DrawList->AddText(tab_icons, 15.0f, (rect.Min + rect.Max - label_size) / 2, ImColor(0.0f, 0.0f, 0.0f, it_anim->second.text_opacity), name);

    return pressed;
}


struct tab_elementt {
    float element_opacity;
};

bool Custom::Tabb(const char* name, bool boolean)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = ImGui::CalcTextSize(name, NULL, true);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + label_size.x, pos.y + label_size.y));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x + 10, rect.Max.y), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, tab_elementt> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.element_opacity = ImLerp(it_anim->second.element_opacity, (boolean ? 0.8f : hovered ? 0.6f : 0.4f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddText(rect.Min, ImColor(1.0f, 1.0f, 1.0f, it_anim->second.element_opacity), name);

    return pressed;
}
bool Custom::Selection(const char* label, ImTextureID user_texture_id, bool selected)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, pos + ImVec2(50, 50));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y + 5), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, selection_anim> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end())
    {
        anim.insert({ id, {0, 0} });
        it_anim = anim.find(id);
    }

    if (hovered)
        it_anim->second.hovered_anim += 8 * (1.f - ImGui::GetIO().DeltaTime);
    else
        it_anim->second.hovered_anim -= 8 * (1.f - ImGui::GetIO().DeltaTime);

    if (it_anim->second.hovered_anim > 50)
        it_anim->second.hovered_anim = 50;
    else if (it_anim->second.hovered_anim < 0)
        it_anim->second.hovered_anim = 0;

    if (selected)
        it_anim->second.active_anim += 16 * (1.f - ImGui::GetIO().DeltaTime);
    else
        it_anim->second.active_anim -= 16 * (1.f - ImGui::GetIO().DeltaTime);

    if (it_anim->second.active_anim > 155)
        it_anim->second.active_anim = 155;
    else if (it_anim->second.active_anim < 0)
        it_anim->second.active_anim = 0;

    //int hovered_anim = animate(label, "hovered_anim", hovered, 50, 8, STATIC);

    //int active_anim = animate(label, "active_anim", selected, 155, 16, STATIC);

    window->DrawList->AddImage(user_texture_id, rect.Min + ImVec2(15, 15), rect.Max - ImVec2(15, 15), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 100 + it_anim->second.hovered_anim + it_anim->second.active_anim));

    return pressed;
}

#define to_vec4( r, g, b, a ) ImColor( r / 255.f, g / 255.f, b / 255.f, a )

int accent_color[4] = { 255, 0, 0, 255 };
ImColor get_accent_color(float a = 1.f) {

    return to_vec4(accent_color[0], accent_color[1], accent_color[2], a);
}

///////////////////////////////////////////// GRADIENT

inline ImVec4 checkbox_gradient_start = ImColor(30, 30, 30, 255);
inline ImVec4 checkbox_gradient_end = ImColor(30, 30, 30, 255);

///////////////////////////////////////////// CHECKBOXES

inline ImVec4 checkbox_bg = ImColor(30, 30, 30, 255);
inline ImVec4 checkbox_mark = ImColor(255, 255, 255);

///////////////////////////////////////////// TEXT

inline ImVec4 text_active = ImColor(255, 255, 255, 255);
inline ImVec4 text_hov = ImColor(255, 255, 255);
inline ImVec4 text = ImColor(255, 255, 255, 255);

struct checkbox_state {

    ImVec4 gradient_color0, gradient_color1;

    ImVec4 text_color, checkmark_color, background_color;

    float rg;
};

bool Custom::Checkbox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = GetFrameHeight() + 1;
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
    {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (IsItemClicked())
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    static std::map<ImGuiID, checkbox_state> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, checkbox_state() });
        it_anim = anim.find(id);
    }

    it_anim->second.gradient_color0 = ImLerp(it_anim->second.gradient_color0, *v ? checkbox_gradient_start : hovered ? ColorConvertU32ToFloat4(ImColor(0, 0, 0, 0)) : ColorConvertU32ToFloat4(ImColor(0, 0, 0, 0)), g.IO.DeltaTime * 6.f);
    it_anim->second.gradient_color1 = ImLerp(it_anim->second.gradient_color1, *v ? checkbox_gradient_end : hovered ? ColorConvertU32ToFloat4(ImColor(0, 0, 0, 0)) : ColorConvertU32ToFloat4(ImColor(0, 0, 0, 0)), g.IO.DeltaTime * 6.f);
    it_anim->second.checkmark_color = ImLerp(it_anim->second.checkmark_color, *v ? checkbox_mark : (hovered ? ColorConvertU32ToFloat4(ImColor(0, 0, 0, 0)) : ColorConvertU32ToFloat4(ImColor(0, 0, 0, 0))), g.IO.DeltaTime * 9.f);
    it_anim->second.rg = ImLerp(it_anim->second.rg, *v ? 0.f : 10.f, g.IO.DeltaTime * 5.f);


    it_anim->second.text_color = ImLerp(it_anim->second.text_color, *v ? text_active : hovered ? text_hov : text, g.IO.DeltaTime * 6.f);

    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
    RenderNavHighlight(total_bb, id);

    RenderFrame(check_bb.Min, check_bb.Max, GetColorU32(checkbox_bg), true, style.FrameRounding);

    const int vtx_idx_1 = ImGui::GetWindowDrawList()->VtxBuffer.Size;
    window->DrawList->AddRectFilled(check_bb.Min + ImVec2(it_anim->second.rg, it_anim->second.rg), check_bb.Max - ImVec2(it_anim->second.rg, it_anim->second.rg), GetColorU32(it_anim->second.gradient_color0), style.FrameRounding);
    ImGui::ShadeVertsLinearColorGradientKeepAlpha(ImGui::GetWindowDrawList(), vtx_idx_1, ImGui::GetWindowDrawList()->VtxBuffer.Size, check_bb.Min, check_bb.Max, GetColorU32(it_anim->second.gradient_color1), GetColorU32(it_anim->second.gradient_color0));

    const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
    RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad + 3, pad + 3 + it_anim->second.rg), GetColorU32(it_anim->second.checkmark_color), square_sz - pad * 4.0f);

    ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);

    window->DrawList->AddText(label_pos, GetColorU32(it_anim->second.text_color), label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}

void draw_circle(ImVec2 pos, int radius, ImU32 color)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    unsigned char* color_ptr = (unsigned char*)&color;
    draw_list->AddCircleFilled(pos, radius - 1, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], color_ptr[3]));
}

void draw_line(ImVec2 pos1, ImVec2 pos2, ImU32 color, int radius)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float distance = std::sqrt(
        std::pow(pos2.x - pos1.x, 2) + // std:: pow
        std::pow(pos2.y - pos1.y, 2)    // std:: pow
    );
    float alpha;
    if (distance <= 20.0f) {
        alpha = 255.0f;
    }
    else {
        alpha = (1.0f - ((distance - 20.0f) / 25.0f)) * 255.0f;
    }

    int r = (color & 0xFF0000) >> 16; // Extract red component
    int g = (color & 0x00FF00) >> 8;  // Extract green component
    int b = (color & 0x0000FF);       // Extract blue component

    unsigned char* color_ptr = (unsigned char*)&color;

    draw_list->AddLine(pos1, pos2, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], static_cast<int>(alpha)), 1.0f);
    if (distance >= 40.0f) {
        draw_list->AddCircleFilled(pos1, radius - 0.96f, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], alpha * 200.0f));
        draw_list->AddCircleFilled(pos2, radius - 0.96f, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], alpha * 200.0f));
    }
    else if (distance <= 20.0f) {
        draw_list->AddCircleFilled(pos1, radius, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], alpha * 200.0f));
        draw_list->AddCircleFilled(pos2, radius, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], alpha * 200.0f));
    }
    else {
        float radius_factor = 1.0f - ((distance - 20.0f) / 20.0f);
        float offset_factor = 1.0f - radius_factor;
        float offset = (radius - radius * radius_factor) * offset_factor;
        draw_list->AddCircleFilled(pos1, radius - offset, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], alpha * 200.0f));
        draw_list->AddCircleFilled(pos2, radius - offset, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], alpha * 200.0f));
    }
}

void move_circles(ImVec2 aaaa)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 360);

    ImVec2 window_size = ImGui::GetWindowSize();

    for (int i = 0; i < circles_pos.size(); i++)
    {
        ImVec2& pos = circles_pos[i];
        ImVec2& dir = circles_dir[i];
        int radius = circles_radius[i];

        pos.x += dir.x * 0.6;
        pos.y += dir.y * 0.6;

        if (pos.x - radius < 0 || pos.x + radius > aaaa.x + window_size.x)
        {
            dir.x = -dir.x;
            dir.y = dis(gen) % 2 == 0 ? -1 : 1;
        }

        if (pos.y - radius < 0 || pos.y + radius >  aaaa.y + window_size.y)
        {
            dir.y = -dir.y;
            dir.x = dis(gen) % 2 == 0 ? -1 : 1;
        }
    }
}

void Custom::draw_circles_and_lines(ImU32 color, ImVec2 aaa)
{
    move_circles(aaa);

    for (int i = 0; i < circles_pos.size(); i++)
    {
        draw_circle(circles_pos[i], circles_radius[i], color);

        for (int j = i + 1; j < circles_pos.size(); j++)
        {
            float distance = ImGui::GetIO().FontGlobalScale * std::sqrt(
                std::pow(circles_pos[j].x - circles_pos[i].x, 2) + // std:: pow
                std::pow(circles_pos[j].y - circles_pos[i].y, 2)    // std:: pow
            );

            if (distance <= 45.0f)
            {
                draw_line(circles_pos[i], circles_pos[j], color, circles_radius[i]);
            }
        }
    }
}

void Custom::setup_circles()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    std::uniform_int_distribution<> pos_dis(0, static_cast<int>(1920));
    std::uniform_int_distribution<> pos_dis_y(0, static_cast<int>(1080));

    for (int i = 0; i < 400; i++) // 1000 is the number of cricle you render i recommend 400.
    {
        circles_pos.push_back(ImVec2(pos_dis(gen), pos_dis_y(gen)));
        circles_dir.push_back(ImVec2(dis(gen) == 0 ? -1 : 1, dis(gen) == 0 ? -1 : 1));
        circles_radius.push_back(3);
    }
}