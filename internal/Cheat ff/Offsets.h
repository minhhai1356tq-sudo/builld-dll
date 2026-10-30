#pragma once
#include <cstdint>

// Rebuilt from dump.cs (FF V7A) - Verified against dump scan results
// Note: HP & Armor are server-side, cannot be read/written. ESP, Bone, Weapon are client-side.

class OffsetsffNormal {
public:
    static inline uintptr_t Il2Cpp = 0x0;
    static inline uintptr_t InitBase = 0xA342EFC; // Đã có offset chính xác từ Steph Wolf Dump
    static inline uintptr_t StaticClass = 0x5C;

    static inline uintptr_t CurrentMatch = 0x50;
    static inline uintptr_t MatchStatus = 0x8C;
    static inline uintptr_t LocalPlayer = 0x94;
    static inline uintptr_t DictionaryEntities = 0x68;

    static inline uintptr_t Player_IsDead = 0x54; // PlayerData.isDead
    static inline uintptr_t Player_Name = 0x320;  // Player_OriginalNickName
    static inline uintptr_t Player_Data = 0x48; 
    
    static inline uintptr_t Player_TeamModeID = 0x2D4; // Thêm mới từ dump
    static inline uintptr_t Player_IsClientBot = 0x324; // Thêm mới từ dump
    static inline uintptr_t IsClientBot = 0x324; // Alias cho v7a.cpp
    static inline uintptr_t Player_ShadowBase = 0x14E8; // Player.PhysXData
    
    static inline uintptr_t XPose = 0x78;

    static inline uintptr_t AvatarManager = 0x504;
    static inline uintptr_t Avatar = 0x828; // Player.Avatar
    static inline uintptr_t Avatar_IsVisible = 0x95; 
    static inline uintptr_t Avatar_Data = 0x14;
    static inline uintptr_t Avatar_Data_IsTeam = 0x51;

    static inline uintptr_t FollowCamera = 0x494;
    static inline uintptr_t Camera = 0x18;
    static inline uintptr_t AimRotation = 0x440;
    static inline uintptr_t MainCameraTransform = 0x28C;
    
    // ViewMatrix (Dùng để vẽ ESP)
    static inline uintptr_t ViewMatrix = 0xE8;

    static inline uintptr_t Weapon = 0x434; // ActiveUISightingWeapon
    static inline uintptr_t WeaponData = 0x58;
    static inline uintptr_t WeaponRecoil = 0xC;

    static inline uintptr_t Vida = 0xC;
    static inline uintptr_t LevelUp = 0x14A8;
    
    static inline uintptr_t PlayerAttributes = 0x500;
    static inline uintptr_t LocalPlayerAttributes = 0x500;

    static inline uintptr_t isfiring = 0x58C;
    
    // Giữ nguyên các offset cũ đã verify hoặc map lại
    static inline uintptr_t pomba = 0x4D0; 
    static inline uintptr_t bisteca = 0x8DC;
    static inline uintptr_t arma = 0x38;
    static inline uintptr_t tiro = 0x2C;
    static inline uintptr_t Lock2X = 0x500;
    static inline uintptr_t ColliderHECFNHJKOMN = 0x4A8;
    static inline uintptr_t ColliderINICDNFOFJB = 0x54;
    static inline uintptr_t RunSpeedUpScaleMAX = 0x210;
    static inline uintptr_t FallingSpeedUpScaleMAX = 0x20C;
    static inline uintptr_t GameTimerMAX = 0x24;
    static inline uintptr_t BuffWeaponMoveSpeedScaleMAX = 0x100;
    static inline uintptr_t NoReload2 = 0xC0;
    static inline uintptr_t GameTimer = 0x10;
    static inline uintptr_t GameVariables = 0xB0;
    static inline uintptr_t CurrentObserver = 0xB4;
    static inline uintptr_t ObserverPlayer = 0x28;
    static inline uintptr_t RightcameraOffset = 0x44;
    static inline uintptr_t FOVcameraoffset = 0x50;
    static inline uintptr_t Backcameraoffset = 0x4C;
    static inline uintptr_t Upcameraoffset = 0x48;
    static inline uintptr_t Phase1CameraEulerAnglesY = 0x48;
    static inline uintptr_t WeaponOnHand = 0x54;
    static inline uintptr_t AuxAimRotation = 0x12C4; // m_CurrentAimRotation
    static inline uintptr_t bullet_hit = 0x2C;
    static inline uintptr_t telepneu = 0x0;
    static inline uintptr_t GhostMode = 0x38;
    static inline uintptr_t LPEIEILIKGC = 0x478;

