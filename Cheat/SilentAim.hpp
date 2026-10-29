#pragma once
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>
#include <memory>
#include <FrameWork/Math/Vectors/Vector3D.hpp>
#include <FrameWork/Render/Globals.h>
#include <internal/Cheat ff/Memory/Memory.hpp>
#include <internal/Cheat ff/Math/WorldToScreen.hpp>
#include <internal/Cheat ff/Player.h>
#include <Windows.h>
#include <FrameWork/Dependencies/ImGui/imgui.h>

// Offsets for Silent Aim
namespace SilentAimOffsets {
    constexpr uint32_t sAim1 = 0x534;
    constexpr uint32_t sAim2 = 0x930;
    constexpr uint32_t sAim3 = 0x38;
    constexpr uint32_t sAim4 = 0x2C;
}

// Silent Target struct
struct SilentTarget {
    uint32_t entity = 0;
    Vector3 headPosition;
    float screenDistance = FLT_MAX;
    float distance3D = FLT_MAX;
};

// Silent Aim Loop
namespace SilentAimLoop {
    extern std::atomic<bool> running;
    extern std::atomic<bool> hasTarget;
    extern Vector3 targetHeadPos;
    extern uint32_t cachedLocalPlayer;
    extern std::thread workerThread;

    void StartLoop();
    void StopLoop();
    void UpdateTarget(uint32_t localPlayer, const Vector3& headPos, bool active);
}

// Main Silent Aim Functions
namespace SilentAim {
    SilentTarget FindBestSilentTarget(uint32_t localPlayer, Vector3 cameraPos,
                                       Matrix4x4 viewMatrix, int screenWidth, int screenHeight);

    void DoSilentAim(uint32_t localPlayer, const Vector3& targetHead);
}

// Helper Functions
float GetDistance(ImVec2 a, ImVec2 b);
