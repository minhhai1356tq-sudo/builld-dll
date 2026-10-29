#define IMGUI_DEFINE_MATH_OPERATORS

#include "Visual.hpp"
#include <Windows.h>
#include <cmath>
#include <algorithm>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <ImGui/imgui_internal.h>
#include <ImGui/imgui.h>
#include <FrameWork/Render/settings.h>
#include <FrameWork/Render/Globals.h>
#include <internal/Cheat ff/Math/WorldToScreen.hpp>
#include <internal/Cheat ff/Offsets.h>
#include <internal/Cheat ff/Memory/Memory.hpp>

struct HealthBarState {
    float AnimatedHealth = 1.0f;
    float PulseTime = 0.0f;
};

struct BoxAnimState {
    float GlowIntensity = 0.0f;
    float PulseTime = 0.0f;
};

static std::unordered_map<int, HealthBarState> healthBarStates;
static std::unordered_map<int, BoxAnimState> boxAnimStates;

void DrawNameTag(const Player& player, ImVec2 headPos, float boxWidth, float boxHeight) {
    if (!g_Globals.Visuals.ESPInfo && !g_Globals.Visuals.HealthBar) return;

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    // Use Bold font for premium look 
    ImGui::PushFont(font::inter_bold);
    float fontScale = g_Globals.Visuals.TextSize / 15.0f;
    if (fontScale < 0.7f) fontScale = 0.7f;

    float fontScaleText = fontScale * 0.7f;
    float paddingX = 8.0f * fontScale;
    float paddingY = 3.0f * fontScale;

    // HP Value (lượng máu còn lại, 1-200) - nền trắng, chữ đen
    char hpValueStr[16];
    snprintf(hpValueStr, sizeof(hpValueStr), "%d", player.Health);
    ImVec2 hpValueSize = ImGui::CalcTextSize(hpValueStr) * fontScaleText;

    std::string nameStr = player.Name;
    ImVec2 nameSize = ImGui::CalcTextSize(nameStr.c_str()) * fontScaleText;

    char distStr[16];
    snprintf(distStr, sizeof(distStr), "%dm", (int)player.Distance);
    ImVec2 distSize = ImGui::CalcTextSize(distStr) * fontScaleText;

    float totalWidth = 0;
    // HP Value first
    totalWidth += hpValueSize.x + paddingX * 2 + 2.0f;
    totalWidth += nameSize.x + paddingX * 2 + 2.0f;
    totalWidth += distSize.x + paddingX * 2;

    float maxTextHeight = (std::max)({ hpValueSize.y, nameSize.y, distSize.y });
    float tagHeight = (maxTextHeight + paddingY * 2) * 0.85f;

    // Nâng chữ lên một tí
    float verticalOffset = (boxHeight * 0.05f) + 3.0f;
    ImVec2 startPos = ImVec2(headPos.x - totalWidth / 2, headPos.y - tagHeight - verticalOffset);
    float rounding = 2.0f;

    // Draw background and text
    float currentX = startPos.x;
    const float scale = 1.15f;
    // HP Value - nền trắng, chữ đen (always draw with ESPInfo)
    {
        float width = hpValueSize.x + paddingX * 2;
        float height = tagHeight;
        ImVec2 center = ImVec2(currentX + width / 2.0f, startPos.y + height / 2.0f);
        float scaledWidth = width * scale;
        float scaledHeight = height * scale;
        ImVec2 bgMin = ImVec2(center.x - scaledWidth / 2.0f, center.y - scaledHeight / 2.0f);
        ImVec2 bgMax = ImVec2(center.x + scaledWidth / 2.0f, center.y + scaledHeight / 2.0f);
        drawList->AddRectFilled(bgMin, bgMax, IM_COL32(255, 255, 255, 220), rounding);
        // Center text vertically and horizontally
        ImVec2 textSize = ImGui::CalcTextSize(hpValueStr);
        ImVec2 textPos = ImVec2(center.x - textSize.x / 2.0f, center.y - textSize.y / 2.0f);
        drawList->AddText(textPos, IM_COL32(0, 0, 0, 255), hpValueStr);
        currentX += hpValueSize.x + paddingX * 2 + 2.0f;
    }

    // Name (always draw with ESPInfo)
    {
        float width = nameSize.x + paddingX * 2;
        float height = tagHeight;
        ImVec2 center = ImVec2(currentX + width / 2.0f, startPos.y + height / 2.0f);
        float scaledWidth = width * scale;
        float scaledHeight = height * scale;
        ImVec2 bgMin = ImVec2(center.x - scaledWidth / 2.0f, center.y - scaledHeight / 2.0f);
        ImVec2 bgMax = ImVec2(center.x + scaledWidth / 2.0f, center.y + scaledHeight / 2.0f);
        drawList->AddRectFilled(bgMin, bgMax, IM_COL32(0, 0, 0, 180), rounding);
        // Center text vertically and horizontally
        ImVec2 textSize = ImGui::CalcTextSize(nameStr.c_str());
        ImVec2 textPos = ImVec2(center.x - textSize.x / 2.0f, center.y - textSize.y / 2.0f);
        ImU32 nameColor = (player.IsTeam == Player::Bool3::True) ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255);
        drawList->AddText(textPos, nameColor, nameStr.c_str());
        currentX += nameSize.x + paddingX * 2 + 2.0f;
    }

    // Distance (always draw with ESPInfo)
    {
        float width = distSize.x + paddingX * 2;
        float height = tagHeight;
        ImVec2 center = ImVec2(currentX + width / 2.0f, startPos.y + height / 2.0f);
        float scaledWidth = width * scale;
        float scaledHeight = height * scale;
        ImVec2 bgMin = ImVec2(center.x - scaledWidth / 2.0f, center.y - scaledHeight / 2.0f);
        ImVec2 bgMax = ImVec2(center.x + scaledWidth / 2.0f, center.y + scaledHeight / 2.0f);
        drawList->AddRectFilled(bgMin, bgMax, IM_COL32(0, 0, 0, 180), rounding);
        // Center text vertically and horizontally
        ImVec2 textSize = ImGui::CalcTextSize(distStr);
        ImVec2 textPos = ImVec2(center.x - textSize.x / 2.0f, center.y - textSize.y / 2.0f);
        drawList->AddText(textPos, IM_COL32(255, 255, 0, 255), distStr);
    }

    ImGui::PopFont();
}

