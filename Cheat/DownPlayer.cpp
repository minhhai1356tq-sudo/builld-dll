#include <FrameWork/Dependencies/ImGui/imgui.h>
#include <FrameWork/Dependencies/ImGui/imgui_internal.h>
#include <internal/Cheat ff/Memory/Memory.hpp>
#include <FrameWork/Math/Vectors/Vector3D.hpp>
#include <FrameWork/Math/Vectors/Vector2D.hpp>
#include <internal/Cheat ff/Math/WorldToScreen.hpp>
#include <FrameWork/Render/Globals.h>
#include <internal/Cheat ff/Offsets.h>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <thread>
#include <atomic>
#include <chrono>
#include <cfloat>
#include "DownPlayer.hpp"

#define NOMINMAX

std::atomic<bool> downPlayerRunning{ false };
std::thread downPlayerThread;
constexpr float DownOffset = 0.45f;
static std::unordered_map<uint32_t, Vector3> originalPositions;
static uint32_t currentTarget = 0;
static uint32_t lastTarget = 0;

static bool ResolveTransformPositionAddress(uint32_t nodeTransform, uint32_t& positionAddress) {
    positionAddress = 0;
    if (!nodeTransform || nodeTransform == (uint32_t)-1) return false;

    uint32_t transformValue = 0;
    uint32_t transformObjValue = 0;
    uint32_t indexValue = 0;
    uint32_t matrixValue = 0;
    uint32_t matrixListValue = 0;
    uint32_t matrixIndicesValue = 0;

    if (!Mem.Read(nodeTransform + 0x8, transformValue) || !transformValue || transformValue == (uint32_t)-1) return false;
    if (!Mem.Read(transformValue + 0x8, transformObjValue) || !transformObjValue || transformObjValue == (uint32_t)-1) return false;
    if (!Mem.Read(transformObjValue + 0x24, indexValue) || indexValue == (uint32_t)-1) return false;
    if (!Mem.Read(transformObjValue + 0x20, matrixValue) || !matrixValue || matrixValue == (uint32_t)-1) return false;

    if (g_Globals.General.CurrentGameVersion == 1) {
        if (!Mem.Read(matrixValue + 0x18, matrixListValue) || !matrixListValue || matrixListValue == (uint32_t)-1) return false;
        if (!Mem.Read(matrixValue + 0x1C, matrixIndicesValue) || !matrixIndicesValue || matrixIndicesValue == (uint32_t)-1) return false;
    }
    else if (g_Globals.General.CurrentGameVersion == 2) {
        if (!Mem.Read(matrixValue + 0x14, matrixListValue) || !matrixListValue || matrixListValue == (uint32_t)-1) return false;
        if (!Mem.Read(matrixValue + 0x18, matrixIndicesValue) || !matrixIndicesValue || matrixIndicesValue == (uint32_t)-1) return false;
    }
    else {
        return false;
    }

    positionAddress = indexValue * 0x30 + matrixListValue;
    return positionAddress != 0 && positionAddress != (uint32_t)-1;
}

static bool TryReadWeaponChainFiringV1(uint32_t localPlayer) {
    uint32_t pomba = 0;
    uint32_t bisteca = 0;
    uint32_t arma = 0;
    bool firing = false;

    return Mem.ReadFast2<uint32_t>(localPlayer + OffsetsffNormal::pomba, &pomba) && pomba &&
        Mem.ReadFast2<uint32_t>(pomba + OffsetsffNormal::bisteca, &bisteca) && bisteca &&
        Mem.ReadFast2<uint32_t>(bisteca + OffsetsffNormal::arma, &arma) && arma &&
        Mem.ReadFast2<bool>(arma + OffsetsffNormal::tiro, &firing) && firing;
}

static bool TryReadWeaponChainFiringV2(uint32_t localPlayer) {
    uint32_t pomba = 0;
    uint32_t bisteca = 0;
    uint32_t arma = 0;
    bool firing = false;

    return Mem.ReadFast2<uint32_t>(localPlayer + OffsetsffmaxBeta::pomba, &pomba) && pomba &&
        Mem.ReadFast2<uint32_t>(pomba + OffsetsffmaxBeta::bisteca, &bisteca) && bisteca &&
        Mem.ReadFast2<uint32_t>(bisteca + OffsetsffmaxBeta::arma, &arma) && arma &&
        Mem.ReadFast2<bool>(arma + OffsetsffmaxBeta::tiro, &firing) && firing;
}

static bool IsEnemyCandidate(const Player& entity) {
    return entity.IsKnown &&
        !entity.IsDead &&
        !entity.IsKnocked &&
        entity.IsTeam != Player::Bool3::True &&
        entity.Address != 0;
}

bool DownPlayer::DownPlayerr::IsLocalFiring() {
    bool firing = false;
    uint32_t localPlayer = g_Globals.EspConfig.LocalPlayer;
    if (!localPlayer) return false;

    if (g_Globals.General.CurrentGameVersion == 1) {
        if (Mem.ReadFast2<bool>(localPlayer + OffsetsffNormal::isfiring, &firing) && firing) return true;
        if (TryReadWeaponChainFiringV1(localPlayer)) return true;
        if (Mem.ReadFast2<bool>(localPlayer + OffsetsffNormal::tiro, &firing) && firing) return true;
    }
    else if (g_Globals.General.CurrentGameVersion == 2) {
        if (Mem.ReadFast2<bool>(localPlayer + OffsetsffmaxBeta::IS_FIRING, &firing) && firing) return true;
        if (TryReadWeaponChainFiringV2(localPlayer)) return true;
        if (Mem.ReadFast2<bool>(localPlayer + OffsetsffmaxBeta::tiro, &firing) && firing) return true;
    }

    return false;
}

