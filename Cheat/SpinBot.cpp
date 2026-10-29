#include "SpinBot.hpp"
#include <internal/Cheat ff/Memory/Memory.hpp>
#include <internal/Cheat ff/Math/Quaternion.hpp>
#include <FrameWork/Math/Vectors/Vector3D.hpp>
#include <internal/Cheat ff/Offsets.h>
#include <FrameWork/Render/Globals.h>
#include <cmath>
#include <thread>
#include <chrono>

namespace SpinBot {

std::atomic<bool> SpinBotExploit::running{ false };
std::thread       SpinBotExploit::thread;

static constexpr int   LOOP_DELAY_MS = 5;
static constexpr float DEG_TO_RAD    = 3.14159265358979323846f / 180.0f;

void SpinBotExploit::Start() {
    if (running) return;
    running = true;

    thread = std::thread([] {
        float currentYaw = 0.0f;

        while (running && !g_Globals.General.ShutDown) {
            try {
                if (!g_Globals.Exploits.SpinBot) {
                    currentYaw = 0.0f;
                    std::this_thread::sleep_for(std::chrono::milliseconds(LOOP_DELAY_MS));
                    continue;
                }

                uint32_t localPlayer = g_Globals.EspConfig.LocalPlayer;
                if (!localPlayer) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(12));
                    continue;
                }

                // Dùng offset Root bone từ dự án hiện tại
                uint32_t root = 0;
                if (!Mem.ReadFast2(localPlayer + OffsetsffmaxBeta::BonesffmaxBeta::Root, &root) || !root)
                    goto next;

                // Cadeia pointer như code cũ, nhưng dùng Mem.ReadFast2
                uint32_t t1 = 0;
                if (!Mem.ReadFast2(root + 0x8, &t1) || !t1)
                    goto next;

                uint32_t t2 = 0;
                if (!Mem.ReadFast2(t1 + 0x8, &t2) || !t2)
                    goto next;

                uint32_t visualState = 0;
                if (!Mem.ReadFast2(t2 + 0x20, &visualState) || !visualState)
                    goto next;

                {
                    // Đọc vị trí hiện tại
                    Vector3 pos{};
                    if (!Mem.ReadFast2(visualState + 0x60, &pos))
                        goto next;

                    // Tăng yaw
                    currentYaw += (float)g_Globals.Exploits.SpinSpeed;
                    if (currentYaw >= 360.0f)
                        currentYaw -= 360.0f;

                    // Tạo quaternion quay quanh trục Y
                    Quaternion rot = Quaternion::FromAngleAxis(
                        currentYaw * DEG_TO_RAD,
                        Vector3(0.0f, 1.0f, 0.0f));

                    // Ghi vị trí và quay
                    Mem.Write(visualState + 0x60, pos);
                    Mem.Write(visualState + 0x70, rot);
                }
            }
            catch (...) {
                // Bỏ qua lỗi
            }

        next:
            std::this_thread::sleep_for(std::chrono::milliseconds(LOOP_DELAY_MS));
        }

        running = false;
    });
    thread.detach();
}

void SpinBotExploit::Stop() {
    running = false;
    if (thread.joinable())
        thread.join();
}

} // namespace SpinBot