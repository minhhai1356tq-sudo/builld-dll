#pragma once
#include <ImGui/imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS


inline ImColor main_color(110, 92, 228, 210);
inline ImColor particlescolor(110, 92, 228, 255);

inline ImColor titlecolor(110, 92, 228, 50);
inline ImColor checkback(110, 92, 228, 40);
inline ImColor background_color(9, 9, 9, 130);
inline ImColor alphabg(9, 9, 9, 0);
inline ImColor background_colorX(4, 4, 4, 160);

inline ImColor testg(99, 83, 204, 60);
inline ImColor testgx(99, 84, 204, 0);
inline ImColor second_color(12, 12, 12, 155);
inline ImColor checkborder(110, 92, 228, 25);
inline ImColor tabs(110, 92, 228, 85);
inline ImColor tabsbg(99, 83, 204, 0);
inline ImColor tabshadowx(0, 0, 0, 255);
inline ImColor tabsbgx(0, 0, 0, 0);
inline ImColor tipcolor(230, 40, 40, 255);
inline ImColor tabbgcolor(24, 24, 24, 255);

inline ImColor tabalphabg(1, 1, 1, 100);
inline ImVec2 center_text(ImVec2 min, ImVec2 max, const char* text)
{
	return min + (max - min) / 2 - ImGui::CalcTextSize(text) / 2;
}

inline ImColor text_color[3] = { ImColor(255, 255, 255, 255), ImColor(200, 200, 200, 255), ImColor(150, 150, 150, 255) };

inline float anim_speed = 30.f;

inline ImVec2 frame_size = ImVec2(300, 50);

inline ImFont* default_font;
inline ImFont* second_font;
inline ImFont* big_font;

inline ImFont* icon_font2;
inline ImFont* akirax;
inline ImFont* AkiraBig;
inline ImFont* tabss;
inline ImFont* tabsiconsnew;
inline ImFont* cheatlogo;
inline ImGuiWindow* main_window;
inline char search[64];

inline char login[64];
inline char password[64];


// WIDGET COLORS

namespace x {

	inline ImVec4 button_push_gradient_hoveredXX = ImColor(160, 60, 60, 95);
	inline ImVec4 button_pop_gradient_hoveredXX = ImColor(60, 0, 0, 95);
	///////////////////////////////////////////// TABS

	inline ImVec4 tab_text_active = ImColor(255, 255, 255, 255);
	inline ImVec4 tab_text_hov = ImColor(98, 112, 122, 255);
	inline ImVec4 tab_text = ImColor(88, 92, 112, 255);

	inline ImVec4 tab_active = ImColor(171, 33, 33, 255);
	inline ImVec4 glow_active = ImColor(255, 0, 0, 255);

	///////////////////////////////////////////// TEXT

	inline ImVec4 text_active = ImColor(255, 255, 255, 255);
	inline ImVec4 text_hov = ImColor(255, 255, 255, 255);
	inline ImVec4 text = ImColor(66, 65, 70, 255);

	///////////////////////////////////////////// CHILD

	inline ImVec4 child_background = ImColor(22, 21, 26, 255);
	inline ImVec4 border_child = ImColor(39, 23, 23, 255);
	inline ImVec4 child_gradient = ImColor(255, 0, 0, 255);

	inline ImVec4 border_child_default = ImColor(22, 21, 26, 255);
	inline ImVec4 child_name = ImColor(88, 92, 112, 255);

	///////////////////////////////////////////// GRADIENT

	inline ImVec4 checkbox_gradient_start = ImColor(155, 0, 0, 255);
	inline ImVec4 checkbox_gradient_end = ImColor(230, 0, 0, 255);

	///////////////////////////////////////////// CHECKBOXES

	inline ImVec4 checkbox_bg = ImColor(32, 32, 32, 255);
	inline ImVec4 checkbox_mark = ImColor(110, 92, 228, 245);

	///////////////////////////////////////////// SLIDERS

	inline ImVec4 slider_background = ImColor(17, 16, 21, 255);
	inline ImVec4 circle_push_gradient = ImColor(51, 49, 59, 255);
	inline ImVec4 circle_pop_gradient = ImColor(61, 60, 76, 255);


	///////////////////////////////////////////// INPUTTEXT

	inline ImVec4 input_push_gradient = ImColor(27, 26, 31, 255);
	inline ImVec4 input_pop_gradient = ImColor(17, 16, 21, 255);


	///////////////////////////////////////////// SCROLLBAR

	inline ImVec4 scroll_background = ImColor(15, 17, 20, 255);
	inline ImVec4 scroll_keep = ImColor(36, 35, 40, 255);

	///////////////////////////////////////////// COMBOBOX

	inline ImVec4 combo_background = ImColor(44, 43, 50, 255);

	inline ImVec4 combo_push_gradient = ImColor(0, 222, 219, 255);
	inline ImVec4 combo_pop_gradient = ImColor(17, 16, 21, 255);

	///////////////////////////////////////////// BUTTON