uint32_t FindClosestTarget() {
    uint32_t bestTarget = 0;
    float closestDist = FLT_MAX;
    Vector2 screenCenter = {
        (float)g_Globals.EspConfig.Width / 2.0f,
        (float)g_Globals.EspConfig.Height / 2.0f
    };

    for (const auto& pair : g_Globals.EspConfig.Entities) {
        const Player& entity = pair.second;
        if (!IsEnemyCandidate(entity)) continue;

        Vector2 screenPos = W2S::WorldToScreen(
            g_Globals.EspConfig.ViewMatrix,
            entity.Head,
            g_Globals.EspConfig.Width,
            g_Globals.EspConfig.Height
        );

        if (screenPos.X < 1 || screenPos.Y < 1 || screenPos.X > g_Globals.EspConfig.Width || screenPos.Y > g_Globals.EspConfig.Height) continue;

        float crosshairDist = Vector2::Distance(screenCenter, screenPos);
        if (crosshairDist > g_Globals.AimBot.RageFov) continue;

        if (crosshairDist < closestDist) {
            closestDist = crosshairDist;
            bestTarget = entity.Address;
        }
    }
    return bestTarget;
}

void ApplyDown(uint32_t entityAddr) {
    uint32_t bone1 = 0;
    uint32_t positionAddress = 0;
    Vector3 currentPos;

    if (g_Globals.General.CurrentGameVersion == 1) {
        if (!Mem.ReadFast2<uint32_t>(entityAddr + OffsetsffNormal::BonesffNormal::Root, &bone1) || !bone1) return;
    } else if (g_Globals.General.CurrentGameVersion == 2) {
        if (!Mem.ReadFast2<uint32_t>(entityAddr + OffsetsffmaxBeta::BonesffmaxBeta::Root, &bone1) || !bone1) return;
    } else {
        return;
    }

    if (!ResolveTransformPositionAddress(bone1, positionAddress)) return;
    if (!Mem.ReadFast2<Vector3>(positionAddress, &currentPos)) return;

    if (originalPositions.find(entityAddr) == originalPositions.end()) {
        originalPositions[entityAddr] = currentPos;
    }

    float targetY = originalPositions[entityAddr].Y - DownOffset;
    if (currentPos.Y != targetY) {
        Vector3 newPos = currentPos;
        newPos.Y = targetY;
        try {
            Mem.Write<Vector3>(positionAddress, newPos);
        } catch (const std::exception& e) {
            std::cerr << "Write failed: " << e.what() << std::endl;
        }
    }
}

void DownPlayer::DownPlayerr::RestoreAllPositions() {
    for (auto it = originalPositions.begin(); it != originalPositions.end(); ) {
        uint32_t entityAddr = it->first;
        uint32_t bone1 = 0;
        uint32_t positionAddress = 0;
        Vector3 currentPos;

        if (g_Globals.General.CurrentGameVersion == 1) {
            if (!Mem.ReadFast2<uint32_t>(entityAddr + OffsetsffNormal::BonesffNormal::Root, &bone1) || !bone1) {
                it = originalPositions.erase(it);
                continue;
            }
        } else if (g_Globals.General.CurrentGameVersion == 2) {
            if (!Mem.ReadFast2<uint32_t>(entityAddr + OffsetsffmaxBeta::BonesffmaxBeta::Root, &bone1) || !bone1) {
                it = originalPositions.erase(it);
                continue;
            }
        } else {
            it = originalPositions.erase(it);
            continue;
        }

        if (!ResolveTransformPositionAddress(bone1, positionAddress)) {
            it = originalPositions.erase(it);
            continue;
        }
        if (!Mem.ReadFast2<Vector3>(positionAddress, &currentPos)) {
            it = originalPositions.erase(it);
            continue;
        }

        if (currentPos.Y != it->second.Y) {
            Vector3 restoredPos = currentPos;
            restoredPos.Y = it->second.Y;
            try {
                Mem.Write<Vector3>(positionAddress, restoredPos);
            } catch (const std::exception& e) {
                std::cerr << "Restore failed: " << e.what() << std::endl;
            }
        }
        ++it;
    }
    originalPositions.clear();
}

void DownPlayer::DownPlayerr::DownPlayer() {
    if (downPlayerRunning) return;
    downPlayerRunning = true;

    downPlayerThread = std::thread([] {
        while (downPlayerRunning) {
            if (!g_Globals.Exploits.DownPlayer) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            bool keyPressed = (GetAsyncKeyState(g_Globals.Exploits.DownPlayerBind) & 0x8000) != 0;
            if (!keyPressed) {
                RestoreAllPositions();
                currentTarget = 0;
                lastTarget = 0;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            if (g_Globals.EspConfig.Width == -1 || g_Globals.EspConfig.Height == -1 || !g_Globals.EspConfig.Matrix) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            uint32_t localPlayer = g_Globals.EspConfig.LocalPlayer;
            if (!localPlayer) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            bool firing = IsLocalFiring();
            if (firing) {
                currentTarget = FindClosestTarget();
                if (currentTarget) {
                    if (currentTarget != lastTarget) {
                        lastTarget = currentTarget;
                    }
                    ApplyDown(currentTarget);
                }
                else {
                    RestoreAllPositions();
                }
            }
            else {
                RestoreAllPositions();
                currentTarget = 0;
                lastTarget = 0;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(6));
        }

        RestoreAllPositions();
    });

    downPlayerThread.detach();
}

void DownPlayer::DownPlayerr::StopDownPlayer() {
    downPlayerRunning = false;
    if (downPlayerThread.joinable()) {
        downPlayerThread.join();
    }
    RestoreAllPositions();
}
