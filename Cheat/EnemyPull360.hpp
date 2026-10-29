#pragma once
#include <cstdint>
#include <atomic>
#include <thread>

namespace EnemyPull {
    class EnemyPull360 {
    public:
        static void Start();
        static void Stop();
        static bool IsLocalFiring();
        static void RestoreOriginalPosition();
    };
}
