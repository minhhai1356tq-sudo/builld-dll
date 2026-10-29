#include "TeleportMark.hpp"
#include <thread>
#include <atomic>
#include <FrameWork/Render/Globals.h>
#include <internal/Cheat ff/Memory/Memory.hpp>
#include <internal/Cheat ff/Offsets.h>
#include <FrameWork/Math/Vectors/Vector3D.hpp>

namespace InternalTeleportMark {
    static std::atomic<bool> isRunning{ false };
    static std::thread worker;
    static uint32_t staticGameFacade = 0;

    void Start() {
        if (isRunning) return;
        isRunning = true;
        worker = std::thread([] {
            while (isRunning) {
                if (!g_Globals.Exploits.TeleportMark) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }

                uint32_t localPlayer = 0;
                uint32_t baseGameFacade;
                if (!Mem.ReadFast2(OffsetsffmaxBeta::Il2Cpp + OffsetsffmaxBeta::InitBase, &baseGameFacade)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }

                uint32_t gameFacade, currentGame;
                Mem.ReadFast2(baseGameFacade, &gameFacade);
                Mem.ReadFast2(gameFacade + OffsetsffmaxBeta::StaticClass, &staticGameFacade);

                Vector3 markPos = Vector3(0, 0, 0);
                uint32_t UIInGameScene, m_BigMapCtrl, m_MapContentCtrl, m_LocalMapMarkController;

                if (staticGameFacade == 0) {
                    if (!Mem.ReadFast2(OffsetsffmaxBeta::Il2Cpp + OffsetsffmaxBeta::InitBase, &baseGameFacade)) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        continue;
                    }
                    if (!Mem.ReadFast2(baseGameFacade, &gameFacade)) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        continue;
                    }
                    if (!Mem.ReadFast2(gameFacade + OffsetsffmaxBeta::StaticClass, &staticGameFacade)) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        continue;
                    }
                }

                if (!Mem.ReadFast2(staticGameFacade + 0x0, &currentGame)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }
                if (!Mem.ReadFast2(currentGame + 0x8, &UIInGameScene)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }
                if (!Mem.ReadFast2(UIInGameScene + 0x1F4, &m_BigMapCtrl)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }
                if (!Mem.ReadFast2(m_BigMapCtrl + 0X54, &m_MapContentCtrl)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }
                if (!Mem.ReadFast2(m_MapContentCtrl + 0x90, &m_LocalMapMarkController)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }
                if (!Mem.ReadFast2(m_LocalMapMarkController + 0x58, &markPos)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }

                if (markPos != Vector3(0, 0, 0)) {
                    uint32_t currentGameMatch;
                    Mem.ReadFast2(staticGameFacade, &currentGame);
                    Mem.ReadFast2(currentGame + OffsetsffmaxBeta::CurrentMatch, &currentGameMatch);
                    Mem.ReadFast2(currentGameMatch + OffsetsffmaxBeta::LocalPlayer, &localPlayer);

                    if (localPlayer) {
                        uint32_t localRootBonePtr;
                        if (Mem.ReadFast2(localPlayer + OffsetsffmaxBeta::BonesffmaxBeta::Root, &localRootBonePtr)) {
                            uint32_t localTransformValue;
                            if (Mem.ReadFast2(localRootBonePtr + 0x8, &localTransformValue)) {
                                uint32_t localTransformObjPtr;
                                if (Mem.ReadFast2(localTransformValue + 0x8, &localTransformObjPtr)) {
                                    uint32_t localMatrixValue;
                                    if (Mem.ReadFast2(localTransformObjPtr + 0x20, &localMatrixValue)) {
                                        Mem.Write(localMatrixValue + 0x60, markPos);
                                    }
                                }
                            }
                        }
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
        worker.detach();
    }

    void Stop() {
        isRunning = false;
        staticGameFacade = 0;
    }
}
