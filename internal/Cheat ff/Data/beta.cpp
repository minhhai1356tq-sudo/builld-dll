#include <map>
#include "Beta.hpp"
#include <cstdint>
#include <FrameWork/Render/Globals.h>
#include <ImGui/imgui.h>

#include <internal/Cheat ff/Math/Entity.h> 
#include <FrameWork/Render/Globals.h>
#include <internal/Cheat ff/Memory/Memory.hpp>
#include <ImGui/imgui.h>
#include <internal/Cheat ff/Math/Tmatrix_beta.h>
#include <internal/Cheat ff/Offsets.h>
#include <internal/Cheat ff/Math/WorldToScreen.hpp>
#include <internal/Cheat ff/Math/Quaternion.hpp>
#include <FrameWork/Math/Vectors/Vector3D.hpp>
#include <random>
#include <thread>
#include <cmath>

static Vector3 CalcAngle(const Vector3& src, const Vector3& dst) {
    Vector3 dir = dst - src;
    float hyp = sqrtf(dir.X * dir.X + dir.Z * dir.Z);
    Vector3 angles{};
    angles.X = -atan2f(dir.Y, hyp) * (180.0f / 3.14159265f);
    angles.Y = atan2f(dir.Z, dir.X) * (180.0f / 3.14159265f);
    angles.Z = 0.0f;
    return angles;
}

namespace FWork {
    static uint32_t savedLocalPlayer = 0;
    static int errorCounter = 0;

    inline void SyncContextWithEspConfig() {
        Context::HasMatrix = g_Globals.EspConfig.Matrix;
        Context::ViewMatrix = g_Globals.EspConfig.ViewMatrix;
        Context::LocalMainCamera = g_Globals.EspConfig.MainCamera;
        Context::LocalPlayerAddress = g_Globals.EspConfig.LocalPlayer;
        Context::LocalMainCameraAddress = g_Globals.EspConfig.LocalMainCameraAddress;
        Context::WindowWidth = g_Globals.EspConfig.Width;
        Context::WindowHeight = g_Globals.EspConfig.Height;

        std::unique_lock<std::shared_mutex> lock(Context::EntityMutex);
        Context::Entities.clear();

        for (const auto& [address, player] : g_Globals.EspConfig.Entities) {
            Entity entity{};
            entity.Address = player.Address;
            entity.IsDead = player.IsDead;
            entity.IsKnocked = player.IsKnocked;
            entity.Health = player.Health;
            entity.IsVisible = player.IsVisible;
            entity.IsBot = player.IsBot;
            entity.IsTeam = (player.IsTeam == Player::Bool3::True) ? Bool3::True : Bool3::False;
            entity.Head = player.Head;
            entity.Root = player.Root;
            entity.Hip = player.Hip;
            entity.Neck = player.Neck;
            entity.IsKnown = player.IsKnown;
            Context::Entities[address] = entity;
        }
    }

