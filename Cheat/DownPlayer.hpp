#pragma once
#include <cstdint>
#include <FrameWork/Math/Vectors/Vector3D.hpp>
#include <internal/Cheat ff/Player.h>
#include <FrameWork/Render/Globals.h>
#include <internal/Cheat ff/Memory/Memory.hpp>

namespace DownPlayer {
    class DownPlayerr {
    public:
        static void DownPlayer();
        static void StopDownPlayer();
        static void RestoreAllPositions();
        static bool IsLocalFiring();
    };
}
