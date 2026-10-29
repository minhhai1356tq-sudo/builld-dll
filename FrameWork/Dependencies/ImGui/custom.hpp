#include "imgui.h"
#include "imgui_internal.h"

extern ImFont* tab_icons;
namespace Custom {
	bool Selection(const char* label, ImTextureID user_texture_id, bool selected);
	bool Tabb(const char* name, bool boolean);
	bool Checkbox(const char* label, bool* v);
	bool tab(const char* icon, bool boolean);
	void setup_circles();
	void draw_circles_and_lines(ImU32 color, ImVec2 aaa);
};
