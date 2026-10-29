#pragma once

#include "FrameWork\Dependencies\ImGui\imgui.h"

namespace colors {

    inline ImVec4 accent_color = ImColor(255, 127, 73);

    namespace menu {

        inline ImVec4 window_bg = ImColor(20, 20, 22);
        inline ImVec4 border = ImColor(17, 17, 17, 0);
        inline ImVec4 watermark_bg = ImColor(21, 19, 20, 100);
        inline ImVec4 watermark_border = ImColor(21, 19, 20, 0);
        inline ImVec4 watermark_filled = ImColor(27, 25, 28);

    }

    namespace tabs {

        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_hovered = ImColor(150, 150, 150);
        inline ImVec4 text_inactive = ImColor(76, 76, 77);

    }

    namespace child {

        inline ImVec4 child_background = ImColor(23, 23, 25);
        inline ImVec4 child_top = ImColor(25, 25, 27);
        inline float child_rounding = 7.f;

    }

    namespace checkbox {

        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_hovered = ImColor(150, 150, 150);
        inline ImVec4 text_inactive = ImColor(76, 76, 77);

        inline ImVec4 checkbox_bg_active = ImColor(255, 99, 71);
        inline ImVec4 checkbox_bg_inactive = ImColor(220, 20, 60);

        inline ImVec4 circle_inactive = ImColor(44, 44, 46);

    }

    namespace slider {

        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_hovered = ImColor(150, 150, 150);
        inline ImVec4 text_inactive = ImColor(76, 76, 77);
        inline ImVec4 slider_inactive = ImColor(220, 20, 60);

    }

    namespace combo {


        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_hovered = ImColor(150, 150, 150);
        inline ImVec4 text_inactive = ImColor(76, 76, 77);
        inline ImVec4 combo_bg = ImColor(27, 27, 29);

    }

    namespace color_picker {

        inline ImVec4 picker_bg = ImColor(27, 27, 29);

    }

    namespace binder {

        inline ImVec4 binder_bg = ImColor(27, 27, 29);
        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_hovered = ImColor(150, 150, 150);
        inline ImVec4 text_inactive = ImColor(76, 76, 77);
        inline ImVec4 line = ImColor(40, 40, 42);
        inline ImVec4 image_active = ImColor(255, 255, 255);
        inline ImVec4 image_hovered = ImColor(150, 150, 150);
        inline ImVec4 image_inactive = ImColor(76, 76, 77);

    }

    namespace button {

        inline ImVec4 button_bg = ImColor(27, 27, 29);
        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_inactive = ImColor(76, 76, 77);
    }

    namespace input {

        inline ImVec4 input_bg = ImColor(27, 27, 29);
        inline ImVec4 input_image = ImColor(47, 47, 49);
        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_hovered = ImColor(150, 150, 150);
        inline ImVec4 text_inactive = ImColor(76, 76, 77);
    }

    namespace preview {

        inline ImVec4 rect = ImColor(40, 40, 42);
        inline ImVec4 name = ImColor(255, 255, 255);
        inline ImVec4 distance = ImColor(255, 255, 255);
        inline ImVec4 head = ImColor(255, 255, 255);
        inline ImVec4 dice = ImColor(255, 255, 255);

    }


}

namespace settings {
    inline float checkbox_rounding = 100.f;
}


namespace c
{
    inline ImVec4 accent = ImColor(118, 187, 117);
    inline ImVec4 separator = ImColor(22, 23, 26);

    namespace bg
    {
        inline ImVec4 background = ImColor(15, 15, 15);
        inline ImVec2 size = ImVec2(650, 570);
        inline float rounding = 8.f;
    }

    namespace child
    {
        inline ImVec4 background = ImColor(17, 17, 18);
        inline ImVec4 cap = ImColor(20, 21, 23);
        inline float rounding = 8.f;
    }

    namespace page
    {
        inline ImVec4 background_active = ImColor(31, 33, 38);
        inline ImVec4 background = ImColor(22, 23, 25);

        inline ImVec4 text_hov = ImColor(69, 74, 95);
        inline ImVec4 text = ImColor(68, 71, 85);

        inline float rounding = 4.f;
    }

    namespace elements
    {
        inline ImVec4 background_hovered = ImColor(31, 33, 38);
        inline ImVec4 background = ImColor(22, 23, 25);
        inline float rounding = 2.f;
    }

    namespace checkbox
    {
        inline ImVec4 mark = ImColor(0, 0, 0, 255);
    }

    namespace text
    {
        inline ImVec4 text_active = ImColor(255, 255, 255);
        inline ImVec4 text_hov = ImColor(69, 74, 95);
        inline ImVec4 text = ImColor(68, 71, 85);
    }
}