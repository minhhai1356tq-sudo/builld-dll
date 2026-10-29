#include "Cheat.hpp"
#include <FrameWork/FrameWork.hpp>
#include <Windows.h>
#include "FrameWork/Render/Interface.hpp"
#include <FrameWork/Render/Render.hpp>
#include <FrameWork/Render/Globals.h>
#include <MinHook/include/MinHook.h>
#include <internal/Cheat ff/Memory/Memory.hpp>
#include <internal/Cheat ff/Data/v7a.hpp>
#include <internal/Cheat ff/Data/beta.hpp>
#include <internal/Cheat ff/Esp/Visual.hpp>
#include "DownLocalPlayer.hpp"
#include "DownPlayer.hpp"
#include "EnemyPull360.hpp"
#include "SpinBot.hpp"
#include "TeleportMark.hpp"
#include "FlyHack.hpp"
#include "SilentAim.hpp"

void initcmd() {
    AllocConsole();
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    SetConsoleTitleA("Debug Console");
}

void closecmd() {
    FreeConsole();
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);
}
bool MemoryInit = false;
FrameWork::Interface* g_pInterface = nullptr;
bool bShouldUnload = false;
HMODULE g_hModule;
DWORD WINAPI Unload() {


    if (g_pInterface) {
        g_pInterface->ShutDown();
        delete g_pInterface;
        g_pInterface = nullptr;

    }

    if (MemoryUtils::ogPhysRead) {
        MH_DisableHook((LPVOID)MemoryUtils::ogPhysRead);
        MH_RemoveHook((LPVOID)MemoryUtils::ogPhysRead);

    }

    MH_Uninitialize();
    bShouldUnload = true;


    if (g_hModule) {
        FreeLibraryAndExitThread(g_hModule, 0);
    }
    return 0;
}

void HookMemory() {


    auto vmm = GetModuleHandleA("BstkVMM.dll");
    if (vmm == nullptr) {

        return;
    }


    auto readFunc = (MemoryUtils::PGMPhysReadFunc)GetProcAddress(vmm, "PGMPhysRead");
    if (readFunc == nullptr) {

        return;
    }


    MH_Initialize();

    if (MH_CreateHook((LPVOID)readFunc, MemoryUtils::HookedPGMPhysRead, (LPVOID*)&MemoryUtils::ogPhysRead) != MH_OK) {

        return;
    }


    if (MH_EnableHook((LPVOID)readFunc) != MH_OK) {

        return;
    }



    int waitCounter = 0;
    while (MemoryUtils::vmPtr == nullptr) {
        Sleep(100);
        waitCounter++;
        if (waitCounter % 10 == 0) {
            printf("[MEMORY] Waiting... (%d/30 seconds)\n", waitCounter / 10);
        }
        if (waitCounter > 300) {

            return;
        }
    }


    MemoryUtils::ogCPU = (MemoryUtils::VMMGetCpuByIdFunc)GetProcAddress(vmm, "VMMGetCpuById");
    MemoryUtils::ogCast = (MemoryUtils::PGMPhysGCPtr2GCPhysFunc)GetProcAddress(vmm, "PGMPhysGCPtr2GCPhys");
    MemoryUtils::ogWrite = (MemoryUtils::PGMPhysSimpleWriteGCPhysFunc)GetProcAddress(vmm, "PGMPhysSimpleWriteGCPhys");


    MemoryUtils::Initialize(MemoryUtils::vmPtr);
    MemoryInit = true;

}


namespace Cheat
{
    void HideConsole()
    {
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    }