static void DrawHorizontalHealthBarTop(short CurrentHealth, short MaxHealth, float x, float y, float w, int GlowRadius = 10)
{
    ImDrawList* DrawList = ImGui::GetBackgroundDrawList();
    if (MaxHealth <= 0) return;

    float HealthPercentage = std::clamp(static_cast<float>(CurrentHealth) / MaxHealth, 0.0f, 1.0f);
    float FilledBarWidth = w * HealthPercentage;
    float BarHeight = 2.5f;

    ImVec4 GreenColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    ImVec4 YellowColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    ImVec4 RedColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

    ImVec4 HealthBarColor = (HealthPercentage > 0.5f)
        ? ImLerp(GreenColor, YellowColor, (1.0f - HealthPercentage) * 2.0f)
        : ImLerp(YellowColor, RedColor, (0.5f - HealthPercentage) * 2.0f);

    ImVec4 GlowColor = (HealthPercentage > 0.5f) ? GreenColor :
        (HealthPercentage > 0.25f) ? YellowColor : RedColor;

    ImU32 BarColorU32 = ImGui::ColorConvertFloat4ToU32(HealthBarColor);


    DrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + BarHeight), IM_COL32(0, 0, 0, 128));


    for (int i = GlowRadius; i > 0; --i) {
        float Alpha = 25.0f * (1.0f - ((float)i / GlowRadius));
        DrawList->AddRectFilled(
            ImVec2(x - i, y - i),
            ImVec2(x + FilledBarWidth + i, y + BarHeight + i),
            IM_COL32((int)(GlowColor.x * 255), (int)(GlowColor.y * 255), (int)(GlowColor.z * 255), (int)Alpha)
        );
    }


    DrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + FilledBarWidth, y + BarHeight), BarColorU32);
}

