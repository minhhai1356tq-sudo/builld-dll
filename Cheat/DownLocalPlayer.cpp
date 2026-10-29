#include "DownLocalPlayer.hpp"
#include <chrono>
#include <cmath>
#include <windows.h>
#include <imgui.h>
#include <imgui_internal.h>

namespace DownLocalPlayer {
    std::atomic<bool> DownLocalPlayerr::isRunning{ false };
    std::atomic<bool> DownLocalPlayerr::shouldStop{ false };
    std::atomic<bool> DownLocalPlayerr::isFrozen{ false };
    std::thread DownLocalPlayerr::tpThread;
    Vector3 DownLocalPlayerr::frozenPos = Vector3::Zero();
    float DownLocalPlayerr::teleportDownDistance = 1.0f;

    void DownLocalPlayerr::DownLocal() {
        if (isRunning) return;

        isRunning = true;
        shouldStop = false;
        isFrozen = false;

        tpThread = std::thread([]() {
            bool positionCalculated = false;
            bool lastKeyState = false;
            bool isToggled = false;

            while (!shouldStop) {
                bool currentKeyState = g_Globals.Exploits.DownLocalPlayerBind != 0 &&
                    (GetAsyncKeyState(g_Globals.Exploits.DownLocalPlayerBind) & 0x8000) != 0;

                if (currentKeyState && !lastKeyState) {
                    isToggled = !isToggled;
                    g_Globals.Exploits.DownLocalPlayer = isToggled;
                    positionCalculated = false;
                    isFrozen = false;
                }
                lastKeyState = currentKeyState;

                if (g_Globals.Exploits.DownLocalPlayer != isToggled) {
                    isToggled = g_Globals.Exploits.DownLocalPlayer;
                    if (!isToggled) {
                        positionCalculated = false;
                        isFrozen = false;
                    }
                }

                if (!isToggled) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }

                if (!g_Globals.EspConfig.Matrix || g_Globals.EspConfig.Width == -1 || g_Globals.EspConfig.Height == -1) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }

                uintptr_t localPlayer = g_Globals.EspConfig.LocalPlayer;
                if (!localPlayer || localPlayer < 0x10000) {
                    positionCalculated = false;
                    isFrozen = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }

                uintptr_t rootBone = 0, transform1 = 0, transform2 = 0, matrix = 0;
                Vector3 currentPos;
                uintptr_t rootOffset = 0;

                if (g_Globals.General.CurrentGameVersion == 2) {
                    rootOffset = OffsetsffmaxBeta::BonesffmaxBeta::Root;
                }
                else {
                    rootOffset = OffsetsffNormal::BonesffNormal::Root;
                }

                if (!Mem.Read<uintptr_t>(localPlayer + rootOffset, rootBone) ||
                    rootBone == 0 || rootBone < 0x10000) {
                    positionCalculated = false;
                    isFrozen = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }
                if (!Mem.Read<uintptr_t>(rootBone + 0x8, transform1) ||
                    transform1 == 0 || transform1 < 0x10000) {
                    positionCalculated = false;
                    isFrozen = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }
                if (!Mem.Read<uintptr_t>(transform1 + 0x8, transform2) ||
                    transform2 == 0 || transform2 < 0x10000) {
                    positionCalculated = false;
                    isFrozen = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }
                if (!Mem.Read<uintptr_t>(transform2 + 0x20, matrix) ||
                    matrix == 0 || matrix < 0x10000) {
                    positionCalculated = false;
                    isFrozen = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }
                if (!Mem.Read<Vector3>(matrix + 0x60, currentPos)) {
                    positionCalculated = false;
                    isFrozen = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }

                if (!isfinite(currentPos.X) || !isfinite(currentPos.Y) || !isfinite(currentPos.Z)) {
                    positionCalculated = false;
                    isFrozen = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }

                if (!positionCalculated) {
                    frozenPos = currentPos;
                    frozenPos.Y -= teleportDownDistance;
                    positionCalculated = true;
                    isFrozen = true;
                }

                if (isFrozen) {
                    float distance = sqrtf(
                        powf(currentPos.X - frozenPos.X, 2) +
                        powf(currentPos.Y - frozenPos.Y, 2) +
                        powf(currentPos.Z - frozenPos.Z, 2)
                    );

                    if (distance > 2.0f) {
                        positionCalculated = false;
                        isFrozen = false;
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        continue;
                    }
                }

                if (matrix != 0 && matrix >= 0x10000 &&
                    isfinite(frozenPos.X) && isfinite(frozenPos.Y) && isfinite(frozenPos.Z)) {
                    try {
                        Mem.Write<Vector3>(matrix + 0x60, frozenPos);
                    }
                    catch (...) {
                        positionCalculated = false;
                        isFrozen = false;
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            isRunning = false;
            isFrozen = false;
        });

        tpThread.detach();
    }

    void DownLocalPlayerr::StopDownLocal() {
        if (!isRunning) return;
        shouldStop = true;
        isFrozen = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        isRunning = false;
    }

    bool DownLocalPlayerr::IsActive() {
        return isRunning;
    }
}