    void Initialize()
    {
       
        HookMemory();
        if (!MemoryInit) {
            MessageBox(nullptr, L"Memory initialization failed", L"Error", MB_OK | MB_ICONERROR);
            return;
        }
        DownLocalPlayer::DownLocalPlayerr::DownLocal();
        DownPlayer::DownPlayerr::DownPlayer();
        EnemyPull::EnemyPull360::Start();
        FrameWork::Overlay::Setup(Render::FindRenderWindow());
        FrameWork::Overlay::Initialize();
        if (FrameWork::Overlay::IsInitialized())
        {
            FrameWork::Interface Interface(FrameWork::Overlay::GetOverlayWindow(), FrameWork::Overlay::GetTargetWindow(), FrameWork::Overlay::dxGetDevice(), FrameWork::Overlay::dxGetDeviceContext());
            Interface.UpdateStyle();
            FrameWork::Overlay::SetupWindowProcHook(std::bind(&FrameWork::Interface::WindowProc, &Interface, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

            MSG Message;
            ZeroMemory(&Message, sizeof(Message));

            int frameCount = 0;
            int lastEntityCount = 0;
            bool lastESPState = false;
            while (Message.message != WM_QUIT) {
                if (PeekMessage(&Message, FrameWork::Overlay::GetOverlayWindow(), NULL, NULL, PM_REMOVE))
                {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }
                ImGui::GetIO().MouseDrawCursor = Interface.GetMenuOpen() && g_Globals.General.ShowMenu;

                if (Interface.ResizeHeight != 0 || Interface.ResizeWidht != 0)
                {
                    FrameWork::Overlay::dxCleanupRenderTarget();
                    FrameWork::Overlay::dxGetSwapChain()->ResizeBuffers(0, Interface.ResizeWidht, Interface.ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
                    Interface.ResizeHeight = Interface.ResizeWidht = 0;
                    FrameWork::Overlay::dxCreateRenderTarget();
                }

                Interface.HandleMenuKey();

                FrameWork::Overlay::UpdateWindowPos();

                ImGui_ImplDX11_NewFrame();
                ImGui_ImplWin32_NewFrame();
                ImGui::NewFrame();
                {
                    RECT targetRect;
                    if (GetWindowRect(FrameWork::Overlay::GetTargetWindow(), &targetRect)) {
                        g_Globals.EspConfig.Width = targetRect.right - targetRect.left;
                        g_Globals.EspConfig.Height = targetRect.bottom - targetRect.top;
                    }

                    static int lastVersion = 0;
                    if (g_Globals.General.CurrentGameVersion != lastVersion) {
                        if (g_Globals.General.CurrentGameVersion == 1) {

                        }
                        else if (g_Globals.General.CurrentGameVersion == 2) {

                        }
                        else if (g_Globals.General.CurrentGameVersion == 0 && lastVersion != 0) {

                        }
                        lastVersion = g_Globals.General.CurrentGameVersion;
                    }

                    if (g_Globals.General.CurrentGameVersion == 1) {
                        FWork::Data::Work();
                    }
                    else if (g_Globals.General.CurrentGameVersion == 2) {
                        FWork::Dataffmax::Work();
                    }

                    frameCount++;
                    if (frameCount >= 100) {
                        int currentCount = g_Globals.EspConfig.Entities.size();
                        if (currentCount != lastEntityCount) {
                            if (currentCount > 0) {
                                printf("[ESP] Entities found: %d\n", currentCount);
                            }
                            lastEntityCount = currentCount;
                        }
                        frameCount = 0;
                    }

                    if (g_Globals.Visuals.Enable != lastESPState) {
                        if (g_Globals.Visuals.Enable) {

                        }
                        else {

                        }
                        lastESPState = g_Globals.Visuals.Enable;
                    }

                    // Draw Silent Aim FOV Circle
                    if (g_Globals.AimBot.ShowFovCircle) {
                        ImVec2 screenCenter(static_cast<float>(g_Globals.EspConfig.Width) / 2.0f, static_cast<float>(g_Globals.EspConfig.Height) / 2.0f);
                        
                        float radius = static_cast<float>(g_Globals.AimBot.SilentAimFov);
                        float thickness = g_Globals.AimBot.fovThickness;
                        
                        ImDrawList* drawList = ImGui::GetForegroundDrawList();
                        float step = 1.0f / 64.0f;
                        float time = ImGui::GetTime();
                        float speedMultiplier = 2.0f;
                        
                        for (int i = 0; i < 64; ++i) {
                            float t1 = i * step;
                            float t2 = (i + 1) * step;
                            
                            ImU32 color1, color2;
                            if (g_Globals.AimBot.RgbMode) {
                                float hue1 = fmod(t1 + time * speedMultiplier, 1.0f);
                                float hue2 = fmod(t2 + time * speedMultiplier, 1.0f);
                                color1 = ImColor::HSV(hue1, 1.0f, 1.0f);
                                color2 = ImColor::HSV(hue2, 1.0f, 1.0f);
                            } else {
                                color1 = ImColor(255, 255, 255);
                                color2 = ImColor(255, 255, 255);
                            }
                            
                            float angle1 = t1 * 2.0f * IM_PI;
                            float angle2 = t2 * 2.0f * IM_PI;
                            
                            ImVec2 p1 = ImVec2(screenCenter.x + cos(angle1) * radius, screenCenter.y + sin(angle1) * radius);
                            ImVec2 p2 = ImVec2(screenCenter.x + cos(angle2) * radius, screenCenter.y + sin(angle2) * radius);
                            
                            drawList->AddLine(p1, p2, color1, thickness);
                        }
                    }

                    // Silent Aim Logic
                    if (g_Globals.AimBot.SilentAimEnabled) {
                        SilentAimLoop::StartLoop();
                        
                        SilentTarget best = SilentAim::FindBestSilentTarget(
                            g_Globals.EspConfig.LocalPlayer,
                            g_Globals.EspConfig.MainCamera,
                            g_Globals.EspConfig.ViewMatrix,
                            g_Globals.EspConfig.Width,
                            g_Globals.EspConfig.Height
                        );
                        
                        if (best.entity != 0) {
                            SilentAimLoop::UpdateTarget(g_Globals.EspConfig.LocalPlayer, best.headPosition, true);
                        } else {
                            SilentAimLoop::UpdateTarget(g_Globals.EspConfig.LocalPlayer, Vector3::Zero(), false);
                        }
                    } else {
                        SilentAimLoop::StopLoop();
                    }

                    if (g_Globals.Visuals.Enable) {
                        ESP::Players();
                    }

                    Interface.RenderGui();

                    if (g_Globals.General.pendiclear) {
                        Mem.Cache.clear();
                        g_Globals.General.pendiclear = false;

                    }

                    if (GetAsyncKeyState(VK_DELETE) & 1) {

                        g_Globals.General.ShutDown = true;
                    }
                }

                ImGui::EndFrame();
                ImGui::Render();
                FrameWork::Overlay::dxRefresh();
                ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
                FrameWork::Overlay::dxGetSwapChain()->Present(0, 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(g_Globals.General.Delay));
            }

        }
      
    }

    void ShutDown() {
        DownLocalPlayer::DownLocalPlayerr::StopDownLocal();
        DownPlayer::DownPlayerr::StopDownPlayer();
        EnemyPull::EnemyPull360::Stop();
        SpinBot::SpinBotExploit::Stop();
        InternalTeleportMark::Stop();
        InternalFlyMini::Stop();
        SilentAimLoop::StopLoop();
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }
}
DWORD WINAPI MainThread(LPVOID lpParameter) {
#ifdef _DEBUG
    initcmd();
#endif

    Cheat::Initialize();

    while (!bShouldUnload && !g_Globals.General.ShutDown) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    Cheat::ShutDown();

#ifdef _DEBUG
    closecmd();
#endif

    return 0;
}

//HMODULE g_hModule;
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, MainThread, hModule, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
        bShouldUnload = true;
        g_Globals.General.ShutDown = true;
        break;
    }
    return TRUE;
}
