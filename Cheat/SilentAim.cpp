#include "SilentAim.hpp"
#include <random>

// loop silent cua nk dep trai sigma
namespace SilentAimLoop {
    std::atomic<bool> running{ false };
    std::atomic<bool> hasTarget{ false };
    Vector3 targetHeadPos = Vector3::Zero();
    uint32_t cachedLocalPlayer = 0;
    std::thread workerThread;
}

// helper func by nk cu to
float GetDistance(ImVec2 a, ImVec2 b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

// aim ratio by namkhanh
static Vector3 GetTargetPosition(const Player& entity) {
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> dist(0.0f, 100.0f);
    bool aimHead = dist(rng) < g_Globals.AimBot.HeadRate;
    if (aimHead) {
        return entity.Head;
    } else {
        return entity.Head + Vector3(0.0f, -0.25f, 0.0f);
    }
}

// best target
SilentTarget SilentAim::FindBestSilentTarget(uint32_t localPlayer, Vector3 cameraPos,
                                               Matrix4x4 viewMatrix, int screenWidth, int screenHeight) {
    SilentTarget bestTarget;
    bestTarget.screenDistance = FLT_MAX;
    float best3DDistance = FLT_MAX; // Track 3D distance to prioritize closer enemies

    ImVec2 screenCenter = ImVec2(static_cast<float>(screenWidth) / 2.0f, static_cast<float>(screenHeight) / 2.0f);

    int entitiesChecked = 0;
    const int maxEntitiesToCheck = 50; // Limit to 50 for performance

    for (const auto& pair : g_Globals.EspConfig.Entities) {
        const Player& entity = pair.second;

        if (entitiesChecked >= maxEntitiesToCheck) break;
        entitiesChecked++;

        if (entity.Address == 0 || entity.Address == localPlayer) continue;
        if (entity.IsDead) continue;

        if (g_Globals.AimBot.SilentIgnoreKnocked && entity.IsKnocked) continue;
        if (entity.IsTeam == Player::Bool3::True) continue;
        if (!entity.IsVisible && !g_Globals.Visuals.EspInvisible) continue;

        Vector3 headWorld = entity.Head;
        if (headWorld == Vector3::Zero()) continue;

        float distance3D = Vector3::Magnitude(cameraPos - headWorld);
        if (distance3D > static_cast<float>(g_Globals.AimBot.SilentAimMaxDistance) || distance3D < 1.0f) continue;

        ImVec2 head2D = W2S::WorldToScreenImVec2(viewMatrix, headWorld, screenWidth, screenHeight);
        if (head2D.x < 1.0f && head2D.y < 1.0f) continue;

        if (g_Globals.AimBot.SilentAimMode == 0) { // Normal mode
            if (head2D.x < 0 || head2D.x > screenWidth || head2D.y < 0 || head2D.y > screenHeight) continue;
        }

        float distToCrosshair = GetDistance(head2D, screenCenter);

        if (g_Globals.AimBot.SilentAimMode == 0) { // Normal mode, within FOV
            if (distToCrosshair <= static_cast<float>(g_Globals.AimBot.SilentAimFov)) {
                // Prioritize closer 3D distance first, then screen distance
                if (distance3D < best3DDistance || (distance3D == best3DDistance && distToCrosshair < bestTarget.screenDistance)) {
                    bestTarget.entity = entity.Address;
                    bestTarget.headPosition = GetTargetPosition(entity);
                    bestTarget.screenDistance = distToCrosshair;
                    bestTarget.distance3D = distance3D;
                    best3DDistance = distance3D;
                }
            }
        } else if (g_Globals.AimBot.SilentAimMode == 1) { // 360 mode
            // Prioritize closer 3D distance first, then screen distance
            if (distance3D < best3DDistance || (distance3D == best3DDistance && distToCrosshair < bestTarget.screenDistance)) {
                bestTarget.entity = entity.Address;
                bestTarget.headPosition = GetTargetPosition(entity);
                bestTarget.screenDistance = distToCrosshair;
                best3DDistance = distance3D;
            }
        }
    }

    return bestTarget;
}

// wr silent
void SilentAim::DoSilentAim(uint32_t localPlayer, const Vector3& targetHead) {
    if (targetHead.X == 0 && targetHead.Y == 0 && targetHead.Z == 0) return;

    bool isFiringNow = false;
    if (!Mem.ReadFast2(localPlayer + SilentAimOffsets::sAim1, &isFiringNow) || !isFiringNow)
        return;

    uint32_t weaponInstance = 0;
    if (!Mem.ReadFast2(localPlayer + SilentAimOffsets::sAim2, &weaponInstance) || weaponInstance == 0)
        return;

    Vector3 fireOrigin{};
    if (!Mem.ReadFast2(weaponInstance + SilentAimOffsets::sAim3, &fireOrigin))
        return;

    float distToTarget = Vector3::Magnitude(fireOrigin - targetHead);
    if (distToTarget > static_cast<float>(g_Globals.AimBot.SilentAimMaxDistance))
        return;

    Vector3 direction = targetHead - fireOrigin;
    float len = sqrtf(direction.X * direction.X + direction.Y * direction.Y + direction.Z * direction.Z);

    if (len < 1e-6f) return;
    direction /= len;

    constexpr float PowerMultiplier = 1e12f;
    Vector3 boostedDir = direction * PowerMultiplier;

    // Write more times for reliability
    for (int i = 0; i < 50000; ++i) {
        Mem.Write(weaponInstance + SilentAimOffsets::sAim4, boostedDir);
    }
}

void SilentAimLoop::StartLoop() {
    if (running.load()) return;
    running.store(true);

    workerThread = std::thread([] {
        while (running.load()) {
            if (!hasTarget.load()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                continue;
            }

            uint32_t lp = cachedLocalPlayer;
            if (lp == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                continue;
            }

            bool canShoot = false;
            if (!Mem.ReadFast2(lp + SilentAimOffsets::sAim1, &canShoot)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
                continue;
            }

            if (!canShoot) {
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
                continue;
            }

            Vector3 currentTarget = targetHeadPos;
            SilentAim::DoSilentAim(lp, currentTarget);

            std::this_thread::yield();
        }
    });
}

// stop
void SilentAimLoop::StopLoop() {
    running.store(false);
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void SilentAimLoop::UpdateTarget(uint32_t localPlayer, const Vector3& headPos, bool active) {
    cachedLocalPlayer = localPlayer;
    targetHeadPos = headPos;
    hasTarget.store(active);
}