static void DrawVerticalHealthBar(short CurrentHealth, short MaxHealth, ImVec2 Position, float TotalHeight, int GlowRadius = 10)
{
    ImDrawList* DrawList = ImGui::GetBackgroundDrawList();
    if (MaxHealth <= 0) return;

    float HealthPercentage = std::clamp(static_cast<float>(CurrentHealth) / MaxHealth, 0.0f, 1.0f);
    float FilledBarHeight = TotalHeight * HealthPercentage;
    float BarWidth = 2.5f;

    ImVec4 GreenColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    ImVec4 YellowColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    ImVec4 RedColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

    ImVec4 HealthBarColor = (HealthPercentage > 0.5f)
        ? ImLerp(GreenColor, YellowColor, (1.0f - HealthPercentage) * 2.0f)
        : ImLerp(YellowColor, RedColor, (0.5f - HealthPercentage) * 2.0f);

    ImU32 BarColorU32 = ImGui::ColorConvertFloat4ToU32(HealthBarColor);

    DrawList->AddRectFilled(ImVec2(Position.x, Position.y), ImVec2(Position.x + BarWidth, Position.y + TotalHeight), IM_COL32(0, 0, 0, 128));

    ImVec4 GlowColor = (HealthPercentage > 0.5f) ? GreenColor : (HealthPercentage > 0.25f) ? YellowColor : RedColor;

    for (int i = GlowRadius; i > 0; --i) {
        float Alpha = 25.0f * (1.0f - ((float)i / GlowRadius));
        DrawList->AddRectFilled(
            ImVec2(Position.x - i, Position.y + (TotalHeight - FilledBarHeight) - i),
            ImVec2(Position.x + BarWidth + i, Position.y + TotalHeight + i),
            IM_COL32((int)(GlowColor.x * 255), (int)(GlowColor.y * 255), (int)(GlowColor.z * 255), (int)Alpha)
        );
    }

    DrawList->AddRectFilled(
        ImVec2(Position.x, Position.y + (TotalHeight - FilledBarHeight)),
        ImVec2(Position.x + BarWidth, Position.y + TotalHeight),
        BarColorU32
    );
}

static void DrawHealthBarBelowFullBox(short CurrentHealth, short MaxHealth, float x, float y, float w, float h, int GlowRadius = 10)
{
    ImDrawList* DrawList = ImGui::GetBackgroundDrawList();
    if (MaxHealth <= 0) return;

    float HealthPercentage = std::clamp(static_cast<float>(CurrentHealth) / MaxHealth, 0.0f, 1.0f);
    float FilledBarWidth = w * HealthPercentage;
    float BarHeight = 2.5f;
    float offsetY = 5.0f;

    ImVec4 GreenColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    ImVec4 YellowColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    ImVec4 RedColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

    ImVec4 HealthBarColor = (HealthPercentage > 0.5f)
        ? ImLerp(GreenColor, YellowColor, (1.0f - HealthPercentage) * 2.0f)
        : ImLerp(YellowColor, RedColor, (0.5f - HealthPercentage) * 2.0f);

    ImVec4 GlowColor = (HealthPercentage > 0.5f) ? GreenColor : (HealthPercentage > 0.25f) ? YellowColor : RedColor;

    ImU32 BarColorU32 = ImGui::ColorConvertFloat4ToU32(HealthBarColor);

    DrawList->AddRectFilled(ImVec2(x, y + h + offsetY), ImVec2(x + w, y + h + offsetY + BarHeight), IM_COL32(0, 0, 0, 128));

    for (int i = GlowRadius; i > 0; --i) {
        float Alpha = 30.0f * (1.0f - ((float)i / GlowRadius));
        DrawList->AddRectFilled(
            ImVec2(x - i, y + h + offsetY - i),
            ImVec2(x + FilledBarWidth + i, y + h + offsetY + BarHeight + i),
            IM_COL32((int)(GlowColor.x * 255), (int)(GlowColor.y * 255), (int)(GlowColor.z * 255), (int)Alpha)
        );
    }

    DrawList->AddRectFilled(ImVec2(x, y + h + offsetY), ImVec2(x + FilledBarWidth, y + h + offsetY + BarHeight), BarColorU32);

    char healthText[12];
    snprintf(healthText, sizeof(healthText), "%d HP", CurrentHealth);

    ImVec2 textSize = ImGui::CalcTextSize(healthText);
    ImVec2 textPos = ImVec2(x + (w / 2.0f) - (textSize.x / 2.0f), y + h + offsetY + BarHeight + 2.0f);

    ImU32 textColor = IM_COL32(255, 255, 255, 230);
    if (HealthPercentage <= 0.25f) textColor = IM_COL32(255, 60, 60, 230);
    else if (HealthPercentage <= 0.5f) textColor = IM_COL32(255, 215, 0, 230);

    DrawList->AddText(textPos, textColor, healthText);
}

