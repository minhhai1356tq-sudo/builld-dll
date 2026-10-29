#include "EnemyPull360.hpp"
#include <FrameWork/Render/Globals.h>
#include <internal/Cheat ff/Memory/Memory.hpp>
#include <internal/Cheat ff/Offsets.h>
#include <internal/Cheat ff/Math/WorldToScreen.hpp>
#include <FrameWork/Math/Vectors/Vector3D.hpp>
#include <unordered_map>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cfloat>

namespace EnemyPull {
    std::atomic<bool> enemyPullRunning{ false };
    std::thread enemyPullThread;

    static std::unordered_map<uint32_t, Vector3> originalPositions;
    static uint32_t currentTargetId = 0;
    static bool wasFiringLastFrame = false;
    static bool isTargetPositionLocked = false;
    static Vector3 lockedTargetPosition;

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

    bool EnemyPull360::IsLocalFiring() {
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

    static uint32_t FindClosestEntity() {
        if (g_Globals.EspConfig.Width == -1 || g_Globals.EspConfig.Height == -1 ||
            !g_Globals.EspConfig.Matrix || g_Globals.EspConfig.Entities.empty()) {
            return 0;
        }

        float centerX = g_Globals.EspConfig.Width / 2.0f;
        float centerY = g_Globals.EspConfig.Height / 2.0f;
        float fov = 450.0f;

        uint32_t closestEntity = 0;
        float closestDistanceSqr = FLT_MAX;

        for (const auto& pair : g_Globals.EspConfig.Entities) {
            const Player& entity = pair.second;

            if (!IsEnemyCandidate(entity)) continue;

            Vector2 head2D = W2S::WorldToScreen(
                g_Globals.EspConfig.ViewMatrix,
                entity.Head,
                g_Globals.EspConfig.Width,
                g_Globals.EspConfig.Height
            );

            if (head2D.X < 1 || head2D.Y < 1 || head2D.X > g_Globals.EspConfig.Width || head2D.Y > g_Globals.EspConfig.Height) continue;

            float dx = head2D.X - centerX;
            float dy = head2D.Y - centerY;
            float distanceSqr = dx * dx + dy * dy;

            if (distanceSqr <= fov * fov && distanceSqr < closestDistanceSqr) {
                closestEntity = entity.Address;
                closestDistanceSqr = distanceSqr;
            }
        }

        return closestEntity;
    }

    static void LockTargetPosition(uint32_t entityAddr) {
        currentTargetId = entityAddr;

        uint32_t rootBone = 0;
        uint32_t positionAddress = 0;
        Vector3 posA_Enemy;

        if (g_Globals.General.CurrentGameVersion == 1) {
            if (!Mem.ReadFast2(entityAddr + OffsetsffNormal::BonesffNormal::Root, &rootBone) || rootBone == 0) return;
        } else if (g_Globals.General.CurrentGameVersion == 2) {
            if (!Mem.ReadFast2(entityAddr + OffsetsffmaxBeta::BonesffmaxBeta::Root, &rootBone) || rootBone == 0) return;
        } else {
            return;
        }

        if (!ResolveTransformPositionAddress(rootBone, positionAddress)) return;
        if (!Mem.ReadFast2(positionAddress, &posA_Enemy)) return;

        if (originalPositions.find(entityAddr) == originalPositions.end()) {
            originalPositions[entityAddr] = posA_Enemy;
        }

        Vector3 posB_PlayerCamera = g_Globals.EspConfig.MainCamera;
        Vector3 vecC_ViewDirection = { g_Globals.EspConfig.ViewMatrix.m00, g_Globals.EspConfig.ViewMatrix.m10, g_Globals.EspConfig.ViewMatrix.m20 };
        vecC_ViewDirection = Vector3::Normalized(vecC_ViewDirection);
        float realDistance3D = Vector3::Distance(posB_PlayerCamera, posA_Enemy);

        lockedTargetPosition = posB_PlayerCamera + (vecC_ViewDirection * realDistance3D);
        lockedTargetPosition.Y += g_Globals.Exploits.PullHeightOffset;
        isTargetPositionLocked = true;
    }

    static void ForceEntityToLockedPosition() {
        if (currentTargetId == 0 || g_Globals.EspConfig.Entities.find(currentTargetId) == g_Globals.EspConfig.Entities.end()) {
            EnemyPull360::RestoreOriginalPosition();
            return;
        }

        uint32_t rootBone = 0;
        uint32_t positionAddress = 0;

        if (g_Globals.General.CurrentGameVersion == 1) {
            if (!Mem.ReadFast2(currentTargetId + OffsetsffNormal::BonesffNormal::Root, &rootBone) || rootBone == 0) return;
        } else if (g_Globals.General.CurrentGameVersion == 2) {
            if (!Mem.ReadFast2(currentTargetId + OffsetsffmaxBeta::BonesffmaxBeta::Root, &rootBone) || rootBone == 0) return;
        } else {
            return;
        }

        if (!ResolveTransformPositionAddress(rootBone, positionAddress)) return;
        Mem.Write(positionAddress, lockedTargetPosition);
    }

    void EnemyPull360::RestoreOriginalPosition() {
        if (isTargetPositionLocked && currentTargetId != 0) {
            auto it = originalPositions.find(currentTargetId);
            if (it != originalPositions.end()) {
                uint32_t rootBone = 0;
                uint32_t positionAddress = 0;

                bool success = false;
                if (g_Globals.General.CurrentGameVersion == 1) {
                    success = Mem.ReadFast2(currentTargetId + OffsetsffNormal::BonesffNormal::Root, &rootBone) && rootBone != 0;
                } else if (g_Globals.General.CurrentGameVersion == 2) {
                    success = Mem.ReadFast2(currentTargetId + OffsetsffmaxBeta::BonesffmaxBeta::Root, &rootBone) && rootBone != 0;
                }

                if (success && ResolveTransformPositionAddress(rootBone, positionAddress)) {
                    Mem.Write(positionAddress, it->second);
                }

                originalPositions.erase(currentTargetId);
            }
        }

        currentTargetId = 0;
        isTargetPositionLocked = false;
    }

    void EnemyPull360::Start() {
        if (enemyPullRunning) return;
        enemyPullRunning = true;

        enemyPullThread = std::thread([]() {
            while (enemyPullRunning) {
                if (!g_Globals.Exploits.EnemyPullEnabled) {
                    RestoreOriginalPosition();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    continue;
                }

                bool isCurrentlyFiring = IsLocalFiring();

                if (isCurrentlyFiring) {
                    if (!isTargetPositionLocked) {
                        uint32_t closestEntity = FindClosestEntity();
                        if (closestEntity != 0) {
                            LockTargetPosition(closestEntity);
                        }
                    }

                    if (isTargetPositionLocked) {
                        ForceEntityToLockedPosition();
                    }

                    wasFiringLastFrame = true;
                } else if (wasFiringLastFrame) {
                    RestoreOriginalPosition();
                    wasFiringLastFrame = false;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            RestoreOriginalPosition();
        });

        enemyPullThread.detach();
    }

    void EnemyPull360::Stop() {
        enemyPullRunning = false;
        if (enemyPullThread.joinable()) {
            enemyPullThread.join();
        }
        RestoreOriginalPosition();
    }
}
