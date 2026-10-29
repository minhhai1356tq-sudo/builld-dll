

#pragma once
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif 

#include <string>
#include <wtypes.h>
#include <algorithm> 
#include <vector>
#include <iostream>
#include <Windows.h>

#include <cstdlib>
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
inline ImColor LightGreen = ImColor(50, 255, 100);
inline ImColor SoftRed = ImColor(255, 80, 80);
inline ImColor WarningYellow = ImColor(255, 200, 0);
inline ImColor InfoBlue = ImColor(50, 150, 255);
inline ImColor Purple = ImColor(180, 100, 255);

inline ImColor SoftYellow = ImColor(255, 230, 120);

struct NotifyItem {
	std::string message, information;
	ImColor color;
	ImRect bb;
	ImVec2 offset = ImVec2(-200.f, 0.f);
	ImVec2 target_offset = ImVec2(-200.f, 0.f);
	float timer = 0.f;
};

class NotificationSystem {
public:
	void Notification(const std::string& message, const std::string& information, ImColor color)
	{
		NotifyItem notif;
		notif.message = message;
		notif.information = information;
		notif.color = color;
		notif.offset = ImVec2(-200.f, 0.f);
		notif.target_offset = ImVec2(-200.f, 0.f);
		notif.timer = 0.f;
		notifications.push_back(notif);
	}

	void DrawNotifications()
	{
		ImVec2 displaySize = ImGui::GetIO().DisplaySize;
		int index = 0;

		for (auto it = notifications.begin(); it != notifications.end(); )
		{
			NotifyItem& notif = *it;
			notif.timer += ImGui::GetIO().DeltaTime * 10.f;
			notif.target_offset = (notif.timer > 30.f) ? ImVec2(5000.f, 0.f) : ImVec2(0.f, 0.f);
			notif.offset = ImLerp(notif.offset, notif.target_offset, ImGui::GetIO().DeltaTime * 10.f);

			float notif_x = displaySize.x - 150.f + notif.offset.x;
			float notif_y = displaySize.y - 60.f - index * 55.f;

			ImVec2 notif_pos = ImVec2(notif_x, notif_y);
			notif.bb = ImRect(notif_pos, notif_pos + ImVec2(140.f, 46.f));

			auto draw = ImGui::GetForegroundDrawList();


			draw->AddRectFilled(notif.bb.Min, notif.bb.Max, ImColor(0, 0, 0, 204), 6.f);


			draw->AddText(notif.bb.Min + ImVec2(10.f, 6.f), IM_COL32(255, 255, 255, 255), notif.message.c_str());
			draw->AddText(notif.bb.Min + ImVec2(10.f, 22.f), ImColor(160, 160, 160), notif.information.c_str());


			float  progress = std::clamp(notif.timer / 30.f, 0.f, 1.f);
			ImVec2 bar_min = ImVec2(notif.bb.Min.x, notif.bb.Max.y - 4.f);
			ImVec2 bar_max = ImVec2(notif.bb.Min.x + notif.bb.GetWidth() * progress, notif.bb.Max.y);
			draw->AddRectFilled(bar_min, bar_max, notif.color);
			draw->AddShadowRect(bar_min, bar_max, notif.color, 45.f, ImVec2(0, 0), 0, 0.f);

			if (notif.timer > 35.f) it = notifications.erase(it);
			else { ++it; ++index; }
		}
	}

private:
	std::vector<NotifyItem> notifications;
};

inline NotificationSystem notificationSystem;
