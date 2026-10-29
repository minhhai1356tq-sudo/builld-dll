#pragma comment(lib, "wininet.lib")

#include "Interface.hpp"
#include <Cheat/Cheat.hpp>
#include <Cheat/ConfigSystem.hpp>
#include <Cheat/DownLocalPlayer.hpp>
#include <Cheat/DownPlayer.hpp>
#include <Cheat/EnemyPull360.hpp>
#include <Cheat/SpinBot.hpp>
#include <Cheat/TeleportMark.hpp>
#include <Cheat/FlyHack.hpp>
#include <Cheat/SilentAim.hpp>
#include <io.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <thread>
#include <atomic>
#include <windows.h>
#include <wininet.h>
#include "Globals.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImFont* DefaultESPFont = nullptr;
ImFont* Verdana = nullptr;
ImFont* SmallFonts = nullptr;
ImFont* EspFont = nullptr;

#include "Bytes.hpp"
#include "networko.hpp"
#include <ImGui/freetype.h>
#include "settings.h"
#include "fonts.h"
#include <internal/Cheat ff/Offsets.h>
#include <internal/Adb/Adb.hpp>
#include <Cheat/notifications.h>

void AdbFreeFireMax()
{

    notificationSystem.Notification("Phantom Zone", "Connecting...", ImColor(100, 200, 255));

    adb::TerminateAdbProcesses();
    adb::ExecuteADBRawCommand("kill-server");
    Sleep(500);
    
    // Connect to extra ports
    adb::ExecuteADBRawCommand("connect 127.0.0.1:5556");
    adb::ExecuteADBRawCommand("connect 127.0.0.1:5557");
    adb::ExecuteADBRawCommand("connect 127.0.0.1:5565");
    Sleep(500);
    
    adb::ExecuteADBRawCommand("devices");
    Sleep(500);

    bool GetDir = adb::ChangeDirectory(adb::GetExecutableDirectory());
    if (!GetDir)
    {

        notificationSystem.Notification("Phantom Zone", "Dir failed", ImColor(255, 80, 80));
        g_Globals.General.CurrentGameVersion = 0;
        return;
    }


    std::string il2cppStr = adb::ExecuteShellCommandNoSu(
        "cat /proc/$(pidof com.dts.freefiremax)/maps | grep libil2cpp.so | sed -n '2p'");

    if (il2cppStr.empty())
    {

        notificationSystem.Notification("Phantom Zone", "il2cpp not found", ImColor(255, 150, 0));
        g_Globals.General.CurrentGameVersion = 0;
        return;
    }

    OffsetsffmaxBeta::Il2Cpp = adb::ConvertToUintPtr(il2cppStr);

    if (OffsetsffmaxBeta::Il2Cpp == 0)
    {

        notificationSystem.Notification("Phantom Zone", "il2cpp = 0", ImColor(255, 80, 80));
        g_Globals.General.CurrentGameVersion = 0;
        return;
    }

    g_Globals.General.hookstatus = true;
    g_Globals.General.CurrentGameVersion = 2;


    notificationSystem.Notification("Phantom Zone", "Connected!", ImColor(0, 255, 150));

    Sleep(1000);
}

void AdbFreeFireNormal()
{

    notificationSystem.Notification("Free Fire", "Connecting...", ImColor(100, 200, 255));

    adb::TerminateAdbProcesses();
    adb::ExecuteADBRawCommand("kill-server");
    Sleep(500);
    
    // Connect to extra ports
    adb::ExecuteADBRawCommand("connect 127.0.0.1:5556");
    adb::ExecuteADBRawCommand("connect 127.0.0.1:5557");
    adb::ExecuteADBRawCommand("connect 127.0.0.1:5565");
    Sleep(500);
    
    adb::ExecuteADBRawCommand("devices");
    Sleep(500);

    bool GetDir = adb::ChangeDirectory(adb::GetExecutableDirectory());
    if (!GetDir)
    {

        notificationSystem.Notification("Free Fire", "Dir failed", ImColor(255, 80, 80));
        g_Globals.General.CurrentGameVersion = 0;
        return;
    }


    std::string il2cppStr = adb::ExecuteShellCommandNoSu(
        "cat /proc/$(pidof com.dts.freefireth)/maps | grep libil2cpp.so ");

    if (il2cppStr.empty())
    {

        notificationSystem.Notification("Free Fire", "il2cpp not found", ImColor(255, 150, 0));
        g_Globals.General.CurrentGameVersion = 0;
        return;
    }

    OffsetsffNormal::Il2Cpp = adb::ConvertToUintPtr(il2cppStr);

    if (OffsetsffNormal::Il2Cpp == 0)
    {

        notificationSystem.Notification("Free Fire", "il2cpp = 0", ImColor(255, 80, 80));
        g_Globals.General.CurrentGameVersion = 0;
        return;
    }

    g_Globals.General.hookstatus = true;
    g_Globals.General.CurrentGameVersion = 1;


    notificationSystem.Notification("Free Fire", "Connected!", ImColor(0, 255, 150));

    Sleep(1000);
}

namespace FrameWork
{
    void RenderCircleWithBorder(ImVec2 center, float radius,
        ImU32 fill_color, ImU32 border_color, float border_thickness)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return;

        ImDrawList* draw_list = window->DrawList;
        draw_list->AddCircleFilled(center, radius, fill_color);
        draw_list->AddCircle(center, radius, border_color, 0, border_thickness);

