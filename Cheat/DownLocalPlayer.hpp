#pragma once
#include <cstdint>
#include <atomic>
#include <thread>
#include <FrameWork/Math/Vectors/Vector3D.hpp>
#include <internal/Cheat ff/Player.h>
#include <FrameWork/Render/Globals.h>
#include <internal/Cheat ff/Memory/Memory.hpp>
#include <internal/Cheat ff/Offsets.h>

namespace DownLocalPlayer {
    class DownLocalPlayerr {
    public:
        static void DownLocal();
        static void StopDownLocal();
        static bool IsActive();

    private:
        static std::atomic<bool> isRunning;
        static std::atomic<bool> shouldStop;
        static std::atomic<bool> isFrozen;
        static std::thread tpThread;
        static Vector3 frozenPos;
        static float teleportDownDistance;
    };
}