    class BonesffNormal {
    public:
        static inline uintptr_t Head = 0x49C;
        static inline uintptr_t Neck = 0x4A4;
        static inline uintptr_t Hip = 0x4A0;
        static inline uintptr_t RightShoulder = 0x4D4;
        static inline uintptr_t LeftShoulder = 0x4D0;
        static inline uintptr_t LeftHand = 0x4DC;
        static inline uintptr_t RightHand = 0x4D8;
        static inline uintptr_t LeftAnkle = 0x4C0;
        static inline uintptr_t RightAnkle = 0x4C4;
        static inline uintptr_t Hand = 0x4D8;
        static inline uintptr_t Root = 0x4B0;
        static inline uintptr_t LeftFoot = 0x4C0;
        static inline uintptr_t RightFoot = 0x4C4;
        static inline uintptr_t RootBone = 0x4B0;
        static inline uintptr_t Groin = 0x4A0;
        static inline uintptr_t RightElbow = 0x4E0;
        static inline uintptr_t LeftElbow = 0x4E4;
        static inline uintptr_t RightWrist = 0x4C8;
        static inline uintptr_t LeftWrist = 0x4C8;
    };
};

class OffsetsffmaxBeta {
public:
    static inline uintptr_t Il2Cpp = 0x0;
    static inline uintptr_t InitBase = 0xA342EFC; // Sửa lại cho đúng chuẩn
    static inline uintptr_t StaticClass = 0x5C;
    static inline uintptr_t Libanogs = 0x0;
    static inline uintptr_t Libunity = 0x2514;

    static inline uintptr_t CurrentMatch = 0x50;
    static inline uintptr_t MatchStatus = 0x8C;
    static inline uintptr_t LocalPlayer = 0x94;
    static inline uintptr_t DictionaryEntities = 0x68;

    static inline uintptr_t Player_IsDead = 0x54;
    static inline uintptr_t Player_Name = 0x320;
    static inline uintptr_t Player_Data = 0x48;
    
    static inline uintptr_t PlayerID = 0x298;
    static inline uintptr_t XPose = 0x78;
    static inline uintptr_t Player_ShadowBase = 0x14E8;

    static inline uintptr_t AvatarManager = 0x504;
    static inline uintptr_t Avatar = 0x828;
    static inline uintptr_t Avatar_Data = 0x14;
    static inline uintptr_t Avatar_IsVisible = 0x95;
    static inline uintptr_t Avatar_Data_IsTeam = 0x51;

    static inline uintptr_t FollowCamera = 0x494;
    static inline uintptr_t Camera = 0x18;
    static inline uintptr_t MainCameraTransform = 0x28C;
    static inline uintptr_t ViewMatrix = 0xE8;
    
    static inline uintptr_t RightcameraOffset = 0x44;
    static inline uintptr_t Upcameraoffset = 0x48;
    static inline uintptr_t Backcameraoffset = 0x4C;
    static inline uintptr_t FOVcameraoffset = 0x50;
    static inline uintptr_t Phase1CameraEulerAnglesY = 0x48;

    static inline uintptr_t AimRotation = 0x440;
    static inline uintptr_t AuxAimRotation = 0x12C4;
    static inline uintptr_t AimbotVisible = 0x434;
    static inline uintptr_t aimbotai = 0x434;

    static inline uintptr_t Weapon = 0x434;
    static inline uintptr_t WeaponData = 0x58;
    static inline uintptr_t WeaponRecoil = 0xC;
    static inline uintptr_t WeaponName = 0x38;
    static inline uintptr_t WeaponInfo = 0x64;
    static inline uintptr_t WeaponID = 0x14;
    static inline uintptr_t WeaponOnHand = 0x54;
    static inline uintptr_t WeaponPointer = 0x8F0;
    static inline uintptr_t playerWeaponInfoClass = 0x460;
    static inline uintptr_t UnkPlayerWeaponInfoClass = 0x400;
    static inline uintptr_t bullet_hit = 0x2C;
    static inline uintptr_t guntipposition = 0x38;
    static inline uintptr_t knockdown_real_damage = 0x344;

    static inline uintptr_t pomba = 0x4D0;
    static inline uintptr_t bisteca = 0x8DC;
    static inline uintptr_t arma = 0x38;
    static inline uintptr_t tiro = 0x2C;
    static inline uintptr_t SilentAimV1 = 0x4A8;
    static inline uintptr_t IS_FIRING = 0x58C;
    static inline uintptr_t IsAiming = 0x4D0;
    static inline uintptr_t sAim1 = 0x58C;
    static inline uintptr_t sAim2 = 0x9D0;
    static inline uintptr_t sAim3 = 0x38;
    static inline uintptr_t sAim4 = 0x2C;

    static inline uintptr_t HeadCollider = 0x4E8;
    static inline uintptr_t searchHex = 0x54;

