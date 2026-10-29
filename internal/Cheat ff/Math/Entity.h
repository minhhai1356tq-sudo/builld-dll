#pragma once
#include <cstdint>
#include <string>
#include <chrono>
#include <unordered_map>
#include <shared_mutex>
#define NOMINMAX
#include <Windows.h>
#include "FrameWork/Math/Vectors/Vector2D.hpp"
#include "FrameWork/Math/Vectors/Vector3D.hpp"
#include "Matrix4v4.hpp"


enum class Bool3 { True, False, Unknown };

class Entity {
public:


    Vector2 ScreenHead{ 0.f, 0.f };
    Vector2 ScreenRoot{ 0.f, 0.f };
    float BoxW = 0.f;
    float BoxH = 0.f;
    bool Renderable = false;

    int EP = 0;
    int Shield = 0;

    bool IsKnown = false;
    bool IsBot = false;
    Bool3 IsTeam = Bool3::Unknown;
    bool IsDead = false;
    bool IsKnocked = false;
    short Health = 0;
    short Gun = 0;
    float Distance = 0.f;
    bool IsVisible = false;
    uint32_t Address = 0;

    uint32_t AvatarManager = 0;
    uint32_t Avatar = 0;
    uint32_t AvatarData = 0;

    Vector3 Root{};
    Vector3 Head{};
    Vector3 Spine{};
    Vector3 Hip{};
    Vector3 Neck{};
    Vector3 LeftShoulder{};
    Vector3 LeftElbow{};
    Vector3 LeftWrist{};
    Vector3 LeftHand{};
    Vector3 RightShoulder{};
    Vector3 RightElbow{};
    Vector3 RightWrist{};
    Vector3 RightHand{};
    Vector3 LeftCalf{};
    Vector3 LeftFoot{};
    Vector3 RightCalf{};
    Vector3 RightFoot{};

    std::string Name;
    uint64_t ID = 0;
    int Level = 0;
    std::string WeaponName;
    int WeaponIndex = 0;

    std::chrono::steady_clock::time_point lastSeen{};


    uint64_t lastSeenTick = 0;
    uint32_t failStreak = 0;

    uint32_t visibleScore = 0;
    bool renderVisible = false;
    static constexpr uint32_t VISIBLE_MAX = 6;
    static constexpr uint32_t VISIBLE_TH_ON = 3;
    static constexpr uint32_t VISIBLE_TH_OFF = 2;

    bool    hadGoodBones = false;
    Vector3 lastGoodHead{};
    Vector3 lastGoodRoot{};
    Matrix4x4 lastGoodMatrix{};
    short   lastGoodHealth = 0;
    short   lastGoodShield = 0;
    int     lastGoodEP = 0;
    std::string lastGoodName;

    void OnInserted(uint64_t tickNow) {
        lastSeenTick = tickNow;
        failStreak = 0;
        visibleScore = 0;
        renderVisible = false;
        hadGoodBones = false;
    }
};






class Context {
public:
    static inline uint32_t MatchStatusPreview = 0;
    static inline std::shared_mutex EntityMutex;


    static inline uint32_t LibraryAddress = 0;
    static inline bool      HasMatrix = false;
    static inline Matrix4x4 ViewMatrix{};

    static inline std::unordered_map<uint32_t, Entity> Entities;
    static inline std::unordered_map<uint32_t, Entity> LastValidEntities;
    std::unordered_map<uint32_t, Entity> emptyMap;


    static inline int      MatchStatus = 0;
    static inline Vector3  LocalMainCamera{};
    static inline uint32_t LocalPlayerAddress = 0;
    static inline uint32_t LocalMainCameraAddress = 0;
    static inline uint32_t LocalPlayerOrObserver = 0;
    static inline uint32_t CurrentMatch = 0;
    static inline bool     LocalPlayerIsFiring = false;
    static inline bool     LocalPlayerInMatch = false;

    static inline int WindowWidth = 0;
    static inline int WindowHeight = 0;
    static inline int WindowX = 0;
    static inline int WindowY = 0;

    static inline HWND EmulatorHwnd = nullptr;

    static inline float ViewYaw = 0.0f;


    static inline int   previoustotalPlayers = 0;
    static inline int   totalPlayers = 0;
    static inline int   totalPlayersStatic = 0;
    static inline int   maxPlayersDetected = 0;
    static inline std::string tipoDePartida = "Unknown CT";
    static inline std::string tipoDePartidaAnterior = "Unknown CT";
    static inline std::string tipoDePartidaRPC = "Unknown CT";
    static inline bool        RPCMatrixActivate = true;
};