void DrawAnimatedRainbowFOVWithGlow(ImVec2 center, float radius, float lineThickness, float glowThickness, int segments = 100, float rainbowSpeed = 0.2f) {
    auto* drawList = ImGui::GetBackgroundDrawList();
    double time = ImGui::GetTime();
    float hueOffset = fmod(time * rainbowSpeed, 1.0f);
    float glowOpacity = 0.10f;
    for (float thickness = glowThickness; thickness > 1.0f; thickness -= 1.5f) {
        for (int i = 0; i < segments; ++i) {
            float a0 = (2.0f * IM_PI * i) / segments;
            float a1 = (2.0f * IM_PI * (i + 1)) / segments;

            ImVec2 p0 = ImVec2(center.x + cosf(a0) * radius, center.y + sinf(a0) * radius);
            ImVec2 p1 = ImVec2(center.x + cosf(a1) * radius, center.y + sinf(a1) * radius);

            ImU32 glowColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, glowOpacity));
            drawList->AddLine(p0, p1, glowColor, thickness);
        }
        glowOpacity * 0.6f;
    }
    for (int i = 0; i < segments; ++i) {
        float a0 = (2.0f * IM_PI * i) / segments;
        float a1 = (2.0f * IM_PI * (i + 1)) / segments;
        ImVec2 p0 = ImVec2(center.x + cosf(a0) * radius, center.y + sinf(a0) * radius);
        ImVec2 p1 = ImVec2(center.x + cosf(a1) * radius, center.y + sinf(a1) * radius);
        float hue = fmod(hueOffset + (float)i / segments, 1.0f);
        ImU32 color = ImColor::HSV(hue, 1.0f, 1.0f);
        drawList->AddLine(p0, p1, color, lineThickness);
    }
}