    static inline uintptr_t IsClientBot = 0x324;
    static inline uintptr_t BotOffset = 0xC0;
    static inline uintptr_t isBotOffs = 0xC0;
    static inline uintptr_t Vida = 0xC;
    static inline uintptr_t GhostMode = 0x38;
    static inline uintptr_t telepneu = 0x0;
    static inline uintptr_t LPEIEILIKGC = 0x478;
    static inline uintptr_t LevelUp = 0x14A8;
    static inline uintptr_t Neck = 0x4A4;

    static inline uintptr_t PlayerAttributes = 0x500;
    static inline uintptr_t LocalPlayerAttributes = 0x500;

    static inline uintptr_t RunSpeedUpScale = 0x210;
    static inline uintptr_t WalkDownSpeedScale = 0x1C0;
    static inline uintptr_t FallingSpeedUpScale = 0x20C;
    static inline uintptr_t RunSpeedUpScaleMAX = 0x210;
    static inline uintptr_t FallingSpeedUpScaleMAX = 0x20C;
    static inline uintptr_t BuffWeaponMoveSpeedScaleMAX = 0x100;
    static inline uintptr_t BuffWeaponMoveSpeedScale = 0x100;
    static inline uintptr_t MoveSpeedScale = 0x100;
    static inline uintptr_t BuffWeaponDamageScale = 0xF8;

    static inline uintptr_t InfiniteAmmo = 0x104;
    static inline uintptr_t NoReload = 0xC1;
    static inline uintptr_t NoReload2 = 0xC0;
    static inline uintptr_t ShootNoReload = 0xC1;

    static inline uintptr_t InstantFire = 0x0; 
    static inline uintptr_t InstantFire1 = 0x178;
    static inline uintptr_t InstantFire2 = 0x17C;
    static inline uintptr_t InstantFire3 = 0x180;
    static inline uintptr_t InstantFire4 = 0x188;

    static inline uintptr_t m_FireIntervalScaleMAX = 0x1D0;
    static inline uintptr_t m_FireIntervalScaleSkillMAX = 0x1D4;
    static inline uintptr_t m_FireIntervalScaleTwoMAX = 0x180;
    static inline uintptr_t m_FireIntervalScale = 0x1D0;
    static inline uintptr_t m_FireIntervalScaleSkill = 0x1D4;
    static inline uintptr_t m_FireIntervalScaleTwo = 0x184;

    static inline uintptr_t IsActiveSkill = 0x774;
    static inline uintptr_t TatsuyaBoost = 0x1C0;
    static inline uintptr_t TimerPtr = 0x10;
    static inline uintptr_t Timer = 0x24;
    static inline uintptr_t GameTimer = 0x10;
    static inline uintptr_t FixedDeltaTime = 0x24;
    static inline uintptr_t GameTimerMAX = 0x24;
    static inline uintptr_t GameVariables = 0xB0;

    static inline uintptr_t CurrentObserver = 0xB4;
    static inline uintptr_t ObserverPlayer = 0x64;

    static inline uintptr_t BaseProfileInfo = 0x1A74;
    static inline uintptr_t WeaponSwap = 0x1517CAC;

    static inline uintptr_t UsePhysicalCharacterController = 0x0;
    static inline uintptr_t lastCharacterControllerCenterY = 0x0;
    static inline uintptr_t RealCharacterControllerCenterYdelta = 0x0;

    static inline uintptr_t FiringSilent = 0x4D0;
    static inline uintptr_t SilentBulletDataPtrOffset = 0x8DC;
    static inline uintptr_t AimRotationFov = 0x398;

    static inline uint32_t MOVEMENT_COMP = 0x1178;
    static inline uint32_t POSITION_OFF = 0x1C;
    static inline uint32_t VSPEED_OFF = 0x28;
    static inline uint32_t GROUNDED_OFF = 0x150;

    class BonesffmaxBeta {
    public:
        static inline uintptr_t Head = 0x49C;
        static inline uintptr_t Root = 0x4B0;
        static inline uintptr_t Spine = 0x4A8;

        static inline uintptr_t Hip = 0x4A0;
        static inline uintptr_t LeftShoulder = 0x4D0;
        static inline uintptr_t RightShoulder = 0x4D4;
        static inline uintptr_t LeftElbow = 0x4E4;
        static inline uintptr_t RightElbow = 0x4E0;
        static inline uintptr_t LeftWrist = 0x4C8;
        static inline uintptr_t RightWrist = 0x4C8;
        static inline uintptr_t LeftWristJoint = 0x4C8;
        static inline uintptr_t RightWristJoint = 0x4C8;
        static inline uintptr_t LeftHand = 0x4DC;
        static inline uintptr_t LeftCalf = 0x4B8;
        static inline uintptr_t RightCalf = 0x4BC;
        static inline uintptr_t LeftFoot = 0x4C0;
        static inline uintptr_t RightFoot = 0x4C4;
    };
};
