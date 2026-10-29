#include "FlyHack.hpp"
#include <thread>
#include <atomic>
#include <cmath>
#include <FrameWork/Render/Globals.h>
#include <internal/Cheat ff/Memory/Memory.hpp>
#include <internal/Cheat ff/Offsets.h>
#include <FrameWork/Math/Vectors/Vector3D.hpp>

namespace InternalFlyMini {
    static std::atomic<bool> isRunning{ false };
    static std::thread worker;
    static float targetHeight = 2.5f; // Lock cứng độ cao 2.5 mét
    static bool hasStarted = false;
    static uint32_t matrixPtr = 0;

    void Start() {
        if (isRunning) return;
        isRunning = true;
        worker = std::thread([] {
            while (isRunning) {
                if (!g_Globals.Exploits.FlyHack) {
                    hasStarted = false;
                    matrixPtr = 0;
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }

                uint32_t localPlayer = 0;
                uint32_t baseGameFacade;
                if (!Mem.ReadFast2(OffsetsffmaxBeta::Il2Cpp + OffsetsffmaxBeta::InitBase, &baseGameFacade)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }

                uint32_t gameFacade, staticGameFacade, currentGame, currentMatch;
                Mem.ReadFast2(baseGameFacade, &gameFacade);
                Mem.ReadFast2(gameFacade + OffsetsffmaxBeta::StaticClass, &staticGameFacade);
                Mem.ReadFast2(staticGameFacade, &currentGame);
                Mem.ReadFast2(currentGame + OffsetsffmaxBeta::CurrentMatch, &currentMatch);
                Mem.ReadFast2(currentMatch + OffsetsffmaxBeta::LocalPlayer, &localPlayer);

                if (localPlayer) {
                    uint32_t dkRootPtr = 0, dkT1 = 0, dkT2 = 0, dkMat = 0;
                    bool dkChainOk =
                        Mem.ReadFast2(localPlayer + OffsetsffmaxBeta::BonesffmaxBeta::Root, &dkRootPtr) && dkRootPtr != 0 &&
                        Mem.ReadFast2(dkRootPtr + 0x8, &dkT1) && dkT1 != 0 &&
                        Mem.ReadFast2(dkT1 + 0x8, &dkT2) && dkT2 != 0 &&
                        Mem.ReadFast2(dkT2 + 0x20, &dkMat) && dkMat != 0;

                    if (dkChainOk) {
                        matrixPtr = dkMat;

                        Vector3 currentPos{};
                        if (Mem.ReadFast2(matrixPtr + 0x60, &currentPos)) {
                            if (!hasStarted) {
                                targetHeight = currentPos.Y + 1.6f; // Đầu tiên nhấc lên 1.6m từ vị trí hiện tại
                                hasStarted = true;
                            }

                            currentPos.Y = targetHeight; // Lock cứng độ cao 2.5m mỗi frame, không giật
                            Mem.Write(matrixPtr + 0x60, currentPos);
                        }
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Giảm delay xuống 50ms để lock cực mượt
            }
        });
        worker.detach();
    }

    void Stop() {
        isRunning = false;
        hasStarted = false;
        matrixPtr = 0;
    }
}