        static ImVec2 particle_pos[300];
        ImGui::GetWindowDrawList()->AddShadowCircle(particle_pos[1], 1.f, ImColor(220, 110, 90), 4 * 240, ImVec2(0, -90), 0, 1);
        ImGui::GetWindowDrawList()->AddShadowCircle(particle_pos[1], 1.f, ImColor(220, 110, 90), 4 * 40, ImVec2(825, 30), 0, 1);
    }

    void Interface::UpdateStyle()
    {
        ImGuiStyle* Style = &ImGui::GetStyle();
        ImGuiIO& io = ImGui::GetIO();

        Style->WindowRounding = 7;
        Style->WindowBorderSize = 1;
        Style->WindowPadding = ImVec2(0, 0);
        Style->WindowShadowSize = 0;
        Style->ScrollbarSize = 3;
        Style->ScrollbarRounding = 0;
        Style->PopupRounding = 5;

        Style->Colors[ImGuiCol_Separator] = ImColor(50, 50, 50, 255);
        Style->Colors[ImGuiCol_SeparatorActive] = ImColor(70, 70, 70, 255);
        Style->Colors[ImGuiCol_SeparatorHovered] = ImColor(60, 60, 60, 255);
        Style->Colors[ImGuiCol_ResizeGrip] = ImColor(30, 30, 30, 255);
        Style->Colors[ImGuiCol_ResizeGripActive] = ImColor(70, 70, 70, 255);
        Style->Colors[ImGuiCol_ResizeGripHovered] = ImColor(50, 50, 50, 255);
        Style->Colors[ImGuiCol_PopupBg] = ImColor(10, 10, 10, 255);
        Style->Colors[ImGuiCol_ScrollbarBg] = ImColor(20, 20, 20, 255);
        Style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(50, 50, 50, 255);
        Style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(90, 90, 90, 255);
        Style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(70, 70, 70, 255);
        Style->Colors[ImGuiCol_WindowBg] = ImColor(10, 10, 10, 255);
        Style->Colors[ImGuiCol_Border] = ImColor(50, 50, 50, 255);
        Style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
        Style->Colors[ImGuiCol_FrameBg] = ImColor(30, 30, 30, 255);
        Style->Colors[ImGuiCol_FrameBgHovered] = ImColor(50, 50, 50, 255);
        Style->Colors[ImGuiCol_FrameBgActive] = ImColor(70, 70, 70, 255);
        Style->Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);
        Style->Colors[ImGuiCol_SliderGrab] = ImColor(50, 50, 50, 255);
        Style->Colors[ImGuiCol_SliderGrabActive] = ImColor(90, 90, 90, 255);

        DefaultESPFont = io.Fonts->AddFontFromMemoryTTF(neverloseFont, sizeof(neverloseFont), 12.f, 0, io.Fonts->GetGlyphRangesCyrillic());
        SmallFonts = io.Fonts->AddFontFromMemoryTTF(pixelmix_ttf, sizeof(pixelmix_ttf), 10.f, 0, io.Fonts->GetGlyphRangesCyrillic());

        io.LogFilename = nullptr;
        Assets::Initialize(IDevice);
    }
    void Interface::ApplyWindowStyle(bool menuOpen)
    {
        if (!hWindow) return;

        LONG exStyle = GetWindowLong(hWindow, GWL_EXSTYLE);
        exStyle |= WS_EX_TOPMOST | WS_EX_LAYERED;
        exStyle &= ~WS_EX_TOOLWINDOW;

        if (menuOpen)
        {
            exStyle &= ~WS_EX_TRANSPARENT;
            SetWindowLong(hWindow, GWL_EXSTYLE, exStyle);
            SetLayeredWindowAttributes(hWindow, RGB(0, 0, 0), 255, LWA_ALPHA);
            SetForegroundWindow(hWindow);
            SetFocus(hWindow);
        }
        else
        {
            exStyle |= WS_EX_TRANSPARENT;
            SetWindowLong(hWindow, GWL_EXSTYLE, exStyle);
            SetLayeredWindowAttributes(hWindow, RGB(0, 0, 0), 255, LWA_ALPHA);
            if (hTargetWindow && IsWindow(hTargetWindow))
                SetForegroundWindow(hTargetWindow);
        }

        RedrawWindow(hWindow, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASENOW);
    }

    void Interface::HandleMenuKey()
    {
        int menuKey = g_Globals.General.MenuKey;

        if (menuKey == 0) {
            menuKey = VK_INSERT;
        }

        bool isDown = (GetAsyncKeyState(menuKey) & 0x8000) != 0;

        if (isDown && !wasKeyDown)
        {
            bIsMenuOpen = !bIsMenuOpen;
            ApplyWindowStyle(bIsMenuOpen);
        }

        wasKeyDown = isDown;
    }

    static void DrawTabAccent()
    {
        ImGui::GetWindowDrawList()->AddShadowCircle(
            ImGui::GetWindowPos(), 280,
            ImColor(0, 0, 0), 470,
            ImVec2(910, 675), 440,
            ImColor(0.f, 0.f, 0.f, 255.f));
    }

    static int selectedVersion = 0;
    static bool isConnecting = false;
    static int OldCurrentTab = 0;
    void Interface::AimbotTab()
    {
        ImGui::SetCursorPos(ImVec2(20, 20));
        ImGui::CustomChild("Aimbot", ImVec2(320, 400));
        {
            ImGui::Checkbox("Aimbot Legit (AI)", &g_Globals.AimBot.AimbotLegit);
            ImGui::SliderInt("Delay Aimbot (ms)", &g_Globals.AimBot.AimbotAIDelay, 1, 1000, "%d ms");
            ImGui::Spacing(); ImGui::Spacing();
            ImGui::Checkbox("Ignore Knocked", &g_Globals.AimBot.IgnoreKnocked);
            ImGui::Separator();
            ImGui::Checkbox("Silent Aim", &g_Globals.AimBot.SilentAimEnabled);
            ImGui::KeyBind("Silent Aim Bind", &g_Globals.AimBot.SilentAimBind);
            const char* silentModes[] = { "Normal", "360" };
            ImGui::Combo("Silent Aim Mode", &g_Globals.AimBot.SilentAimMode, silentModes, IM_ARRAYSIZE(silentModes));

            if (g_Globals.AimBot.SilentAimMode == 0) { // Normal mode
                ImGui::SliderInt("Silent Aim FOV", &g_Globals.AimBot.SilentAimFov, 1, 720);
                
                // Logic to make HeadRate + BodyRate always = 100
                int prevHead = g_Globals.AimBot.HeadRate;
                ImGui::SliderInt("Head Rate", &g_Globals.AimBot.HeadRate, 0, 100);
                if (g_Globals.AimBot.HeadRate != prevHead) {
                    g_Globals.AimBot.BodyRate = 100 - g_Globals.AimBot.HeadRate;
                }
                
                int prevBody = g_Globals.AimBot.BodyRate;
                ImGui::SliderInt("Body Rate", &g_Globals.AimBot.BodyRate, 0, 100);
                if (g_Globals.AimBot.BodyRate != prevBody) {
                    g_Globals.AimBot.HeadRate = 100 - g_Globals.AimBot.BodyRate;
                }

                ImGui::Checkbox("Silent Ignore Knocked", &g_Globals.AimBot.SilentIgnoreKnocked);
                
                // FOV Circle options
                ImGui::Checkbox("Show FOV Circle", &g_Globals.AimBot.ShowFovCircle);
                ImGui::SliderFloat("FOV Circle Thickness", &g_Globals.AimBot.fovThickness, 1.0f, 10.0f);
                ImGui::Checkbox("RGB Mode", &g_Globals.AimBot.RgbMode);
            } else { // 360 mode
                ImGui::Checkbox("Silent Ignore Knocked", &g_Globals.AimBot.SilentIgnoreKnocked);
            }

            ImGui::SliderInt("Silent Aim Max Distance", &g_Globals.AimBot.SilentAimMaxDistance, 1, 1000);
        }
        ImGui::EndCustomChild();

        ImGui::SetCursorPos(ImVec2(355, 20));
        ImGui::CustomChild("Free Fire Selection", ImVec2(ImGui::GetWindowSize().x - 395, 250));
        {
            const char* versions[] = { "Free Fire Normal", "Free Fire Max Beta/x86" };

            if (ImGui::Combo("Select Your Free Fire Version:", &selectedVersion, versions, IM_ARRAYSIZE(versions)))
            {
                isConnecting = false;
            }

            if (!isConnecting)
            {
                if (ImGui::Button("Hook Emulator", ImVec2(ImGui::GetWindowSize().x - 40, 35)))
                {
                    isConnecting = true;

                    if (selectedVersion == 0)
                    {
                        std::thread([=]() {
                            AdbFreeFireNormal();
                            isConnecting = false;
                            }).detach();
                    }
                    else if (selectedVersion == 1)
                    {
                        std::thread([=]() {
                            AdbFreeFireMax();
                            isConnecting = false;
                            }).detach();
                    }
                }
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));
                ImGui::Button("Connecting...", ImVec2(ImGui::GetWindowSize().x - 40, 35));
                ImGui::PopStyleColor();
            }

            ImGui::Spacing();
            ImGui::Checkbox("No Recoil", &g_Globals.AimBot.NoRecoil);
            ImGui::Checkbox("Fast Switch", &g_Globals.AimBot.FastSwitch);
            ImGui::Checkbox("Fast Reload", &g_Globals.AimBot.FastReload);
        }
        ImGui::EndCustomChild();

        DrawTabAccent();
    }

    
    void Interface::VisualsTab()
    {
        constexpr ImGuiColorEditFlags kColorFlags = ImGuiColorEditFlags_NoDragDrop |ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs;

        ImGui::SetCursorPos(ImVec2(20, 20));
        ImGui::CustomChild("Options", ImVec2(320, 230));
        {
            ImGui::Checkbox("Enabled", &g_Globals.Visuals.Enable);
        ImGui::SliderInt("Render Distance", &g_Globals.Visuals.DistanceEsp, 0, 600, "%dm");
        ImGui::Spacing(); ImGui::Spacing();
        ImGui::Checkbox("Show LocalPlayer", &g_Globals.EspConfig.visible);
        ImGui::Checkbox("Show NPC", &g_Globals.Visuals.Enemy);
        ImGui::Checkbox("Show Visible Only", &g_Globals.EspConfig.visible);
        ImGui::Checkbox("ESP Invisible", &g_Globals.Visuals.EspInvisible);
        }
        ImGui::EndCustomChild();

        ImGui::SetCursorPos(ImVec2(20, 275));
        ImGui::CustomChild("Positions", ImVec2(320, 160));
        {
            ImGui::Combo("HealthBar", &g_Globals.Visuals.players_healthbar, "None\0Left\0Right\0Bottom\0");
            ImGui::Spacing();
            ImGui::Combo("ArmorBar", &g_Globals.Visuals.players_shieldbar, "None\0Left\0Right\0Bottom\0");
        }
        ImGui::EndCustomChild();

        ImGui::SetCursorPos(ImVec2(355, 20));
        ImGui::CustomChild("Esp", ImVec2(300, 490));
        {
            ImGui::Checkbox("Box", &g_Globals.Visuals.Box);
            ImGui::Checkbox("Head", &g_Globals.Visuals.HeadDot);
            ImGui::Checkbox("Skeleton", &g_Globals.Visuals.Skeleton);
            ImGui::Checkbox("Filled Box", &g_Globals.Visuals.FilledBox);
            ImGui::Checkbox("Info", &g_Globals.Visuals.ESPInfo);
            ImGui::Checkbox("Weapon Name", &g_Globals.Visuals.ESPWeapon);
            ImGui::Checkbox("SnapLines", &g_Globals.Visuals.Lines);
            ImGui::Checkbox("Health Bar", &g_Globals.Visuals.HealthBar);
            ImGui::Checkbox("Armor Bar", &g_Globals.Visuals.ShieldBar);
            ImGui::Checkbox("Knocked", &g_Globals.Visuals.ShowDead);
            ImGui::Checkbox("Bot Tag", &g_Globals.Visuals.ID);
            ImGui::Spacing();

            ImGui::Separator();
            ImGui::Spacing();
            ImGui::ColorEdit4("Visible Color", g_Globals.Visuals.EnemyColor, kColorFlags);
            ImGui::ColorEdit4("Not Visible Color", g_Globals.Visuals.WatermarkColor, kColorFlags);
            ImGui::ColorEdit4("Skeleton Color", g_Globals.Visuals.SkeletonColor, kColorFlags);
            ImGui::ColorEdit4("Filled Box Color", g_Globals.Visuals.Filledboxcolor, kColorFlags);
            ImGui::ColorEdit4("Name Color", g_Globals.Visuals.NameColor, kColorFlags);
            ImGui::ColorEdit4("Head Color", g_Globals.Visuals.HeadDotColor, kColorFlags);
            ImGui::ColorEdit4("Weapon Name Color", g_Globals.Visuals.ESPWeaponColor, kColorFlags);
            ImGui::ColorEdit4("Distance Color", g_Globals.Visuals.DistColor, kColorFlags);
            ImGui::ColorEdit4("Snaplines Color", g_Globals.Visuals.LinesColor, kColorFlags);
            ImGui::ColorEdit4("Health Bar Color", g_Globals.Visuals.texthColor, kColorFlags);
            ImGui::ColorEdit4("Knocked Color", g_Globals.Visuals.KnockedColor, kColorFlags);
        }
        ImGui::EndCustomChild();

        DrawTabAccent();
    }

    void Interface::MiscellaneousTab() {
        // Spin Bot, Teleport Mark và Fly Hack
        ImGui::SetCursorPos(ImVec2(20, 20));
        ImGui::CustomChild(("Spin Bot"), ImVec2(320, 200));
        {
            ImGui::Checkbox(("Enabled"), &g_Globals.Exploits.SpinBot);
            ImGui::KeyBind(("Keybind"), &g_Globals.Exploits.SpinBotBind);
            ImGui::SliderInt(("Spin Speed"), &g_Globals.Exploits.SpinSpeed, 1, 720);
        }
        ImGui::EndCustomChild();

        ImGui::SetCursorPos(ImVec2(20, 240));
        ImGui::CustomChild(("Teleport Mark"), ImVec2(320, 155));
        {
            ImGui::Checkbox(("Enabled"), &g_Globals.Exploits.TeleportMark);
            ImGui::KeyBind(("Keybind"), &g_Globals.Exploits.TeleportMarkBind);
        }
        ImGui::EndCustomChild();

        ImGui::SetCursorPos(ImVec2(355, 20));
        ImGui::CustomChild(("Fly Hack"), ImVec2(320, 155));
        {
            ImGui::Checkbox(("Enabled"), &g_Globals.Exploits.FlyHack);
            ImGui::KeyBind(("Keybind"), &g_Globals.Exploits.FlyHackBind);
        }
        ImGui::EndCustomChild();

        DrawTabAccent();
    }

    void Interface::VehiclesTab()
    {
        constexpr ImGuiColorEditFlags kColorFlags =
            ImGuiColorEditFlags_NoDragDrop |
            ImGuiColorEditFlags_AlphaBar |
            ImGuiColorEditFlags_NoTooltip |
            ImGuiColorEditFlags_NoInputs;

        static ImGuiTextFilter mFilter;

        ImGui::SetCursorPos(ImVec2(20, 20));
        ImGui::CustomChild(("Vehicle List"), ImVec2(320, 350));
        {
            mFilter.Draw(("Search for a Vehicle"));
            if (ImGui::BeginListBox("##VehList", ImVec2(ImGui::GetWindowSize().x - 20, 300)))
                ImGui::EndListBox();
        }
        ImGui::EndCustomChild();

        ImGui::SetCursorPos(ImVec2(355, 20));
        ImGui::CustomChild(("Actions"), ImVec2(300, 285));
        {
            ImGui::Checkbox("Down Local Player", &g_Globals.Exploits.DownLocalPlayer);
            ImGui::KeyBind("Keybind", &g_Globals.Exploits.DownLocalPlayerBind);
            
            ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
            
            ImGui::Checkbox("Spin Bot", &g_Globals.Exploits.SpinBot);
            ImGui::KeyBind("Spin Bot Bind", &g_Globals.Exploits.SpinBotBind);
            ImGui::SliderInt("Spin Speed", &g_Globals.Exploits.SpinSpeed, 1, 720);

            ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
            
            ImGui::Checkbox("Teleport Mark", &g_Globals.Exploits.TeleportMark);
            ImGui::KeyBind("Teleport Mark Bind", &g_Globals.Exploits.TeleportMarkBind);
        }
        ImGui::EndCustomChild();

        ImGui::SetCursorPos(ImVec2(355, 330));
        ImGui::CustomChild(("Visuals"), ImVec2(300, 200));
        {
            ImGui::Checkbox("Enabled", &g_Globals.Visuals.Enable);
            ImGui::Checkbox("Marker", &g_Globals.Visuals.RadarHack);
            ImGui::Checkbox("Name", &g_Globals.Visuals.Name);
            ImGui::Checkbox("Distance", &g_Globals.Visuals.Distance);
            ImGui::ColorEdit4("Color", g_Globals.Visuals.EnemyColor, kColorFlags);
        }
        ImGui::EndCustomChild();

        DrawTabAccent();
    }

    void Interface::PlayersTab()
    {
        static ImGuiTextFilter mFilter;

        ImGui::SetCursorPos(ImVec2(20, 20));
        ImGui::CustomChild(("Player List"), ImVec2(320, 350));
        {
            mFilter.Draw(("Search for a player"));
            if (ImGui::BeginListBox("##PlayerList", ImVec2(ImGui::GetWindowSize().x - 20, 300)))
                ImGui::EndListBox();
        }
        ImGui::EndCustomChild();

        ImGui::SetCursorPos(ImVec2(355, 20));
        ImGui::CustomChild(("Selected Player Information"), ImVec2(300, 130));
        {
        }
        ImGui::EndCustomChild();

        ImGui::SetCursorPos(ImVec2(355, 180));
        ImGui::CustomChild(("Actions"), ImVec2(300, 110));
        {
        }
        ImGui::EndCustomChild();

        DrawTabAccent();
    }

    // Hàm escape ký tự đặc biệt cho JSON
