#include <map>
#include "v7a.hpp"
#include <cstdint>
#include <internal/Cheat ff/Math/Entity.h> // enttity.h
#include <FrameWork/Render/Globals.h>
#include <internal/Cheat ff/Memory/Memory.hpp>
#include <ImGui/imgui.h>
#include <internal/Cheat ff/Math/Tmatrix_v7a.h>
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





static Player* lastSilentTarget = nullptr;
static uint32_t savedLocalPlayer = 0;

namespace FWork {

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

    void Data::Work() {
        g_Globals.EspConfig.Matrix = false;

        try {
            if (OffsetsffNormal::Il2Cpp == 0) {
                static bool debugOnce = true;
                if (debugOnce) {

                    debugOnce = false;
                }
                return;
            }

            char msg[256];
            sprintf_s(msg, "Il2Cpp base: 0x%llX", OffsetsffNormal::Il2Cpp);


            uint32_t baseGameFacade = 0;
            if (!Mem.ReadFast2(OffsetsffNormal::Il2Cpp + OffsetsffNormal::InitBase, &baseGameFacade) || !baseGameFacade) {

                Sleep(500);
                Reset(); return;
            }
            sprintf_s(msg, "baseGameFacade: 0x%X", baseGameFacade);


            uint32_t gameFacade = 0;
            if (!Mem.ReadFast2(baseGameFacade, &gameFacade) || !gameFacade) {

                Sleep(500);
                Reset(); return;
            }
            sprintf_s(msg, "gameFacade: 0x%X", gameFacade);


            uint32_t staticGameFacade = 0;
            if (!Mem.ReadFast2(gameFacade + OffsetsffNormal::StaticClass, &staticGameFacade) || !staticGameFacade) {

                Sleep(500);
                Reset(); return;
            }
            sprintf_s(msg, "staticGameFacade: 0x%X", staticGameFacade);


            g_Globals.EspConfig.cachedStaticGameFacade = staticGameFacade;

            uint32_t currentGame = 0;
            if (!Mem.ReadFast2(staticGameFacade, &currentGame) || !currentGame) {

                Reset(); return;
            }
            sprintf_s(msg, "currentGame: 0x%X", currentGame);


            uint32_t currentMatch = 0;
            if (!Mem.ReadFast2(currentGame + OffsetsffNormal::CurrentMatch, &currentMatch) || !currentMatch) {

                Reset(); return;
            }
            sprintf_s(msg, "currentMatch: 0x%X", currentMatch);


            uint32_t matchStatus = 0;
            Mem.ReadFast2(currentMatch + OffsetsffNormal::MatchStatus, &matchStatus);
            sprintf_s(msg, "matchStatus: %d", matchStatus);


            if (!matchStatus) {
                g_Globals.time.oneflagtime = false;
            }

            if (matchStatus != 1) {

                Reset(); return;
            }

            uint32_t localPlayer = 0;
            bool actuallyAlive = Mem.ReadFast2(currentMatch + OffsetsffNormal::LocalPlayer, &localPlayer) && localPlayer != 0;

            if (actuallyAlive) {
                savedLocalPlayer = localPlayer;
                sprintf_s(msg, "localPlayer found: 0x%X", localPlayer);

            }
            else if (savedLocalPlayer != 0) {
                localPlayer = savedLocalPlayer;

            }
            else {
                localPlayer = 0;

                Reset(); return;
            }

            g_Globals.EspConfig.LocalPlayer = localPlayer;
            g_Globals.EspConfig.IsRealPlayerAlive = actuallyAlive;

            uint32_t mainTransform = 0;
            if (!Mem.ReadFast2(localPlayer + OffsetsffNormal::MainCameraTransform, &mainTransform) || !mainTransform) {

                Reset(); return;
            }

            Vector3 mainPos;
            TransformUtils2::GetPosition(mainTransform, mainPos);
            g_Globals.EspConfig.MainCamera = mainPos;

            uint32_t followCamera = 0;
            if (!Mem.ReadFast2(localPlayer + OffsetsffNormal::FollowCamera, &followCamera) || !followCamera) {

                Reset(); return;
            }

            uint32_t camera = 0;
            if (!Mem.ReadFast2(followCamera + OffsetsffNormal::Camera, &camera) || !camera) {

                Reset(); return;
            }
            g_Globals.EspConfig.LocalMainCameraAddress = camera;

            uint32_t cameraBase = 0;
            if (!Mem.ReadFast2(camera + 0x8, &cameraBase) || !cameraBase) {

                Reset(); return;
            }

            Matrix4x4 viewMatrix;
            if (Mem.ReadFast2(cameraBase + OffsetsffNormal::ViewMatrix, &viewMatrix)) {
                g_Globals.EspConfig.Matrix = true;
                g_Globals.EspConfig.ViewMatrix = viewMatrix;

            }
            else {

                Reset(); return;
            }

            uint32_t entityDictionary = 0;
            if (!Mem.ReadFast2(currentGame + OffsetsffNormal::DictionaryEntities, &entityDictionary) || !entityDictionary) {

                Reset(); return;
            }

            uint32_t entities = 0;
            if (!Mem.ReadFast2(entityDictionary + 0xc, &entities) || !entities) {

                Reset(); return;
            }

            entities += 0x1c;

            uint32_t entitiesCount = 0;
            Mem.ReadFast2(entityDictionary + 0x10, &entitiesCount);

            if (entitiesCount != g_Globals.EspConfig.previousCount) {
                g_Globals.EspConfig.previousCount = entitiesCount;
            }

            if (entitiesCount < 1) {
                if (!g_Globals.EspConfig.Entities.empty()) g_Globals.EspConfig.Entities.clear();
                return;
            }

            int validEntities = 0;
            std::vector<uint32_t> currentEntities;

            for (uint32_t i = 0; i < entitiesCount; ++i) {
                uint32_t entity = 0;
                if (!Mem.ReadFast2(entities + i * 0x10, &entity)) continue;
                if (entity == 0 || entity == localPlayer) continue;

                currentEntities.push_back(entity);

                Player& playerEntry = g_Globals.EspConfig.Entities[entity];
                if (!EntityData(entity, playerEntry, mainPos)) {
                    g_Globals.EspConfig.Entities.erase(entity);
                }
                else {
                    validEntities++;
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

            sprintf_s(msg, "Valid entities processed: %d", validEntities);


            SyncContextWithEspConfig();

            if (g_Globals.AimBot.NoRecoil) {
                uint32_t weaponPtr = 0;
                if (Mem.Read<uint32_t>(localPlayer + OffsetsffNormal::Weapon, weaponPtr) && weaponPtr != 0) {
                    uint32_t weaponData = 0;
                    if (Mem.Read<uint32_t>(weaponPtr + OffsetsffNormal::WeaponData, weaponData) && weaponData != 0) {
                        float recoil = 0.0f;
                        if (Mem.Read<float>(weaponData + OffsetsffNormal::WeaponRecoil, recoil) && recoil != 0.0f) {
                            Mem.Write<float>(weaponData + OffsetsffNormal::WeaponRecoil, 0.0f);
                        }
                    }
                }
            }

            if (g_Globals.AimBot.FastSwitch) {
                uint32_t currentWeaponPtr = 0;
                if (Mem.Read<uint32_t>(localPlayer + OffsetsffNormal::Weapon, currentWeaponPtr) && currentWeaponPtr != 0) {
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
                                if (Mem.ReadFast2(bestTarget->Address + OffsetsffNormal::ColliderHECFNHJKOMN, &rHeadCollider) && rHeadCollider) {
                                    float randomValue = static_cast<float>(rand()) / RAND_MAX;
                                    if (randomValue <= aimAIStrength) {
                                        uintptr_t patchAddr = bestTarget->Address + OffsetsffNormal::ColliderINICDNFOFJB;
                                        
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
                        Mem.Write(localPlayer + OffsetsffNormal::AimRotation, aimRotation);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(g_Globals.AimBot.RageSmooth));
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            
            //AimLogic::Tick(false);
            //SpinBot::Tick(false);




        }
        catch (...) {

        }
    }

    bool Data::EntityData(uint32_t entity, Player& player, Vector3& mainPos) {
        try {
            if (entity == 0 || entity == (uint32_t)-1) return false;

            uint32_t avatarManager = 0;
            if (!Mem.ReadFast2(entity + OffsetsffNormal::AvatarManager, &avatarManager) || !avatarManager || avatarManager == (uint32_t)-1) return false;

            uint32_t avatar = 0;
            if (!Mem.ReadFast2(avatarManager + OffsetsffNormal::Avatar, &avatar) || !avatar || avatar == (uint32_t)-1) return false;

            uint32_t avatarData = 0;
            if (!Mem.ReadFast2(avatar + OffsetsffNormal::Avatar_Data, &avatarData) || !avatarData || avatarData == (uint32_t)-1) return false;

            Mem.ReadFast2(avatar + OffsetsffNormal::Avatar_IsVisible, &player.IsVisible);

            bool isTeam = false;
            Mem.ReadFast2(avatarData + OffsetsffNormal::Avatar_Data_IsTeam, &isTeam);
            player.IsTeam = isTeam ? Player::Bool3::True : Player::Bool3::False;

            player.IsKnown = true;

            Mem.ReadFast2(entity + OffsetsffNormal::Player_IsDead, &player.IsDead);
            if (player.IsDead) return false;


            uint32_t dataPool = 0;
            player.Health = 100;

            if (Mem.ReadFast2(entity + OffsetsffNormal::Player_Data, &dataPool) && dataPool != 0 && dataPool != (uint32_t)-1) {
                uint32_t poolObj = 0;
                if (Mem.ReadFast2(dataPool + 0x8, &poolObj) && poolObj != 0 && poolObj != (uint32_t)-1) {
                    uint32_t pool = 0;
                    if (Mem.ReadFast2(poolObj + 0x10, &pool) && pool != 0 && pool != (uint32_t)-1) {

                        short hpShort = 0;
                        float hpFloat = 0.0f;
                        bool hpRead = false;


                        if (Mem.ReadFast2(pool + OffsetsffNormal::Vida, &hpShort) && hpShort > 0 && hpShort <= 500) {
                            player.Health = hpShort;
                            hpRead = true;
                        }

                        if (!hpRead && Mem.ReadFast2(pool + 0x10, &hpShort) && hpShort > 0 && hpShort <= 500) {
                            player.Health = hpShort;
                            hpRead = true;
                        }

                        if (!hpRead && Mem.ReadFast2(pool + 0x8, &hpShort) && hpShort > 0 && hpShort <= 500) {
                            player.Health = hpShort;
                            hpRead = true;
                        }

                        if (!hpRead && Mem.ReadFast2(pool + OffsetsffNormal::Vida, &hpFloat) && hpFloat > 0.1f && hpFloat <= 500.0f) {
                            player.Health = (short)hpFloat;
                            hpRead = true;
                        }
                        if (!hpRead && Mem.ReadFast2(pool + 0x10, &hpFloat) && hpFloat > 0.1f && hpFloat <= 500.0f) {
                            player.Health = (short)hpFloat;
                            hpRead = true;
                        }

                        if (!hpRead) player.Health = 200;

                        uint32_t weaponptr = 0;
                        if (Mem.ReadFast2(poolObj + 0x20, &weaponptr) && weaponptr != 0 && weaponptr != (uint32_t)-1) {
                            Mem.ReadFast2(weaponptr + 0x10, &player.WeaponID);
                        }
                    }
                }
            }

            player.Address = entity;


            uint32_t shadowBase = 0;
            player.IsKnocked = false;
            if (Mem.ReadFast2(entity + OffsetsffNormal::Player_ShadowBase, &shadowBase) && shadowBase != 0 && shadowBase != (uint32_t)-1) {
                int xpose = 0;
                if (Mem.ReadFast2(shadowBase + OffsetsffNormal::XPose, &xpose)) {
                    player.Pose = static_cast<XPose>(xpose);
                    player.IsKnocked = (xpose == 8);
                }
            }

            Mem.ReadFast2(entity + OffsetsffNormal::IsClientBot, &player.IsBot);


            struct BoneMap { uintptr_t offset; Vector3 Player::* member; };
            static const BoneMap boneMaps[] = {
                { OffsetsffNormal::BonesffNormal::Head, &Player::Head },
                { OffsetsffNormal::BonesffNormal::Neck, &Player::Neck },
                { OffsetsffNormal::BonesffNormal::Hip, &Player::Hip },
                { OffsetsffNormal::BonesffNormal::Root, &Player::Root },
                { OffsetsffNormal::BonesffNormal::LeftShoulder, &Player::LeftShoulder },
                { OffsetsffNormal::BonesffNormal::RightShoulder, &Player::RightShoulder },
                { OffsetsffNormal::BonesffNormal::LeftElbow, &Player::LeftElbow },
                { OffsetsffNormal::BonesffNormal::RightElbow, &Player::RightElbow },
                { OffsetsffNormal::BonesffNormal::LeftWrist, &Player::LeftWrist },
                { OffsetsffNormal::BonesffNormal::RightWrist, &Player::RightWrist },
                { OffsetsffNormal::BonesffNormal::LeftFoot, &Player::LeftFoot },
                { OffsetsffNormal::BonesffNormal::RightFoot, &Player::RightFoot },
                { OffsetsffNormal::BonesffNormal::LeftAnkle, &Player::LeftAnkle },
                { OffsetsffNormal::BonesffNormal::RightAnkle, &Player::RightAnkle }
            };

            for (const auto& bm : boneMaps) {
                uint32_t boneAddr = 0;
                if (Mem.ReadFast2(entity + bm.offset, &boneAddr) && boneAddr && boneAddr != (uint32_t)-1) {
                    TransformUtils2::GetNodePosition(boneAddr, player.*(bm.member));
                }
            }

            if (player.Head == Vector3::Zero()) return false;

            // Tính velocity
            if (player.LastRootPosition != Vector3::Zero()) {
                player.Velocity = player.Root - player.LastRootPosition;
            } else {
                player.Velocity = Vector3::Zero();
            }
            player.LastRootPosition = player.Root;

            if (!(player.Head == Vector3::Zero())) {
                player.Distance = Vector3::Distance(mainPos, player.Head);
            }

            if (player.Distance > 1000 || player.Distance < 0) return false;

            if (player.Name.empty()) {
                uint32_t nameAddr = 0;
                if (Mem.Read(entity + OffsetsffNormal::Player_Name, nameAddr) && nameAddr != 0 && nameAddr != (uint32_t)-1) {
                    int nameLen = 0;
                    if (Mem.Read(nameAddr + 0x8, nameLen) && nameLen > 0 && nameLen < 64) {
                        std::string name = Mem.ReadStringZ2(nameAddr + 0xC, nameLen);
                        if (!name.empty() && name.length() < 32) {
                            player.Name = name;
                        }
                    }
                }
            }

            return true;
        }
        catch (...) {
            return false;
        }
    }

    void Data::Reset() {
        try {
            if (!g_Globals.EspConfig.Entities.empty()) {
                g_Globals.EspConfig.Entities.clear();

            }

            g_Globals.EspConfig.previousCount = 0;
            g_Globals.EspConfig.LocalPlayer = 0;
            g_Globals.EspConfig.IsRealPlayerAlive = false;
            savedLocalPlayer = 0;

            Mem.Cache.clear();

            g_Globals.EspConfig.Matrix = false;
            g_Globals.EspConfig.ViewMatrix = Matrix4x4();
            g_Globals.EspConfig.MainCamera = Vector3(0, 0, 0);


        }
        catch (...) {
        }
    }
}