void ESP::Players()
{
    if (!g_Globals.Visuals.Enable) return;

    auto bgList = ImGui::GetBackgroundDrawList();
    auto fgList = ImGui::GetForegroundDrawList();

    if (!g_Globals.EspConfig.Matrix) return;

    if (g_Globals.AimBot.SilentAimEnabled && g_Globals.AimBot.SilentAimMode == 0 && g_Globals.AimBot.ShowSilentFOV) {
        DrawAnimatedRainbowFOVWithGlow(ImVec2(g_Globals.EspConfig.Width / 2.0f, g_Globals.EspConfig.Height / 2.0f), (float)g_Globals.AimBot.SilentAimFov, 1.5f, 5.0f);
    }

    for (auto& [entityID, player] : g_Globals.EspConfig.Entities)
    {
        if (!player.IsKnown || player.IsDead) continue;
        if (!g_Globals.Visuals.EspInvisible && !player.IsVisible) continue;

        float dist = player.Distance;
        if (dist <= 0 || dist > g_Globals.Visuals.DistanceEsp) continue;

        ImVec2 headPos = W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.Head, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height);
        ImVec2 rootPos = W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.Root, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height);

        if (headPos.x <= 0 || headPos.y <= 0 || rootPos.x <= 0 || rootPos.y <= 0) continue;

        float boxHeight = fabsf(headPos.y - rootPos.y);
        float boxWidth = boxHeight * 0.65f;
        float boxX = headPos.x - boxWidth * 0.5f;
        float boxY = (std::min)(headPos.y, rootPos.y);

        if (g_Globals.Visuals.Lines)
        {
            ImColor lineCol = ImColor(g_Globals.Visuals.LinesColor[0], g_Globals.Visuals.LinesColor[1], g_Globals.Visuals.LinesColor[2], g_Globals.Visuals.LinesColor[3]);
            if (player.IsKnocked) lineCol = ImColor(255, 0, 0, 255);

            ImVec2 lineTarget = (g_Globals.Visuals.EspLines == 1) ? ImVec2(g_Globals.EspConfig.Width * .5f, 0.f) : ImVec2(g_Globals.EspConfig.Width * .5f, (float)g_Globals.EspConfig.Height);

            if (g_Globals.Visuals.GlowLines)
            {
                for (float i = 6.0f; i >= 1.0f; i -= 1.0f)
                {
                    ImColor glowCol = ImColor(lineCol.Value.x, lineCol.Value.y, lineCol.Value.z, lineCol.Value.w * (0.08f / i));
                    bgList->AddLine(headPos, lineTarget, glowCol, g_Globals.Visuals.LineThickness + i * 2.0f);
                }
            }

            bgList->AddLine(headPos, lineTarget, lineCol, g_Globals.Visuals.LineThickness);
        }

        if (g_Globals.Visuals.Skeleton)
        {
            float skeletonThickness = g_Globals.EspConfig.skeletonThickness;
            ImColor skelCol = ImColor(g_Globals.Visuals.SkeletonColor[0], g_Globals.Visuals.SkeletonColor[1], g_Globals.Visuals.SkeletonColor[2], g_Globals.Visuals.SkeletonColor[3]);
            if (player.IsKnocked) skelCol = ImColor(255, 0, 0, 255);

            auto DrawBone = [&](const Vector3& from, const Vector3& to) {
                if (from == Vector3::Zero() || to == Vector3::Zero()) return;

                ImVec2 sa = W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, from, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height);
                ImVec2 sb = W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, to, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height);

                if (sa.x >= 0 && sb.x >= 0 && sa.y >= 0 && sb.y >= 0) {
                    bgList->AddLine(sa, sb, skelCol, skeletonThickness);
                }
                };

            float baseCircleSize = 3.f;
            float distanceFactor = std::clamp(100.0f / dist, 0.5f, 1.5f);
            float circleSize = baseCircleSize * distanceFactor;

            bgList->AddCircle(headPos, circleSize, skelCol, 0, skeletonThickness);

            DrawBone(player.Head, player.Neck);
            DrawBone(player.Neck, player.LeftShoulder);
            DrawBone(player.Neck, player.RightShoulder);
            DrawBone(player.LeftShoulder, player.LeftElbow);
            DrawBone(player.RightShoulder, player.RightElbow);
            DrawBone(player.LeftElbow, player.LeftWrist);
            DrawBone(player.RightElbow, player.RightWrist);
            DrawBone(player.Neck, player.Hip);
            DrawBone(player.Hip, player.LeftFoot);
            DrawBone(player.Hip, player.RightFoot);
        }

        if (g_Globals.Visuals.FilledBox)
        {
            ImColor filledCol = ImColor(0, 0, 0, 80);
            bgList->AddRectFilled(ImVec2(boxX, boxY), ImVec2(boxX + boxWidth, boxY + boxHeight), filledCol, 0.0f);
        }

        if (g_Globals.Visuals.Box)
        {
            ImColor boxCol = ImColor(g_Globals.Visuals.BoxColor[0], g_Globals.Visuals.BoxColor[1], g_Globals.Visuals.BoxColor[2], g_Globals.Visuals.BoxColor[3]);
            if (g_Globals.Visuals.players_box == 1)
            {
                bgList->AddRect(ImVec2(boxX, boxY), ImVec2(boxX + boxWidth, boxY + boxHeight), boxCol, 0.0f, 0, g_Globals.Visuals.Thickness);
            }
            else if (g_Globals.Visuals.players_box == 2)
            {
                float lineW = (boxWidth / 4);
                float lineH = (boxWidth / 4);
                ImVec2 t_l = ImVec2(boxX, boxY);
                ImVec2 t_r = ImVec2(boxX + boxWidth, boxY);
                ImVec2 b_l = ImVec2(boxX, boxY + boxHeight);
                ImVec2 b_r = ImVec2(boxX + boxWidth, boxY + boxHeight);


                bgList->AddLine(t_l, ImVec2(t_l.x + lineW, t_l.y), boxCol, g_Globals.Visuals.Thickness);
                bgList->AddLine(t_l, ImVec2(t_l.x, t_l.y + lineH), boxCol, g_Globals.Visuals.Thickness);

                bgList->AddLine(t_r, ImVec2(t_r.x - lineW, t_r.y), boxCol, g_Globals.Visuals.Thickness);
                bgList->AddLine(t_r, ImVec2(t_r.x, t_r.y + lineH), boxCol, g_Globals.Visuals.Thickness);

                bgList->AddLine(b_l, ImVec2(b_l.x + lineW, b_l.y), boxCol, g_Globals.Visuals.Thickness);
                bgList->AddLine(b_l, ImVec2(b_l.x, b_l.y - lineH), boxCol, g_Globals.Visuals.Thickness);

                bgList->AddLine(b_r, ImVec2(b_r.x - lineW, b_r.y), boxCol, g_Globals.Visuals.Thickness);
                bgList->AddLine(b_r, ImVec2(b_r.x, b_r.y - lineH), boxCol, g_Globals.Visuals.Thickness);
            }
        }

        if (g_Globals.Visuals.HeadDot)
        {
            ImColor dotCol = ImColor(g_Globals.Visuals.HeadDotColor[0], g_Globals.Visuals.HeadDotColor[1], g_Globals.Visuals.HeadDotColor[2], g_Globals.Visuals.HeadDotColor[3]);
            bgList->AddCircleFilled(headPos, 3.5f, dotCol, 0);
            bgList->AddCircle(headPos, 3.5f, ImColor(0, 0, 0, 255), 0, 1.0f);
        }

        if (g_Globals.Visuals.HealthBar)
        {
            switch (g_Globals.Visuals.players_healthbar)
            {
            case 1: DrawVerticalHealthBar(player.Health, 200, ImVec2(boxX - 6, boxY), boxHeight); break;
            case 2: DrawVerticalHealthBar(player.Health, 200, ImVec2(boxX + boxWidth + 2, boxY), boxHeight); break;
            case 3: DrawHorizontalHealthBarTop(player.Health, 200, boxX, boxY - 8.0f, boxWidth); break;
            case 4: DrawHealthBarBelowFullBox(player.Health, 200, boxX, boxY, boxWidth, boxHeight); break;
            }
        }

        DrawNameTag(player, headPos, boxWidth, boxHeight);
    }

    if (g_Globals.AimBot.ShowFOV && g_Globals.AimBot.AimbotRage)
    {
        ImVec2 center(g_Globals.EspConfig.Width / 2.0f, g_Globals.EspConfig.Height / 2.0f);
        float radius = g_Globals.AimBot.RageFov;
        ImU32 color = IM_COL32(255, 0, 0, 200); // Red circle
        auto drawList = ImGui::GetForegroundDrawList();
        drawList->AddCircle(center, radius, color, 0, 2.0f);
    }
}
