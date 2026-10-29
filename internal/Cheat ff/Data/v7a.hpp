#pragma once
#include <internal/Cheat ff/Player.h>


namespace FWork {
    class Data {
    public:
        static void Work();
    private:
        static bool EntityData(uint32_t entity, Player& player, Vector3& mainPos);
        static void Reset();;
    };
}