	inline ImVec4 button_push_gradient_active = ImColor(140, 0, 0, 255);
	inline ImVec4 button_pop_gradient_active = ImColor(70, 0, 0, 255);

	inline ImVec4 button_push_gradient_hovered = ImColor(160, 0, 0, 255);
	inline ImVec4 button_pop_gradient_hovered = ImColor(90, 0, 0, 255);
	// INPUT TEXT NEW 
	inline ImVec4 button_push_gradient_activeX = ImColor(40, 40, 40, 255);
	inline ImVec4 button_pop_gradient_activeX = ImColor(20, 20, 20, 255);

	inline ImVec4 button_push_gradient_hoveredX = ImColor(59, 59, 59, 255);
	inline ImVec4 button_pop_gradient_hoveredX = ImColor(43, 43, 43, 255);

	inline ImVec4 button_push_gradient = ImColor(32, 32, 32, 255);
	inline ImVec4 button_pop_gradient = ImColor(19, 19, 19, 255);
	///////////////////////////////////////////////////////////// NEW BUTTON 


	inline ImVec4 button_push_gradient_active1 = ImColor(140, 0, 0, 255);
	inline ImVec4 button_pop_gradient_active1 = ImColor(70, 0, 0, 255);

	inline ImVec4 button_push_gradient_hovered1 = ImColor(160, 0, 0, 255);
	inline ImVec4 button_pop_gradient_hovered1 = ImColor(90, 0, 0, 255);
	// INPUT TEXT NEW 
	inline ImVec4 button_push_gradient_activeX1 = ImColor(40, 40, 40, 255);
	inline ImVec4 button_pop_gradient_activeX1 = ImColor(20, 20, 20, 255);

	inline ImVec4 button_push_gradient_hoveredX1 = ImColor(59, 59, 59, 255);
	inline ImVec4 button_pop_gradient_hoveredX1 = ImColor(43, 43, 43, 255);

	inline ImVec4 button_push_gradient1 = ImColor(22, 22, 22, 255);
	inline ImVec4 button_pop_gradient1 = ImColor(25, 25, 25, 255);


	///////////////////////////////////////////// KEYBIND

	inline ImVec4 keybind_background = ImColor(18, 18, 20, 255);
	inline ImVec4 keybind_border = ImColor(16, 16, 18, 255);

	///////////////////////////////////////////// TAB VISIBLE

	inline ImVec4 tab_push_gradient = ImColor(255, 0, 0, 255);
	inline ImVec4 tab_pop_gradient = ImColor(171, 33, 33, 255);
	namespace button
	{
		inline ImVec4 background_hov = ImColor(26, 26, 26, 200);
		inline ImVec4 background = ImColor(20, 20, 20, 200);
		inline ImVec4 background_active = ImColor(40, 40, 40, 200);

		inline ImVec4 outline_background = ImColor(30, 29, 34, 255);
		inline float rounding = 4;
		inline float bg_rounding = 5.f;
	}
}


namespace c
{
	inline ImVec4 accent = ImColor(252, 0, 0, 255);
	inline ImVec4 roll_color = ImColor(255, 255, 255);
	inline ImVec4 accent_color = ImColor(252, 0, 0, 255);
	inline ImVec4 accent_colorx = ImColor(255, 0, 0);
	inline ImVec4 accent_colorz = ImColor(155, 0, 0);

	inline ImColor accent_colorxxxxxxxxxx = ImColor(63, 0, 105);
	inline ImVec4 accent_low_color = ImColor(250, 132, 86, 255 / 2);
	inline ImVec4 accent_low = ImColor(174, 197, 255, 255 / 2);

	inline ImVec4 accent_text_color = ImColor(245, 245, 255);

	inline ImVec4 accent_text_low_color = ImColor(245, 245, 255, 255 / 2);

	inline ImVec4 notify = ImColor(43, 48, 54);

	inline float size = 0.5f;
	namespace widget
	{
		inline ImVec4 background = ImColor(28, 28, 35, 255);
		inline ImVec4 selectable = ImColor(37, 37, 47, 255);
		inline ImVec4 popup = ImColor(21, 21, 22, 255);

		inline ImVec4 circle = ImColor(47, 55, 65, 255);
		inline float rounding = 4;
	}
	namespace element
	{
		namespace info_bar
		{
			inline ImVec4 filling = ImColor(0, 0, 0, 255);
			inline ImVec4 stroke = ImColor(14, 15, 22, 255);

			inline ImVec4 box = ImColor(8, 9, 12, 255);
			inline ImVec4 box_outline = ImColor(12, 12, 20, 255);
		}

		inline ImVec4 page_active = ImColor(108, 107, 130, 30);
		inline ImVec4 selectable = ImColor(18, 20, 34, 255);

		namespace popup_elements
		{
			inline ImVec4 filling = ImColor(10, 10, 10, 170);
			inline ImVec4 cog = ImColor(64, 69, 104, 255);
		}