    void Dataffmax::Work() {
        g_Globals.EspConfig.Matrix = false;

        try {
            if (OffsetsffmaxBeta::Il2Cpp == 0) return;

            uint32_t baseGameFacade = 0;
            if (!Mem.ReadFast2(OffsetsffmaxBeta::Il2Cpp + OffsetsffmaxBeta::InitBase, &baseGameFacade) || !baseGameFacade) { Reset(); return; }

            uint32_t gameFacade = 0;
            if (!Mem.ReadFast2(baseGameFacade, &gameFacade) || !gameFacade) { Reset(); return; }

            uint32_t staticGameFacade = 0;
            if (!Mem.ReadFast2(gameFacade + OffsetsffmaxBeta::StaticClass, &staticGameFacade) || !staticGameFacade) { Reset(); return; }

            uint32_t currentGame = 0;
            if (!Mem.ReadFast2(staticGameFacade, &currentGame) || !currentGame) { Reset(); return; }

            uint32_t currentMatch = 0;
            if (!Mem.ReadFast2(currentGame + OffsetsffmaxBeta::CurrentMatch, &currentMatch) || !currentMatch) { Reset(); return; }

            uint32_t matchStatus = 0;
            Mem.ReadFast2(currentMatch + OffsetsffmaxBeta::MatchStatus, &matchStatus);

            if (!matchStatus)
                g_Globals.time.oneflagtime = false;

            uint32_t localPlayer = 0;
            bool actuallyAlive = Mem.ReadFast2(currentMatch + OffsetsffmaxBeta::LocalPlayer, &localPlayer) && localPlayer != 0;

            if (actuallyAlive) {
                savedLocalPlayer = localPlayer;
                errorCounter = 0;
            }
            else if (savedLocalPlayer != 0) {
                localPlayer = savedLocalPlayer;
            }
            else {
                localPlayer = 0; Reset(); return;
            }
            g_Globals.EspConfig.LocalPlayer = localPlayer;
            g_Globals.EspConfig.IsRealPlayerAlive = actuallyAlive;

            uint32_t mainTransform = 0;
            if (!Mem.ReadFast2(localPlayer + OffsetsffmaxBeta::MainCameraTransform, &mainTransform) || !mainTransform) { Reset(); return; }

            Vector3 mainPos;
            TransformUtils::GetPosition(mainTransform, mainPos);
            g_Globals.EspConfig.MainCamera = mainPos;

            uint32_t followCamera = 0;
            if (!Mem.ReadFast2(localPlayer + OffsetsffmaxBeta::FollowCamera, &followCamera) || !followCamera) { Reset(); return; }

            uint32_t camera = 0;
            if (!Mem.ReadFast2(followCamera + OffsetsffmaxBeta::Camera, &camera) || !camera) { Reset(); return; }
            g_Globals.EspConfig.LocalMainCameraAddress = camera;

            uint32_t cameraBase = 0;
            if (!Mem.ReadFast2(camera + 0x8, &cameraBase) || !cameraBase) { Reset(); return; }

            Matrix4x4 viewMatrix;
            if (Mem.ReadFast2(cameraBase + OffsetsffmaxBeta::ViewMatrix, &viewMatrix)) {
                g_Globals.EspConfig.Matrix = true;
                g_Globals.EspConfig.ViewMatrix = viewMatrix;
            }
            else { Reset(); return; }

            uint32_t entityDictionary = 0;
            if (!Mem.ReadFast2(currentGame + OffsetsffmaxBeta::DictionaryEntities, &entityDictionary) || !entityDictionary) { Reset(); return; }

            int entitiesCount = 0;
            if (!Mem.ReadFast2(entityDictionary + 0x10, &entitiesCount) || entitiesCount < 1 || entitiesCount > 10000) {
                if (!g_Globals.EspConfig.Entities.empty())
                    g_Globals.EspConfig.Entities.clear();
                return;
            }


            char msg[128];
            sprintf_s(msg, "Beta entities found: %d", entitiesCount);


            uint32_t entries = 0;
            if (!Mem.ReadFast2(entityDictionary + 0xC, &entries) || !entries) { Reset(); return; }

            uint32_t start = entries + 0x10;

            if (entitiesCount != g_Globals.EspConfig.previousCount)
                g_Globals.EspConfig.previousCount = entitiesCount;

            if (entitiesCount > 0) {
                if (!g_Globals.time.oneflagtime) {
                    g_Globals.time.timerStarted = true;
                    g_Globals.time.timerStartTime = (float)ImGui::GetTime();
                    g_Globals.time.oneflagtime = true;
                }
            }

            int processedCount = 0;
            std::vector<uint32_t> currentEntities;

            for (int i = 0; i < entitiesCount; ++i) {
                uint32_t entry = start + (i * 0x10);

                int hash = 0;
                if (!Mem.ReadFast2(entry + 0x0, &hash) || hash < 0) continue;

                uint32_t entity = 0;
                if (!Mem.ReadFast2(entry + 0xC, &entity) || entity == 0) continue;
                if (entity == localPlayer) continue;

                currentEntities.push_back(entity);

                Player& playerEntry = g_Globals.EspConfig.Entities[entity];
                if (EntityData(entity, playerEntry, mainPos)) {
                    processedCount++;
                }
                else {
                    g_Globals.EspConfig.Entities.erase(entity);
                }
            }


            for (auto it = g_Globals.EspConfig.Entities.begin(); it != g_Globals.EspConfig.Entities.end(); ) {
                if (std::find(currentEntities.begin(), currentEntities.end(), it->first) == currentEntities.end()) {
                    it = g_Globals.EspConfig.Entities.erase(it);
                }
                else {
                    ++it;
                }
            }

            SyncContextWithEspConfig();

            if (g_Globals.AimBot.NoRecoil) {
                uint32_t weaponPtr = 0;
                if (Mem.Read<uint32_t>(localPlayer + OffsetsffmaxBeta::Weapon, weaponPtr) && weaponPtr != 0) {
                    uint32_t weaponData = 0;
                    if (Mem.Read<uint32_t>(weaponPtr + OffsetsffmaxBeta::WeaponData, weaponData) && weaponData != 0) {
                        float recoil = 0.0f;
                        if (Mem.Read<float>(weaponData + OffsetsffmaxBeta::WeaponRecoil, recoil) && recoil != 0.0f) {
                            Mem.Write<float>(weaponData + OffsetsffmaxBeta::WeaponRecoil, 0.0f);
                        }
                    }
                }
            }

            if (g_Globals.AimBot.FastSwitch) {
                uint32_t currentWeaponPtr = 0;
                if (Mem.Read<uint32_t>(localPlayer + OffsetsffmaxBeta::Weapon, currentWeaponPtr) && currentWeaponPtr != 0) {
                    float time = 0x10;
                    Mem.Write<float>(currentWeaponPtr + 0x4C, 0.0f);
                    Mem.Write<float>(currentWeaponPtr + 0x54, 500.0f);
                    Mem.Write<float>(currentWeaponPtr + 0x48, time - 0.01f);
                    Mem.Write<float>(currentWeaponPtr + 0x50, time - 0.01f);
                    Mem.Write<bool>(currentWeaponPtr + 0x3AC, false);
                }
            }
            
            // Legit Aimbot Trackers
            static std::map<uintptr_t, uint32_t> VaMemory; // addr -> original collider
            static std::map<uintptr_t, std::chrono::steady_clock::time_point> GhiBoNho; // addr -> thời gian ghi
            static std::chrono::steady_clock::time_point SauKhiKhoiPhuc = std::chrono::steady_clock::now();
            static std::chrono::steady_clock::time_point KhiNhan = std::chrono::steady_clock::time_point::min();
            static bool isPressing = false;
            
            // LEGIT AIMBOT
            if (g_Globals.AimBot.AimbotLegit) {
                // Restore sau 3000ms
                auto now = std::chrono::steady_clock::now();
                if (std::chrono::duration_cast<std::chrono::milliseconds>(now - SauKhiKhoiPhuc).count() >= 3000) {
                    // Khôi phục tất cả memory
                    for (const auto& [addr, original] : VaMemory) {
                        Mem.Write(addr, original);
                    }
                    VaMemory.clear();
                    GhiBoNho.clear();
                    SauKhiKhoiPhuc = now;
                }
                
                ImVec2 screenCenter(g_Globals.EspConfig.Width / 2.0f, g_Globals.EspConfig.Height / 2.0f);
                bool isKeyPressed = (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
                
                if (isKeyPressed) {
                    if (!isPressing) {
                        KhiNhan = now;
                        isPressing = true;
                    }
                    
                    // Delay giữ phím trước khi aim
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - KhiNhan).count() < g_Globals.AimBot.AimbotAIDelay) {
                        // Đang chờ delay giữ phím
                    } else {
                        float aimAIStrength = 0.50f;
                        
                        // Find closest target
                        float closestDistance = FLT_MAX;
                        Player* bestTarget = nullptr;
                        
                        for (auto& [addr, player] : g_Globals.EspConfig.Entities) {
                            if (player.IsTeam == Player::Bool3::True) continue;
                            if (player.IsDead) continue;
                            if (g_Globals.AimBot.IgnoreKnocked && player.IsKnocked) continue;
                            if (player.Distance > g_Globals.AimBot.DistanceAim) continue;
                            
                            ImVec2 targetPos = W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, player.Head, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height);
                            bool isOnScreen = targetPos.x > 0.0f && targetPos.y > 0.0f && targetPos.x < g_Globals.EspConfig.Width && targetPos.y < g_Globals.EspConfig.Height;
                            
                            if (isOnScreen) {
                                float dist = std::sqrt(std::pow(targetPos.x - screenCenter.x, 2) + std::pow(targetPos.y - screenCenter.y, 2));
                                if (dist < closestDistance) {
                                    closestDistance = dist;
                                    bestTarget = &player;
                                }
                            }
                        }
                        
                        if (bestTarget) {
                            ImVec2 targetPos = W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, bestTarget->Head, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height);
                            bool isOnScreen = targetPos.x > 0.0f && targetPos.y > 0.0f && targetPos.x < g_Globals.EspConfig.Width && targetPos.y < g_Globals.EspConfig.Height;
                            
                            if (isOnScreen) {
                                uint32_t rHeadCollider = 0;
                                if (Mem.ReadFast2(bestTarget->Address + OffsetsffmaxBeta::HeadCollider, &rHeadCollider) && rHeadCollider) {
                                    float randomValue = static_cast<float>(rand()) / RAND_MAX;
                                    if (randomValue <= aimAIStrength) {
                                        uintptr_t patchAddr = bestTarget->Address + OffsetsffmaxBeta::searchHex;
                                        
                                        // Lưu giá trị ban đầu nếu chưa có
                                        if (VaMemory.find(patchAddr) == VaMemory.end()) {
                                            uint32_t originalCollider = 0;
                                            if (Mem.ReadFast2(patchAddr, &originalCollider)) {
                                                VaMemory[patchAddr] = originalCollider;
                                            }
                                        }
                                        
                                        // Ghi memory sau 50ms từ lần ghi trước
                                        if (GhiBoNho.find(patchAddr) == GhiBoNho.end() ||
                                            std::chrono::duration_cast<std::chrono::milliseconds>(now - GhiBoNho[patchAddr]).count() >= 50) {
                                            Mem.Write(patchAddr, rHeadCollider);
                                            GhiBoNho[patchAddr] = now;
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else {
                    isPressing = false;
                    KhiNhan = std::chrono::steady_clock::time_point::min();
                    // Khôi phục memory khi thả phím
                    for (const auto& [addr, original] : VaMemory) {
                        Mem.Write(addr, original);
                    }
                    VaMemory.clear();
                    GhiBoNho.clear();
                }
            }

            // RAGE AIMBOT
            if (g_Globals.AimBot.AimbotRage) {
                bool keyHeld = (GetAsyncKeyState(g_Globals.AimBot.AimbotRageBind) & 0x8000) != 0;
                if (!keyHeld) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                } else {
                    float closestDistance = FLT_MAX;
                    Player* bestTarget = nullptr;
                    Vector3 bestTargetPos;

                    ImVec2 screenCenter(g_Globals.EspConfig.Width / 2.0f, g_Globals.EspConfig.Height / 2.0f);

                    for (auto& [addr, player] : g_Globals.EspConfig.Entities) {
                        if (!player.IsKnown) continue;
                        if (player.IsDead) continue;
                        if (g_Globals.AimBot.IgnoreKnocked && player.IsKnocked) continue;
                        if (player.IsTeam == Player::Bool3::True) continue;

                        Vector3 hitBoxPos;
                        switch (g_Globals.AimBot.HitBox) {
                            case 1: hitBoxPos = player.Neck; break;
                            case 2: hitBoxPos = player.Hip; break;
                            default: hitBoxPos = player.Head; break;
                        }

                        ImVec2 hitBox2D = W2S::WorldToScreenImVec2(g_Globals.EspConfig.ViewMatrix, hitBoxPos, g_Globals.EspConfig.Width, g_Globals.EspConfig.Height);
                        if (hitBox2D.x < 1 || hitBox2D.y < 1) continue;

                        float worldDistance = Vector3::Magnitude(player.Head - g_Globals.EspConfig.MainCamera);
                        if (worldDistance > g_Globals.AimBot.DistanceAim) continue;

                        float crosshairDist = sqrtf((hitBox2D.x - screenCenter.x) * (hitBox2D.x - screenCenter.x) + (hitBox2D.y - screenCenter.y) * (hitBox2D.y - screenCenter.y));
                        if (crosshairDist > g_Globals.AimBot.RageFov) continue;
                        if (crosshairDist < closestDistance) {
                            closestDistance = crosshairDist;
                            bestTarget = &player;
                            bestTargetPos = hitBoxPos;
                        }
                    }

                    if (bestTarget) {
                        Vector3 aimRotation = CalcAngle(g_Globals.EspConfig.MainCamera, bestTargetPos);
                        Mem.Write(localPlayer + OffsetsffmaxBeta::AimRotation, aimRotation);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(g_Globals.AimBot.RageSmooth));
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            
            // Fast Reload Logic
            if (g_Globals.AimBot.FastReload) {
                uint32_t reload = 0;
                if (Mem.ReadFast2(localPlayer + OffsetsffmaxBeta::PlayerAttributes, &reload) && reload != 0) {
                    Mem.Write(reload + OffsetsffmaxBeta::NoReload, true);
                }
            }
            else {
                uint32_t reload = 0;
                if (Mem.ReadFast2(localPlayer + OffsetsffmaxBeta::PlayerAttributes, &reload) && reload != 0) {
                    Mem.Write(reload + OffsetsffmaxBeta::NoReload, false);
                }
            }
            
            //AimLogic::Tick(true);
            //SpinBot::Tick(true);

        }
        catch (...) {

            Reset();
        }
    }

    bool Dataffmax::EntityData(uint32_t entity, Player& player, Vector3& mainPos) {
        try {
            if (entity == 0) return false;

            uint32_t avatarManager = 0;
            Mem.ReadFast2(entity + OffsetsffmaxBeta::AvatarManager, &avatarManager);
            if (!avatarManager) return false;

            uint32_t avatar = 0;
            Mem.ReadFast2(avatarManager + OffsetsffmaxBeta::Avatar, &avatar);
            if (!avatar) return false;

            uint32_t avatarData = 0;
            Mem.ReadFast2(avatar + OffsetsffmaxBeta::Avatar_Data, &avatarData);
            if (!avatarData) return false;

            Mem.ReadFast2(avatar + OffsetsffmaxBeta::Avatar_IsVisible, &player.IsVisible);

            bool isTeam = false;
            Mem.ReadFast2(avatarData + OffsetsffmaxBeta::Avatar_Data_IsTeam, &isTeam);
            player.IsTeam = isTeam ? Player::Bool3::True : Player::Bool3::False;
            if (player.IsTeam == Player::Bool3::True) return false;

            player.IsKnown = true;

            Mem.ReadFast2(entity + OffsetsffmaxBeta::Player_IsDead, &player.IsDead);
            if (player.IsDead) return false;

            uint32_t dataPool = 0;
            if (Mem.ReadFast2(entity + OffsetsffmaxBeta::Player_Data, &dataPool) && dataPool != 0) {
                uint32_t poolObj = 0;
                if (Mem.ReadFast2(dataPool + 0x8, &poolObj) && poolObj != 0) {
                    uint32_t pool = 0;
                    if (Mem.ReadFast2(poolObj + 0x10, &pool) && pool != 0) {
                        Mem.ReadFast2(pool + 0xC, &player.Health);
                        if (player.Health > 1000) player.Health = 200;

                        uint32_t weaponptr = 0;
                        if (Mem.ReadFast2(poolObj + 0x20, &weaponptr) && weaponptr != 0)
                            Mem.ReadFast2(weaponptr + 0xC, &player.WeaponID);
                    }
                }
            }

            if (player.Health <= 0 || player.Health > 1000) return false;
            player.Address = entity;

            uint32_t shadowBase = 0;
            Mem.ReadFast2(entity + OffsetsffmaxBeta::Player_ShadowBase, &shadowBase);
            if (shadowBase != 0) {
                int xpose = 0;
                Mem.ReadFast2(shadowBase + OffsetsffmaxBeta::XPose, &xpose);
                player.Pose = static_cast<XPose>(xpose);
                player.IsKnocked = (xpose == 8);
            }

            Mem.ReadFast2(entity + OffsetsffmaxBeta::IsClientBot, &player.IsBot);

            uint32_t headAddr = 0;
            if (Mem.ReadFast2(entity + OffsetsffmaxBeta::BonesffmaxBeta::Head, &headAddr) && headAddr)
                TransformUtils::GetNodePosition(headAddr, player.Head);
            else return false;

            if (player.Head == Vector3::Zero()) return false;


            static const std::map<uintptr_t, Vector3 Player::*> boneMap = {

                { OffsetsffmaxBeta::BonesffmaxBeta::Spine, &Player::Spine },
                { OffsetsffmaxBeta::BonesffmaxBeta::Hip, &Player::Hip },
                { OffsetsffmaxBeta::BonesffmaxBeta::Root, &Player::Root },
                { OffsetsffmaxBeta::BonesffmaxBeta::LeftShoulder, &Player::LeftShoulder },
                { OffsetsffmaxBeta::BonesffmaxBeta::RightShoulder, &Player::RightShoulder },
                { OffsetsffmaxBeta::BonesffmaxBeta::LeftElbow, &Player::LeftElbow },
                { OffsetsffmaxBeta::BonesffmaxBeta::RightElbow, &Player::RightElbow },
                { OffsetsffmaxBeta::BonesffmaxBeta::LeftWrist, &Player::LeftWrist },
                { OffsetsffmaxBeta::BonesffmaxBeta::RightWrist, &Player::RightWrist },
                { OffsetsffmaxBeta::BonesffmaxBeta::LeftFoot, &Player::LeftFoot },
                { OffsetsffmaxBeta::BonesffmaxBeta::RightFoot, &Player::RightFoot },
                { OffsetsffmaxBeta::BonesffmaxBeta::LeftCalf, &Player::LeftCalf },
                { OffsetsffmaxBeta::BonesffmaxBeta::RightCalf, &Player::RightCalf }
            };

            for (const auto& [offset, member] : boneMap) {
                uint32_t bonePtr = 0;
                if (Mem.ReadFast2(entity + offset, &bonePtr) && bonePtr)
                    TransformUtils::GetNodePosition(bonePtr, player.*member);
            }

            // Tính velocity
            if (player.LastRootPosition != Vector3::Zero()) {
                player.Velocity = player.Root - player.LastRootPosition;
            } else {
                player.Velocity = Vector3::Zero();
            }
            player.LastRootPosition = player.Root;

            if (player.Head != Vector3::Zero())
                player.Distance = Vector3::Distance(mainPos, player.Head);

            if (player.Distance > 1000 || player.Distance < 0) return false;

            if (player.Name.empty()) {
                uint32_t nameAddr = 0;
                if (Mem.Read(entity + OffsetsffmaxBeta::Player_Name, nameAddr) && nameAddr != 0) {
                    int nameLen = 0;
                    if (Mem.Read(nameAddr + 0x8, nameLen) && nameLen > 0 && nameLen < 64) {
                        std::string name = Mem.ReadStringZ2(nameAddr + 0xC, nameLen);
                        if (!name.empty() && name.length() < 32)
                            player.Name = name;
                    }
                }
            }

            return true;
        }
        catch (...) { return false; }
    }
    void Dataffmax::Reset() {
        try {
            if (!g_Globals.EspConfig.Entities.empty())
                g_Globals.EspConfig.Entities.clear();

            g_Globals.EspConfig.previousCount = 0;
            g_Globals.EspConfig.LocalPlayer = 0;
            g_Globals.EspConfig.IsRealPlayerAlive = false;
            Mem.Cache.clear();

            g_Globals.EspConfig.Matrix = false;
            g_Globals.EspConfig.ViewMatrix = Matrix4x4();
            g_Globals.EspConfig.MainCamera = Vector3(0, 0, 0);
            g_Globals.General.pullStop = true;

            if (OffsetsffmaxBeta::Il2Cpp == 0) {
                g_Globals.General.CurrentGameVersion = 0;
                g_Globals.General.hookstatus = false;
            }

        }
        catch (...) {}
    }
}