std::string EscapeJsonString(const std::string& s) {
    std::string o;
    for (auto c : s) {
        switch (c) {
            case '"':  o += "\\\""; break;
            case '\\': o += "\\\\"; break;
            case '\b': o += "\\b"; break;
            case '\f': o += "\\f"; break;
            case '\n': o += "\\n"; break;
            case '\r': o += "\\r"; break;
            case '\t': o += "\\t"; break;
            default:
                if ('\x00' <= c && c <= '\x1f') {
                    char hex[6];
                    sprintf_s(hex, "\\u%04x", (int)c);
                    o += hex;
                } else {
                    o += c;
                }
        }
    }
    return o;
}

// Hàm gửi Discord webhook
void SendDiscordFeedback() {
    std::string discordName = g_Globals.General.FeedbackDiscordName;
    std::string feedbackText = g_Globals.General.FeedbackText;
    int stars = g_Globals.General.FeedbackStars;

    // Escape các ký tự đặc biệt
    discordName = EscapeJsonString(discordName);
    feedbackText = EscapeJsonString(feedbackText);

    // Tạo JSON payload (minify để nhanh hơn)
    std::string starsStr = std::string(stars, '⭐');
    std::string json = "{\"embeds\":[{\"title\":\"**[NEW FEEDBACK REPORT]**\",\"color\":5814783,\"fields\":[{\"name\":\"**[ TÊN NGƯỜI DÙNG DISCORD]**\",\"value\":\"" + discordName + "\",\"inline\":false},{\"name\":\"**[ SỐ SAO ĐÁNH GIÁ ]**\",\"value\":\"" + starsStr + "\",\"inline\":false},{\"name\":\"**[ NHẬN XÉT ]**\",\"value\":\"" + feedbackText + "\",\"inline\":false}]}]}";

    // Gửi request bằng WinINet
    HINTERNET hInternet = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        notificationSystem.Notification("Feedback", "Lỗi kết nối!", ImColor(255, 0, 0));
        return;
    }

    HINTERNET hConnect = InternetConnectA(hInternet, "discord.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) { 
        InternetCloseHandle(hInternet); 
        notificationSystem.Notification("Feedback", "Lỗi kết nối!", ImColor(255, 0, 0));
        return; 
    }

    HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", "/api/webhooks/1503237334777462916/AnMAcrAAfP3BkEzTOoqC_GrxV0iEByPk52QztOBRd9S7jGmehY9MnWja5RzcNJRWgwig", NULL, NULL, NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);
    if (!hRequest) { 
        InternetCloseHandle(hConnect); 
        InternetCloseHandle(hInternet); 
        notificationSystem.Notification("Feedback", "Lỗi kết nối!", ImColor(255, 0, 0));
        return; 
    }

    std::string headers = "Content-Type: application/json\r\nUser-Agent: Mozilla/5.0\r\n";
    HttpAddRequestHeadersA(hRequest, headers.c_str(), headers.length(), HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

    if (HttpSendRequestA(hRequest, NULL, 0, (LPVOID)json.c_str(), (DWORD)json.length())) {
        notificationSystem.Notification("Feedback", "Gửi feedback thành công!", ImColor(0, 255, 0));
        // Reset form sau khi gửi
        memset(g_Globals.General.FeedbackDiscordName, 0, sizeof(g_Globals.General.FeedbackDiscordName));
        memset(g_Globals.General.FeedbackText, 0, sizeof(g_Globals.General.FeedbackText));
        g_Globals.General.FeedbackStars = 5;
    } else {
        DWORD err = GetLastError();
        char errMsg[256];
        sprintf_s(errMsg, "Lỗi: %d", err);
        notificationSystem.Notification("Feedback", errMsg, ImColor(255, 0, 0));
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}

void Interface::SettingsTab()
{
    ImGui::SetCursorPos(ImVec2(20, 20));
    ImGui::CustomChild("Actions", ImVec2(300, 230));
    {
        ImGui::SliderInt("Update Delay", &g_Globals.General.Delay, 1, 100, "%d ms");
        ImGui::Spacing(); ImGui::Spacing();
        ImGui::Checkbox("Stream Mode", &g_Globals.General.Capture);
        ImGui::Spacing(); ImGui::Spacing();
        ImGui::KeyBind("Menu Bind", &g_Globals.General.MenuKey);
        ImGui::Spacing();
        if (ImGui::Button("Exit"))
            exit(0);
    }
    ImGui::EndCustomChild();

    // Feedback Form
    ImGui::SetCursorPos(ImVec2(355, 20));
    ImGui::CustomChild("Feedback", ImVec2(ImGui::GetWindowSize().x - 395, 430));
    {
        ImGui::Text("📝 Gửi Feedback");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Tên Discord:");
        ImGui::InputText("##discordname", g_Globals.General.FeedbackDiscordName, sizeof(g_Globals.General.FeedbackDiscordName), ImGuiInputTextFlags_AutoSelectAll);

        ImGui::Spacing();
        ImGui::Text("Đánh giá (1-5⭐):");
        ImGui::SliderInt("##stars", &g_Globals.General.FeedbackStars, 1, 5);

        ImGui::Spacing();
        ImGui::Text("Nhận xét:");
        ImGui::InputTextMultiline("##feedback", g_Globals.General.FeedbackText, sizeof(g_Globals.General.FeedbackText), ImVec2(0, 150), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_AllowTabInput);

        ImGui::Spacing();
        if (ImGui::Button("Gửi Feedback", ImVec2(ImGui::GetContentRegionAvail().x, 40))) {
            // Gửi trong thread riêng để không làm đơ menu
            std::thread(SendDiscordFeedback).detach();
        }
    }
    ImGui::EndCustomChild();

    DrawTabAccent();
}

    // Helper functions for saving/loading license key
    void SaveLicenseKey() {
        std::ofstream file("license.txt");
        if (file.is_open()) {
            file << g_Globals.General.License;
            file.close();
        }
    }

    void LoadLicenseKey() {
        std::ifstream file("license.txt");
        if (file.is_open()) {
            file.getline(g_Globals.General.License, 255);
            file.close();
        }
    }

    #include <shlobj.h>

    std::string GetHWID() {
        DWORD serial = 0;
        GetVolumeInformationA("C:\\", NULL, 0, &serial, NULL, NULL, NULL, 0);
        return std::to_string(serial);
    }

    std::string GetVietnamTime() {
        time_t t = time(nullptr);
        struct tm tmInfo;
        localtime_s(&tmInfo, &t);
        char buffer[80];
        strftime(buffer, 80, "%d/%m/%Y %H:%M:%S", &tmInfo);
        return buffer;
    }

    void WriteLog(std::string key, std::string hwid, std::string url, std::string response, bool isValid) {
        char desktopPath[MAX_PATH];
        SHGetFolderPathA(NULL, CSIDL_DESKTOP, NULL, 0, desktopPath);
        std::string logPath = std::string(desktopPath) + "\\Ptom.txt";

        // Mở file với chế độ ghi đè (xóa cũ)
        std::ofstream logFile(logPath, std::ios::trunc);
        if (logFile.is_open()) {
            logFile << "==========================\n";
            logFile << "Thời gian: " << GetVietnamTime() << "\n";
            logFile << "Key: " << key << "\n";
            logFile << "HWID: " << hwid << "\n";
            logFile << "Response: " << response << "\n";
            logFile << "Valid: " << (isValid ? "YES" : "NO") << "\n";
            logFile << "==========================\n\n";
            logFile.close();
        }
    }

    std::string UrlEncode(std::string value) {
        std::string result;
        for (char c : value) {
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                result += c;
            } else {
                char buf[4];
                sprintf_s(buf, "%%%02X", (unsigned char)c);
                result += buf;
            }
        }
        return result;
    }

    bool VerifyKey(std::string key) {
        std::string hwid = GetHWID();
        std::string url = "http://hieusbeosauth.onlinewebshop.net/verify.php?owner=nam khanh iu hiu lam&api=ffx86&key=" + key + "&hwid=" + hwid;

        HINTERNET hInt = InternetOpenA("App", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInt) {
            WriteLog(key, hwid, url, "Loi mo InternetOpen", false);
            return false;
        }

        HINTERNET hUrl = InternetOpenUrlA(hInt, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (!hUrl) {
            InternetCloseHandle(hInt);
            WriteLog(key, hwid, url, "Loi mo URL", false);
            return false;
        }

        std::string res = "";
        char buf[1024];
        DWORD bytesRead = 0;
        while (InternetReadFile(hUrl, buf, 1023, &bytesRead) && bytesRead > 0) {
            buf[bytesRead] = 0;
            res += buf;
        }

        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInt);

        bool isValid = (res.find("\"valid\": true") != std::string::npos || res.find("\"valid\":true") != std::string::npos);
        WriteLog(key, hwid, url, res, isValid);

        return isValid;
    }

    void Interface::RenderGui()
    {
        static bool loadedLicense = false;
        static bool triedAutoLogin = false;
        static std::string loginStatus = "";
        static bool prevDownLocalPlayer = false;
        static bool prevDownPlayer = false;
        static bool prevEnemyPullEnabled = false;
        static bool prevSpinBot = false;
        static bool prevSilentAim = false;
        static int frameSkip = 0;
        
        // Khởi động SpinBot thread 1 lần
        static bool spinBotStarted = false;
        if (!spinBotStarted) {
            SpinBot::SpinBotExploit::Start();
            spinBotStarted = true;
        }
        // Khởi động TeleportMark thread 1 lần
        static bool teleportMarkStarted = false;
        if (!teleportMarkStarted) {
            InternalTeleportMark::Start();
            teleportMarkStarted = true;
        }
        // Khởi động FlyHack thread 1 lần
        static bool flyHackStarted = false;
        if (!flyHackStarted) {
            InternalFlyMini::Start();
            flyHackStarted = true;
        }
        // Khởi động Silent Aim thread 1 lần
        static bool silentAimStarted = false;
        if (!silentAimStarted) {
            SilentAimLoop::StartLoop();
            silentAimStarted = true;
        }

        if (!loadedLicense) {
            LoadLicenseKey();
            loadedLicense = true;
        }

        // Auto verify saved key on first run
        if (!triedAutoLogin && strlen(g_Globals.General.License) > 0) {
            triedAutoLogin = true;
            // We'll verify on first render
        }

        ImGuiIO& io = ImGui::GetIO();
        notificationSystem.DrawNotifications();
        g_Globals.EspConfig.Width = (int)io.DisplaySize.x;
        g_Globals.EspConfig.Height = (int)io.DisplaySize.y;

        if (g_Globals.EspConfig.Width <= 0 || g_Globals.EspConfig.Height <= 0) {
            RECT rect;
            if (GetWindowRect(hTargetWindow, &rect)) {
                g_Globals.EspConfig.Width = rect.right - rect.left;
                g_Globals.EspConfig.Height = rect.bottom - rect.top;
            }
        }

        // Draw enemy count (luôn hiển thị, kể cả khi menu đóng)
        int alivePlayerCount = 0;
        for (const auto& pair : g_Globals.EspConfig.Entities) {
            const Player& player = pair.second;
            // Không đếm đồng đội, không đếm dead/knocked
            if (player.IsTeam == Player::Bool3::False && !player.IsDead && !player.IsKnocked) {
                alivePlayerCount++;
            }
        }

        std::string enemyCountText = "Players : " + std::to_string(alivePlayerCount);
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        ImVec2 textSize = ImGui::CalcTextSize(enemyCountText.c_str());
        ImVec2 textPos = ImVec2(
            (displaySize.x - textSize.x) * 0.5f,
            displaySize.y * 0.05f // trên cùng giữa màn hình
        );

        auto drawList = ImGui::GetBackgroundDrawList();
        ImFont* smallFont = SmallFonts ? SmallFonts : io.FontDefault;
        drawList->AddText(
            smallFont,
            16.0f,
            textPos,
            IM_COL32(255, 0, 0, 255), // màu đỏ
            enemyCountText.c_str());

        // --- Check for toggles and hotkeys (works even when menu is hidden) ---
        
        // Check for Down LocalPlayer toggle
        if (g_Globals.Exploits.DownLocalPlayer != prevDownLocalPlayer) {
            if (g_Globals.Exploits.DownLocalPlayer) {
                notificationSystem.Notification("Down LocalPlayer", "Enabled", LightGreen);
            } else {
                notificationSystem.Notification("Down LocalPlayer", "Disabled", SoftRed);
            }
            prevDownLocalPlayer = g_Globals.Exploits.DownLocalPlayer;
        }

        if (g_Globals.Exploits.DownPlayer != prevDownPlayer) {
            if (g_Globals.Exploits.DownPlayer) {
                notificationSystem.Notification("Down Player", "Enabled", LightGreen);
            } else {
                notificationSystem.Notification("Down Player", "Disabled", SoftRed);
            }
            prevDownPlayer = g_Globals.Exploits.DownPlayer;
        }

        // Check for Pull Enemy toggle
        if (g_Globals.Exploits.EnemyPullEnabled != prevEnemyPullEnabled) {
            if (g_Globals.Exploits.EnemyPullEnabled) {
                notificationSystem.Notification("Pull Enemy", "Enabled", LightGreen);
            } else {
                notificationSystem.Notification("Pull Enemy", "Disabled", SoftRed);
            }
            prevEnemyPullEnabled = g_Globals.Exploits.EnemyPullEnabled;
        }

        // Hotkey check for Spin Bot
        if (g_Globals.Exploits.SpinBotBind != 0 && (GetAsyncKeyState(g_Globals.Exploits.SpinBotBind) & 1)) {
            g_Globals.Exploits.SpinBot = !g_Globals.Exploits.SpinBot;
        }
        
        // Check for SpinBot toggle
        if (g_Globals.Exploits.SpinBot != prevSpinBot) {
            if (g_Globals.Exploits.SpinBot) {
                notificationSystem.Notification("Spin Bot", "Enabled", LightGreen);
            } else {
                notificationSystem.Notification("Spin Bot", "Disabled", SoftRed);
            }
            prevSpinBot = g_Globals.Exploits.SpinBot;
        }

        // Hotkey check for Teleport Mark
        static bool prevTeleportMark = false;
        if (g_Globals.Exploits.TeleportMarkBind != 0 && (GetAsyncKeyState(g_Globals.Exploits.TeleportMarkBind) & 1)) {
            g_Globals.Exploits.TeleportMark = !g_Globals.Exploits.TeleportMark;
        }
        
        // Check for Teleport Mark toggle
        if (g_Globals.Exploits.TeleportMark != prevTeleportMark) {
            if (g_Globals.Exploits.TeleportMark) {
                notificationSystem.Notification("Teleport Mark", "Enabled", LightGreen);
            } else {
                notificationSystem.Notification("Teleport Mark", "Disabled", SoftRed);
            }
            prevTeleportMark = g_Globals.Exploits.TeleportMark;
        }

        // Hotkey check for Fly Hack
        static bool prevFlyHack = false;
        if (g_Globals.Exploits.FlyHackBind != 0 && (GetAsyncKeyState(g_Globals.Exploits.FlyHackBind) & 1)) {
            g_Globals.Exploits.FlyHack = !g_Globals.Exploits.FlyHack;
        }
        
        // Check for Fly Hack toggle
        if (g_Globals.Exploits.FlyHack != prevFlyHack) {
            if (g_Globals.Exploits.FlyHack) {
                notificationSystem.Notification("Fly Hack", "Enabled", LightGreen);
            } else {
                notificationSystem.Notification("Fly Hack", "Disabled", SoftRed);
            }
            prevFlyHack = g_Globals.Exploits.FlyHack;
        }

        // Hotkey check for Silent Aim
        if (g_Globals.AimBot.SilentAimBind != 0 && (GetAsyncKeyState(g_Globals.AimBot.SilentAimBind) & 1)) {
            g_Globals.AimBot.SilentAimEnabled = !g_Globals.AimBot.SilentAimEnabled;
        }

        // Check for Silent Aim toggle
        if (g_Globals.AimBot.SilentAimEnabled != prevSilentAim) {
            if (g_Globals.AimBot.SilentAimEnabled) {
                notificationSystem.Notification("Silent Aim", "Enabled", LightGreen);
            } else {
                notificationSystem.Notification("Silent Aim", "Disabled", SoftRed);
            }
            prevSilentAim = g_Globals.AimBot.SilentAimEnabled;
        }

        // --- Menu only from here ---
        if (!bIsMenuOpen) return;

        io.MouseDrawCursor = true;

        // Render login form if not logged in
        if (!g_Globals.General.isLoggedIn) {
            ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowPos(
                ImVec2((io.DisplaySize.x - 500.f) * 0.5f,
                    (io.DisplaySize.y - 300.f) * 0.5f),
                ImGuiCond_FirstUseEver);

            ImGui::Begin("Phantom Zone - Login", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
            {
                ImGuiStyle* Stylex = &ImGui::GetStyle();
                Stylex->Colors[ImGuiCol_WindowBg] = ImColor(10, 10, 10, 255);
                Stylex->Colors[ImGuiCol_Border] = ImColor(50, 50, 50, 255);
                Stylex->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
                Stylex->Colors[ImGuiCol_FrameBg] = ImColor(30, 30, 30, 255);
                Stylex->Colors[ImGuiCol_FrameBgHovered] = ImColor(50, 50, 50, 255);
                Stylex->Colors[ImGuiCol_FrameBgActive] = ImColor(70, 70, 70, 255);
                Stylex->Colors[ImGuiCol_Button] = ImColor(50, 50, 50, 255);
                Stylex->Colors[ImGuiCol_ButtonHovered] = ImColor(70, 70, 70, 255);
                Stylex->Colors[ImGuiCol_ButtonActive] = ImColor(90, 90, 90, 255);

                ImGui::SetCursorPosY(30);
                ImGui::SetCursorPosX((500 - ImGui::CalcTextSize("Phantom Zone").x) * 0.5f);
                ImGui::Text("Phantom Zone");

                ImGui::SetCursorPosY(80);
                ImGui::SetCursorPosX((500 - 150) / 2);
                ImGui::PushItemWidth(150);
                ImGui::InputText("##licensekey", g_Globals.General.License, 255, ImGuiInputTextFlags_EnterReturnsTrue);
                ImGui::PopItemWidth();

                ImGui::SetCursorPosY(130);
                ImGui::SetCursorPosX((500 - 150) / 2);
                static bool isVerifying = false;
                static std::atomic<bool> atomicIsVerifying(false);
                if ((ImGui::Button("Login", ImVec2(150, 50)) || ImGui::IsKeyPressed(ImGuiKey_Enter)) && !isVerifying && !atomicIsVerifying.load()) {
                    isVerifying = true;
                    atomicIsVerifying.store(true);
                    loginStatus = "[*] Đang xác thực...";

                    // Copy key to use in thread
                    std::string keyCopy = g_Globals.General.License;

                    // Verify in a separate thread to not freeze the UI
                    std::thread([keyCopy]() {
                        bool valid = VerifyKey(keyCopy);

                        if (valid) {
                            g_Globals.General.isLoggedIn = true;
                            SaveLicenseKey();
                            loginStatus = "[+] Xác thực thành công!";
                        } else {
                            loginStatus = "[-] Key sai hoặc hết hạn!";
                        }
                        atomicIsVerifying.store(false);
                    }).detach();
                }

                // Reset isVerifying flag when done
                if (isVerifying && !atomicIsVerifying.load()) {
                    isVerifying = false;
                }

                ImGui::SetCursorPosY(200);
                ImGui::SetCursorPosX(50);
                if (!loginStatus.empty()) {
                    ImGui::Text(loginStatus.c_str());
                }
            }
            ImGui::End();
            return; // Don't render main menu if not logged in
        }

        // Render main menu if logged in
        ImGui::SetNextWindowSize(ImVec2(700, 550), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(
            ImVec2((io.DisplaySize.x - 700.f) * 0.5f,
                (io.DisplaySize.y - 550.f) * 0.5f),
            ImGuiCond_FirstUseEver);


        ImGui::Begin(("Menu"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        {
            ImGuiStyle* Stylex = &ImGui::GetStyle();
            Stylex->Colors[ImGuiCol_WindowBg] = ImColor(10, 10, 10, 255);
            Stylex->Colors[ImGuiCol_Border] = ImColor(50, 50, 50, 255);
            Stylex->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
            Stylex->Colors[ImGuiCol_FrameBg] = ImColor(30, 30, 30, 255);
            Stylex->Colors[ImGuiCol_FrameBgHovered] = ImColor(50, 50, 50, 255);
            Stylex->Colors[ImGuiCol_FrameBgActive] = ImColor(70, 70, 70, 255);
            Stylex->Colors[ImGuiCol_Button] = ImColor(50, 50, 50, 255);
            Stylex->Colors[ImGuiCol_ButtonHovered] = ImColor(70, 70, 70, 255);
            Stylex->Colors[ImGuiCol_ButtonActive] = ImColor(90, 90, 90, 255);

            ImVec2 MenuPos = ImGui::GetWindowPos();
            ImVec2 MenuSize = ImGui::GetWindowSize();
            ImVec2 TabSize = ImVec2(110, MenuSize.y);

            static bool NewTabLower = false;

          
            ImGui::SetNextWindowPos(ImVec2(MenuPos.x - TabSize.x + 10, MenuPos.y));
            ImGui::SetNextWindowSize(TabSize);

            ImGui::Begin("Sidebar", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |ImGuiWindowFlags_NoBackground |ImGuiWindowFlags_NoDecoration);
            {
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                ImVec2      TabPos = ImGui::GetWindowPos();

                draw_list->AddRectFilled(TabPos, TabPos + TabSize, ImColor(10, 10, 10, 255), 8, ImDrawFlags_RoundCornersAll);


                ImVec2 BottomMenuPos = ImGui::GetWindowPos();
                ImVec2 BottomMenuSize = ImGui::GetWindowSize();
                ImVec2 BottomTabSize = ImVec2(800, 40);

                ImGui::SetNextWindowPos(ImVec2(BottomMenuPos.x, BottomMenuPos.y + BottomMenuSize.y + 10));
                ImGui::SetNextWindowSize(BottomTabSize);

                ImGui::Begin(" ", nullptr,ImGuiWindowFlags_NoTitleBar |ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
                {
                    ImDrawList* dl = ImGui::GetWindowDrawList();
                    ImVec2      BTabPos = ImGui::GetWindowPos();

                    dl->AddRectFilled(BTabPos, BTabPos + BottomTabSize,  ImColor(10, 10, 10, 255), 8, ImDrawFlags_RoundCornersAll);


                    ImGui::PushFont(Assets::WaterMarkSmallFont);
                    ImVec2 TextPos = BTabPos + ImVec2(55, 11);
                    ImVec2 VoidTextSize = ImGui::CalcTextSize(("Phantom Zone "));

                    dl->AddText(TextPos, ImColor(255, 255, 255), ("Phantom Zone "));
                    dl->AddText(TextPos + ImVec2(VoidTextSize.x, 0), ImColor(255, 255, 255), ("| Best Cheat!"));
                    ImGui::PopFont();
                }
                ImGui::End();


                ImGui::SetCursorPosY(100);
                ImGui::SetCursorPosX(28);
                ImGui::BeginGroup();
                {
                    if (ImGui::Tab("a", ICON_FA_MOUSE, CurrentTab == 0))
                    {
                        OldCurrentTab = CurrentTab; CurrentTab = 0; NewTabLower = (CurrentTab > OldCurrentTab);
                    }
                    if (ImGui::Tab("b", ICON_FA_EYE, CurrentTab == 1))
                    {
                        OldCurrentTab = CurrentTab; CurrentTab = 1; NewTabLower = (CurrentTab > OldCurrentTab);
                    }
                    if (ImGui::Tab("e", ICON_FA_TH, CurrentTab == 2))
                    {
                        OldCurrentTab = CurrentTab; CurrentTab = 2; NewTabLower = (CurrentTab > OldCurrentTab);
                    }
                    if (ImGui::Tab("d", ICON_FA_FILE, CurrentTab == 3))
                    {
                        OldCurrentTab = CurrentTab; CurrentTab = 3; NewTabLower = (CurrentTab > OldCurrentTab);
                    }
                }
                ImGui::EndGroup();
            }
            ImGui::End();

            static float AimBotAnim = 0.f;
            static float VisualsAnim = 0.f;
            static float MiscAnim = 0.f;
            static float SettingsAnim = 0.f;

            float dt = ImGui::GetIO().DeltaTime * 10.f;
            AimBotAnim = ImLerp(AimBotAnim, CurrentTab == 0 ? 0.f : MenuSize.y, dt);
            VisualsAnim = ImLerp(VisualsAnim, CurrentTab == 1 ? 0.f : MenuSize.y, dt);
            MiscAnim = ImLerp(MiscAnim, CurrentTab == 2 ? 0.f : MenuSize.y, dt);
            SettingsAnim = ImLerp(SettingsAnim, CurrentTab == 3 ? 0.f : MenuSize.y, dt);

            if (CurrentTab == 0) { ImGui::SetCursorPos(ImVec2(15, AimBotAnim));   ImGui::BeginChild("AimBotMainChild", ImVec2(MenuSize.x - 20, 680)); AimbotTab();        ImGui::EndChild(); }
            if (CurrentTab == 1) { ImGui::SetCursorPos(ImVec2(15, VisualsAnim));  ImGui::BeginChild("VisualsMainChild", ImVec2(MenuSize.x - 20, 680)); VisualsTab();       ImGui::EndChild(); }
            if (CurrentTab == 2) { ImGui::SetCursorPos(ImVec2(15, MiscAnim));     ImGui::BeginChild("MiscMainChild", ImVec2(MenuSize.x - 20, 680)); MiscellaneousTab(); ImGui::EndChild(); }
            if (CurrentTab == 3) { ImGui::SetCursorPos(ImVec2(15, SettingsAnim)); ImGui::BeginChild("SettingsMainChild", ImVec2(MenuSize.x - 20, 680)); SettingsTab();      ImGui::EndChild(); }
        }
        ImGui::End(); 
    }

   
    void Interface::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg == WM_SIZE && wParam != SIZE_MINIMIZED)
        {
            ResizeWidht = (UINT)LOWORD(lParam);
            ResizeHeight = (UINT)HIWORD(lParam);
        }

        if (bIsMenuOpen)
            ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
    }

    void Interface::Initialize(HWND Window, HWND Target, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
    {
        hWindow = Window;
        hTargetWindow = Target;
        IDevice = Device;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;

        ImGui_ImplWin32_Init(hWindow);
        ImGui_ImplDX11_Init(Device, DeviceContext);

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImFontConfig cfg;
        cfg.FontBuilderFlags =ImGuiFreeTypeBuilderFlags_NoHinting |  ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;

        static ImWchar icomoon_ranges[] = { 0x1, 0x10FFFD, 0 };

        static ImFontConfig icomoon_config;
        icomoon_config.OversampleH = icomoon_config.OversampleV = 1;
        icomoon_config.MergeMode = true;
        icomoon_config.GlyphOffset.y = 3.f;
        icomoon_config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;

        font::default_r = io.Fonts->AddFontFromMemoryTTF(PoppinsRegular, sizeof(PoppinsRegular), 18.f, &cfg, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 18.f, &icomoon_config, icomoon_ranges);

        font::default_m = io.Fonts->AddFontFromMemoryTTF(PoppinsMedium, sizeof(PoppinsMedium), 18.f, &cfg, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 18.f, &icomoon_config, icomoon_ranges);

        font::default_s = io.Fonts->AddFontFromMemoryTTF(PoppinsSemiBold, sizeof(PoppinsSemiBold), 18.f, &cfg, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 18.f, &icomoon_config, icomoon_ranges);

        font::esp_font = io.Fonts->AddFontFromMemoryTTF(PoppinsMedium, sizeof(PoppinsMedium), 15.f, &cfg, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        icomoon_config.GlyphOffset.y = 0.f;

        font::description_font = io.Fonts->AddFontFromMemoryTTF(PoppinsMedium, sizeof(PoppinsMedium), 16.f, &cfg, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 16.f, &icomoon_config, icomoon_ranges);

        icomoon_config.GlyphOffset.y = 3.f;
        font::regular_m = io.Fonts->AddFontFromMemoryTTF(PoppinsRegular, sizeof(PoppinsRegular), 23.f, &cfg, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 23.f, &icomoon_config, icomoon_ranges);

        icomoon_config.GlyphOffset.y = 0.f;
        font::regular_l = io.Fonts->AddFontFromMemoryTTF(PoppinsRegular, sizeof(PoppinsRegular), 35.f, &cfg, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 35.f, &icomoon_config, icomoon_ranges);

        font::s_inter_semibold = io.Fonts->AddFontFromMemoryTTF(PoppinsSemiBold, sizeof(PoppinsSemiBold), 17.f, &cfg, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        font::inter_bold = io.Fonts->AddFontFromMemoryTTF(PoppinsSemiBold, sizeof(PoppinsSemiBold), 18.f, &cfg, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

        icomoon_config.GlyphOffset.y = 3.f;
        font::inter_semibold = io.Fonts->AddFontFromMemoryTTF(PoppinsSemiBold, sizeof(PoppinsSemiBold), 29.f, &cfg, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 27.f, &icomoon_config, icomoon_ranges);

        static ImWchar font_range[] = { 0x0020, 0x10FFFF, 0 };
        font::small_font = io.Fonts->AddFontFromMemoryTTF(PoppinsSemiBold, sizeof(PoppinsSemiBold), 14.f, &cfg, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        font::inter_medium = io.Fonts->AddFontFromMemoryTTF(PoppinsMedium, sizeof(PoppinsMedium), 17.f, &cfg, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

      

        UpdateStyle();
        ApplyWindowStyle(false);
    }

    void Interface::ShutDown()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

} // ob53 updated by S3XSI - X ( !:)! )
