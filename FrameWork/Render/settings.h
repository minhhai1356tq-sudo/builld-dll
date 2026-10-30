#pragma once
#include "colors_widgets.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

// #include <D3DX11.h> // Đã comment để build trên Github Actions
#pragma once
namespace settings {

    inline ImVec2 size_menu = ImVec2(640, 520);
    inline ImVec2 size_watermark = ImVec2(479, 50);
    inline ImVec2 size_preview = ImVec2(300, 400);

}


namespace menu {

    inline ImVec4 general_child = ImColor(23, 23, 25);

}

namespace pictures {

    inline ID3D11ShaderResourceView* logo_img = nullptr;
    inline ID3D11ShaderResourceView* aim_img = nullptr;
    inline ID3D11ShaderResourceView* misc_img = nullptr;
    inline ID3D11ShaderResourceView* visual_img = nullptr;
    inline ID3D11ShaderResourceView* world_img = nullptr;
    inline ID3D11ShaderResourceView* settings_img = nullptr;
    inline ID3D11ShaderResourceView* pen_img = nullptr;
    inline ID3D11ShaderResourceView* keyboard_img = nullptr;
    inline ID3D11ShaderResourceView* input_img = nullptr;
    inline ID3D11ShaderResourceView* wat_logo_img = nullptr;
    inline ID3D11ShaderResourceView* fps_img = nullptr;
    inline ID3D11ShaderResourceView* player_img = nullptr;
    inline ID3D11ShaderResourceView* time_img = nullptr;

}


namespace font
{
	inline ImFont* Nev = nullptr;
	inline ImFont* icomoon_logo = nullptr;
	inline ImFont* description_font = nullptr;
	inline ImFont* esp_font = nullptr;
	inline ImFont* regular_m = nullptr;

	inline ImFont* default_r = nullptr;
	inline ImFont* default_m = nullptr;
	inline ImFont* default_s = nullptr;
	inline ImFont* icomoon_tabs = nullptr;
	inline ImFont* lexend_regular = nullptr;

	inline ImFont* noto_reg = nullptr;
	inline ImFont* noto_symbols2 = nullptr;
	inline ImFont* noto_arabic = nullptr;
	inline ImFont* noto_devanagari = nullptr;
	inline ImFont* noto_japanese = nullptr;
	inline ImFont* noto_khmer = nullptr;
	inline ImFont* noto_korean = nullptr;
	inline ImFont* noto_sc = nullptr;
	inline ImFont* noto_sinhala = nullptr;
	inline ImFont* noto_tamil = nullptr;
	inline ImFont* noto_tc = nullptr;
	inline ImFont* noto_thai = nullptr;
	inline ImFont* unifont = nullptr;

	inline ImFont* regular_l = nullptr;
	inline ImFont* icomoon_page = nullptr;
	inline ImFont* small_font = nullptr;
	inline ImFont* inter_bold = nullptr;
	inline ImFont* inter_semibold = nullptr;
	inline ImFont* s_inter_semibold = nullptr;
	inline ImFont* inter_medium = nullptr;
	inline ImFont* icon_notify = nullptr;
}

#pragma once

#include <d3d11.h>
// #include <d3dx11.h> // Đã comment để build trên Github Actions

namespace FWork {
	namespace Fonts {

		inline ImFont* FontAwesomeRegular = nullptr;
		inline ImFont* FontAwesomeSolid = nullptr;
		inline ImFont* FontAwesomeSolid18 = nullptr;
		inline ImFont* FontAwesomeSolidBig = nullptr;

		inline ImFont* GeistRegular = nullptr;
		inline ImFont* GeistRegularMedium = nullptr;
		inline ImFont* GeistMedium = nullptr;
		inline ImFont* GeistBoldMedium = nullptr;
		inline ImFont* GeistBold = nullptr;
		inline ImFont* IconWeapon = nullptr;

		inline ImFont* InterExtraLight = nullptr;
		inline ImFont* InterLight = nullptr;
		inline ImFont* InterThin = nullptr;
		inline ImFont* InterRegular = nullptr;
		inline ImFont* InterRegular14 = nullptr;
		inline ImFont* InterMedium = nullptr;
		inline ImFont* InterSemiBold = nullptr;
		inline ImFont* InterBold = nullptr;
		inline ImFont* InterBold12 = nullptr;
		inline ImFont* InterExtraBold = nullptr;
		inline ImFont* InterBlack = nullptr;


		inline ImFont* NotoSansRegular = nullptr;
		inline ImFont* NotoSansSymbolsRegular = nullptr;
		inline ImFont* NotoEmojiRegular = nullptr;
		inline ImFont* ArialUnicode = nullptr;
		inline ImFont* LexendRegular = nullptr;




		inline ID3D11ShaderResourceView* Logo;

		void Initialize(ID3D11Device* Device);
	}
}
namespace utils
{
	inline float CalculateTextWidthWithoutColorCodes(const ImFont* font, const char* text_begin, const char* text_end = nullptr) {
		if (!text_end)
			text_end = text_begin + strlen(text_begin); // Если text_end не указан, вычисляем его

		float text_width = 0.0f;
		const char* s = text_begin;

		while (s < text_end) {
			if (*s == '^' && (s + 1) < text_end && *(s + 1) >= '0' && *(s + 1) <= '9') {
				// Пропускаем цветовой код (^ и следующую цифру)
				s += 2;
				continue;
			}

			// Декодируем символ UTF-8
			unsigned int c = (unsigned int)*s;
			if (c < 0x80) {
				s += 1;
			}
			else {
				s += ImTextCharFromUtf8(&c, s, text_end);
				if (c == 0) // Некорректный UTF-8
					break;
			}

			// Получаем глиф и добавляем его ширину к общей ширине текста
			const ImFontGlyph* glyph = font->FindGlyph((ImWchar)c);
			if (glyph) {
				text_width += glyph->AdvanceX;
			}
		}

		return text_width;
	}

	inline ImColor GetColorWithAlpha(ImColor color, float alpha)
	{
		return ImColor(color.Value.x, color.Value.y, color.Value.z, alpha);
	}

	inline ImVec2 center_text(ImVec2 min, ImVec2 max, const char* text)
	{
		return min + (max - min) / 2 - ImGui::CalcTextSize(text) / 2;
	}

	inline ImColor GetDarkColor(const ImColor& color)
	{
		
		float r, g, b, a;
		r = color.Value.x;
		g = color.Value.y;
		b = color.Value.z;
		a = 255;

		float darkPercentage = 0.2f;
		float darkR = r * darkPercentage;
		float darkG = g * darkPercentage;
		float darkB = b * darkPercentage;

		return ImColor(darkR, darkG, darkB, a);
	}
	inline ImVec4 ImColorToImVec4(const ImColor& color)
	{
		return ImVec4(color.Value.x, color.Value.y, color.Value.z, color.Value.w);
	}
}