		namespace notify
		{
			inline ImVec4 red_status = ImColor(255, 48, 47, 255);
			inline ImVec4 blue_status = ImColor(77, 98, 229, 255);
			inline ImVec4 green_status = ImColor(74, 201, 126, 255);
			inline ImVec4 yellow_status = ImColor(226, 111, 32, 255);

		}

		namespace combo
		{
			inline ImVec4 filling = ImColor(15, 16, 23, 255);
		}

		inline ImVec4 filling = ImColor(25, 23, 48, 255);
		inline ImVec4 circle_mark = ImColor(33, 31, 59, 255);
		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(111, 115, 148, 255);
		inline ImVec4 text = ImColor(75, 78, 101, 255);
		inline ImVec4 selector = ImColor(30, 31, 33, 80);

		inline float rounding = 8;
	}
	namespace bg
	{
		inline ImVec4 background = ImColor(14, 14, 15, 210);
		inline ImVec4 outline_background = ImColor(27, 29, 32, 210);
		inline ImVec2 size = ImVec2(1050, 640);
		inline float rounding = 12;
	}
	namespace elements
	{
		inline ImVec4 mark = ImColor(255, 255, 255);

		inline ImVec4 stroke = ImColor(28, 26, 37);
		inline ImVec4 background = ImColor(11, 11, 13, 115);
		inline ImVec4 background_widget = ImColor(21, 23, 26);

		inline ImVec4 text_active = ImColor(255, 255, 255);
		inline ImVec4 text_hov = ImColor(81, 92, 109);
		inline ImVec4 text = ImColor(243, 251, 263, 85);

		inline float rounding = 4;
	}

	namespace child
	{
		inline ImVec4 background = ImColor(22, 23, 26, 250);
		inline ImVec4 outline_background = ImColor(27, 29, 32, 255);
		inline float rounding = 6;
	}

	namespace checkbox
	{
		inline ImVec4 circle_inactive = ImColor(32, 32, 32, 255);
		inline ImVec4 check_inactive = ImColor(27, 29, 32, 0);
		inline ImVec4 background = ImColor(27, 29, 32, 255);
		inline ImVec4 outline_background = ImColor(30, 32, 36, 255);
		inline float rounding = 20;
	}

	namespace slider
	{
		inline ImVec4 circle_inactive = ImColor(43, 48, 54, 255);

		inline ImVec4 background = ImColor(27, 29, 32, 255);
		inline ImVec4 outline_background = ImColor(30, 32, 36, 255);
		inline float rounding = 30;
	}
	inline ImVec4 black = ImColor(0, 0, 0, 255);

	namespace combo
	{
		inline ImVec4 background = ImColor(27, 29, 32, 255);
		inline ImVec4 outline_background = ImColor(30, 32, 36, 255);
		inline float rounding = 3;
	}

	namespace picker
	{
		inline ImVec4 background = ImColor(27, 29, 32, 255);
		inline ImVec4 outline_background = ImColor(30, 32, 36, 255);
		inline float rounding = 2;
	}

	namespace button
	{
		inline ImVec4 background_hov = ImColor(26, 26, 26, 200);
		inline ImVec4 background = ImColor(20, 20, 20, 200);
		inline ImVec4 background_active = ImColor(40, 40, 40, 200);

		inline ImVec4 outline_background = ImColor(30, 30, 30, 255);
		inline ImVec4 outline_background1 = ImColor(34, 34, 34, 255);

		inline float rounding = 4;
		inline float bg_rounding = 5.f;
	}

	namespace input
	{
		inline ImVec4 accent = ImColor(235, 5, 12, 255);
		inline ImVec4 text_selected = ImColor(70, 70, 70, 100);
		inline ImVec4 text_hint = ImColor(180, 180, 180, 255);
		inline ImVec4 text = ImColor(255, 255, 255, 255);
		inline ImVec4 background_hov = ImColor(70, 70, 70, 60);
		inline ImVec4 background = ImColor(30, 30, 30, 20);
		inline ImVec4 background_active = ImColor(26, 27, 29, 255);
		inline ImVec4 background_inactive = ImColor(16, 17, 19, 255);

		inline ImVec4 outline = ImColor(22, 24, 26, 255);
		inline ImVec4 icon = ImColor(26, 27, 31, 255);

		inline float rounding = 2.f;

		/*
			inline ImVec4 background = ImColor(27, 29, 32, 255);
		inline ImVec4 outline_background = ImColor(30, 32, 36, 255);
		inline float rounding = 4;
		*/

	}

	namespace keybind
	{
		inline float rounding = 4.f * c::size;
		inline ImVec4 background = ImColor(25, 25, 25, 255);
		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(130, 130, 130, 255);
		inline ImVec4 text = ImColor(90, 93, 100, 255);
	}


	namespace text
	{
		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(245, 245, 255);
		inline ImVec4 text = ImColor(90, 93, 100);
		inline ImVec4 text2 = ImColor(90, 93, 100, 255);
		inline ImVec4 hide_text = ImColor(43, 48, 54, 255);

	}

}