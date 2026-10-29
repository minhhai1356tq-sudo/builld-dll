#pragma once
#include <Windows.h>
#include <unordered_map>
#include <mutex>
#include <internal/Cheat ff/Player.h>
#include <internal/Cheat ff/Math/Matrix4v4.hpp>
#include <FrameWork/Math/Vectors/Vector3D.hpp>
#include <ImGui/imgui.h>
#include <thread>
#include <atomic>
#include <chrono>
class Globals
{
public:
    struct Esp {
        std::unordered_map<long, Player> Entities;
        std::mutex EntitiesMutex;
        Matrix4x4 ViewMatrix{};
        Vector3 MainCamera{};
        bool IsRealPlayerAlive;
        uint32_t LocalPlayer = 0;
        uint32_t LocalMainCameraAddress = 0;
        uint32_t cachedStaticGameFacade = 0;
        uint32_t previousCount = 0;
        uint32_t lastentity = 0;
        bool Matrix = false;
        int Width = 0;
        int Height = 0;
        bool visible = true;
        bool telemark = false;
        float skeletonThickness = 1.5f;
        int telemarkkey = 0;
        bool ignorealtrue = false;
        static inline HWND EmulatorHwnd = nullptr;
    } EspConfig;

    struct AimBot {
        bool NoClipEnabled = false;

        bool AimbotAIEnabled = false;
        bool AimbotLegit = false;
        int AimbotAIDelay = 100;
        float AimbotAIStrength = 1.0f;

        bool AimbotRage = false;
        int AimbotRageBind = VK_LBUTTON;
        int HitBox = 0; // 0: Head, 1: Neck, 2: Chest/Hip
        int RageFov = 100; // 1-500
        int DistanceAim = 150; // Max aim distance
        bool ShowFOV = false;
        int RageSmooth = 1; // Sleep time in ms
        bool AimLogicEnabled = false;
        int MagicBulletFov = 860;
        bool MagicBulletEnabled = false;
        bool ShootMoveSpeedEnabled = false;
        int  ShootMoveSpeedValue = 13;
        bool SilentAimMaxEnabled = false;
        int  NoClipKey = VK_XBUTTON1;
        int  NoClipKeyMode = 0;
        int  NoClipSpeed = 5;
        bool TriggerBotEnabled = false;
        int  TriggerBotRadius = 15;
        bool LevitateEnabled = false;
        int  LevitateHeight = 10;
        bool LevitationEnabled = false;
        int LevitationBind = 0;
        int LevitationBindMode = 0;
        int LevitationHeight = 5;
        bool TeleportMark = false;
        bool OffAltura = false;
        int teleportMarkKey = 0;
        bool AIMBOT12 = false;
        bool CooldownHack = false;
        bool SAF_Enabled = false;
        bool SAF_Silent = true;
        bool SAF_AutoFire = true;
        bool SAF_CoverKill = true;
        int  SAF_Bind = VK_RBUTTON;
        int  SAF_BindMode = 0;
        int  SAF_FOV = 150;
        int  SAF_Distance = 300;

        int  SilentAimMaxFOV = 360;




        bool SpeedRunEnabled = false;
        int  SpeedRunValue = 15;
        bool SpeedRunFastFire = false;
        bool AimbotFaris = false;
        int  FarisFOV = 180;
        int  FarisDistance = 400;
        int  FarisSmooth = 1;
        int  FarisHeadOffset = 5;
        bool FarisIgnoreKnocked = true;
        bool FarisIgnoreBots = true;

        bool RageV2 = false;
        int  RageV2Fov = 180;
        int  RageV2Distance = 400;
        int  RageV2Smooth = 0;
        int  RageV2Bone = 0;
        bool RageV2IgnoreKnocked = true;
        bool RageV2IgnoreBots = true;
        int  RageV2HeadOffset = 5;

        bool ExtremeEnabled = false;
        bool ExtremeUseFOV = true;
        int  ExtremeFOV = 180;
        int  ExtremeTargetPriority = 0;

        bool FastFire = false;
        int  FastFireValue = 1;
        bool FastFireActive = false;
        bool FlyHackEnabled = false;
        int  FlyHackBind = 0;
        int  FlyHackBindMode = 0;
        bool noreload = false;
        int  BoneSwitchBind = 0;
        int  BoneSwitchBindMode = 0;

        int TargetPriority = 0;
        int MaxDistance = 300;
        int TargetMode = 0;
        bool  enable = false;
        int   aimKey = VK_LBUTTON;
        int   aimKeyMode = 0;
        bool  enable2 = false;
        int   delaytohead = 10;

        bool  rage = false;
        int   rageKey = VK_LBUTTON;
        int   rageKeyMode = 0;

        bool  ai = false;
        bool  AimBotRageMainId = false;
        bool  aimbotvisible = false;
        bool  AimbotAIEnabled2 = false;
        bool  AimbotFFMAX = false;
        int   AimbotBind = VK_LBUTTON;
        int   AimbotBindMode = 0;

        bool  MouseAim = false;
        int   MouseSmooth = 5;
        int   MouseAimBind = 0;

        bool  silentEnable = false;
        bool  silentCover = false;
        bool  silentaim = false;
        int   SilentBind = VK_LBUTTON;
        int   SilentBindMode = 0;
        bool  SilentIgnoreBots = true;
        bool  SilentV3Enabled = false;

        bool  VisionHackEnabled = false;
        int   VisionFOV = 60;

        bool  AimOriginBoost = false;
        int   AimOriginBoostValue = 15;

        bool  aimfoz = false;
        bool  visibleOnly = true;
        float yBias = 0.05f;
        int   aimMaxDistance = 300;
        float aimSmoothing = 4.0f;
        int   boneTarget = 0;
        int   aimSmoothness = 0;
        int   HSRate = 70;

        bool  Forzeaim = false;
        int   forzetype = 0;
        int   Forzeaimstreanth = 10000;
        int   changetype = 0;
        int   changetypeMode = 0;

        bool  AimLock = false;
        bool  AimLockActive = false;
        bool  AimLock2x = false;
        bool  AimLock2xActive = false;
        bool  AimRage = false;

        bool  FullBodyAim = false;
        int   FullBodyAimBind = VK_RBUTTON;
        int   FullBodyAimBindMode = 0;
        int   FullBodyAimFOV = 300;
        bool  FullBodyAimIgnoreKnocked = true;

        bool  AnchorAssistEnabled = false;
        int   AnchorStrength = 5;

        bool  EnemyPullHeadEnabled = false;
        int   PullHeadFOV = 140;
        int   PullHeadMaxDistance = 200;
        int   PullHeadStrengthX = 1;
        int   PullHeadStrengthY = 1;
        bool  PullHeadIgnoreKnocked = true;
        bool  PullHeadIgnoreBots = true;
        int   EnemyPullHeadTickMs = 1;
        bool  EnemyPull360Enabled = false;
        int   PullHeightOffset = 0;

        bool  BestPullEnabled = false;
        int   BestPullBind = 0;
        int   BestPullBindMode = 0;
        int   BestPullFOV = 360;
        int   BestPullMaxDistance = 400;
        int   BestPullStrength = 1;
        int   BestPullHeightOffset = 0;
        bool  BestPullIgnoreKnocked = true;
        bool  BestPullIgnoreBots = true;

        bool  pullEnable = false;
        int   pullStrength = 1;
        bool  EnemyPull = false;
        float PullEnemy_Speed = 0.010f;
        int   SelectPull = 0;
        int   changePullType = 0;
        int   enemyPullTickMs1 = 0;
        int   SmoothAim = 50;

        bool  CameraLeftEnabled = false;
        int CameraLeftX = -1.5;
        int CameraLeftY = 0;
        int CameraLeftZ = 0;
        bool SpeedHackEx = false;
        int  SpeedHackExValue = 25;
        bool  WallPushEnabled = false;

        bool FastMedkitEnabled = false;
        int  FastMedkitValue = 5;
        int AimOriginBoostAxis = 0;
        int  FlyPlayerBind = 0;
        int  FlyPlayerBindMode = 0;
        int   WallPushDistance = 20;
        int   WallPushDuration = 150;
        int   WallPushSteps = 30;
        int   SpinSpeed = 18;
        int  teleportMarkMode2 = 0;
        int  teleportMarkMode = 0;
        int  teleportMarkKey2 = 0;
        bool  SilentMaxEnabled = false;
        int   SilentMaxDistance = 300;
        bool  SilentMaxIgnoreKnock = false;

        bool  AdvTpEnabled = false;
        int   AdvTpKey = 0;
        int   AdvTpKeyMode = 0;
        int   AdvTpHoldMs = 800;
        int   AdvTpHeight = 0;
        bool TeleportMarkV2 = false;
        bool  MagnetEnabled = false;
        int   MagnetBind = VK_LBUTTON;
        int   MagnetBindMode = 0;
        int   MagnetFOV = 360;
        int   MagnetDistance = 500;
        int   MagnetStrength = 0;
        bool  MagnetIgnoreKnocked = true;
        bool  MagnetIgnoreBots = false;
        int   MagnetBone = 2;

        bool FlyPlayer = false;
        bool Telepneu = false;
        bool TelekillEnabled = false;
        bool UndergroundKillEnemy = false;
        bool UnderPlayer = false;
        bool ClimbUP = false;
        bool SpawnKill = false;
        bool  teleportMark = false;


        int  YOffset = 0;
        int  Range = 100;
        int  SpawnKillMaxDistance = 100;
        int  SpwYOffset = 0;


        int  TelekillKey = 0;
        int  TelekillMode = 0;


        int  TelepneuKey = 0;
        int  TelepneuMode = 0;




        int  SilentNormalKey = 0;
        int  SilentNormalMode = 0;


        int  Silent360Key = 0;
        int  Silent360Mode = 0;


        int  SilentCoverKey = 0;
        int  SilentCoverMode = 0;


        int  SpawnKillKey = 0;
        int  SpawnKillMode = 0;
        int   teleportMarkKeyMode = 0;
        bool  TeleportBase = false;
        int   TeleportBaseDistance = 50;
        int   TeleportBaseBind = 0;
        int   TeleportBaseBindMode = 0;

        bool  FlyHack = false;
        int   FlyUpKey = 0;
        int   FlyDownKey = 'C';
        int   FlySpeed = 3;
        bool  AntiKnock = false;
        bool  AntiAim = false;
        int   AntiAimSpeed = 5;
        bool  FovChanger = false;
        int   FovValue = 90;
        bool  FreezeEnemy = false;
        int   FreezeEnemyDistance = 300;
        bool  WallPull = false;
        int   WallPullBind = 0;
        int   WallPullBindMode = 0;
        int   WallPullDistance = 200;
        bool  LevelUpHack = false;
        int   LevelUpValue = 99;
        bool  ObserverSpectate = false;
        bool  SpeedHack = false;
        int   SpeedValue = 2;
        bool  Fastrun = false;
        bool  Speed = false;
        bool AimbotEnabled = false;
        bool SniperEnabled = false;
        int  SniperKey = VK_RBUTTON;
        int  SniperKeyMode = 0;
        bool DownPlayerEnabled = false;
        int  DownPlayerOffset = 3;
        int  DownPlayerKey = 0;
        int  DownPlayerKeyMode = 0;

        bool SpeedEnabled = false;
        bool  InstantFire = false;
        float InstantFireValue = 0.0f;
        bool  FastReload = false;
        bool  NoRecoil = false;
        bool  FastSwitch = false;
        bool  KnockdownDamage = false;
        bool  NoSpread = false;
        bool  RapidFire = false;
        int   RapidFireValue = 1;
        bool  ForceKill = false;
        int   ForceKillDistance = 300;
        bool  BulletThrough = false;
        bool  BulletThroughIgnoreKnocked = true;
        bool  SkillBoost = false;
        int   SkillBoostValue = 3;
        bool  InfiniteAmmo = false;

        bool  VisionBoostEnabled = false;
        int   VisionBoostPerspective = 48;
        int   VisionBoostHeight = 6;

        bool  SilentCoverEnabled = false;
        bool  SilentUltraMaxEnabled = false;
        int   SilentUltraMode = 3;

        bool  PullEnemy2Enabled = false;
        int  PullEnemyMode = 1;
        int  PullEnemySpeed = 15;

        bool  PullSilentEnabled = false;
        int  PullMaxX = 860;
        int  PullMaxY = 200;

        bool  TpToEnemyEnabled = false;
        int  TpToEnemyBind = 0;
        int  TpToEnemyBindMode = 0;

        bool  MapTeleportEnabled = false;
        int  MapTeleportBind = 0;
        int  MapTeleportBindMode = 0;
        int   MapTeleportMap = 0;
        int   MapTeleportIndex = 0;

        bool  FastHealEnabled = false;
        int   FastHealBind = 0;
        int   FastHealBindMode = 0;

        bool  AimOriginBoostEnabled = false;

        bool  Ghost = false;
        bool  GhostEnabled = false;
        int   GhostBind = 0;
        int   GhostBindMode = 0;

        bool  SpinBotEnabled = false;
        int   SpinBotBind = VK_RBUTTON;
        int   SpinBotBindMode = 0;

        bool  AirWalkEnabled = false;
        int   AirWalkBind = 0;
        int   AirWalkBindMode = 0;
        int   AirWalkHeight = 5;

        bool  UnderkillEnabled = false;
        int   UnderkillDistance = 1;
        int   UnderkillDelay = 1;

        bool  LegacyEnabled = false;
        bool  Enabled = false;
        bool  IgnoreKnocked = true;
        bool  IgnoreBots = false;
        bool  IgnoreDowned = false;
        int   DistanceExtreme = 10000;
        int   UpdateInterval = 100;
        int   ShotDelay = 100;
        float legacySmoothing = 0.5f;
        int   telekillinf = 0;
        int   telekillinf2 = 0;
        int   flytrigger = 0;
        int   Divekillx = 0;
        int   tponoff = 0;
        int   oscarinf = 0;
        int   Tpwallx = 0;

        bool   RgbMode = false;
        float  fovRadius = 60.0f;
        float  Fillcolor[4] = { 0.f, 0.f, 0.f, 0.2f };
        int    menuFov = 90;

        bool Silent360Enabled = false;
        int  Silent360Fov = 360;
        int  Silent360Distance = 500;
        bool Silent360IgnoreKnock = false;

        // Silent Aim Config
        bool SilentAimEnabled = false;
        int SilentAimBind = 0;
        int SilentAimMode = 0; // 0 = Normal, 1 = 360
        int SilentAimFov = 100;
        int SilentAimMaxDistance = 130;
        int HeadRate = 64;
        int BodyRate = 36;
        bool ShowSilentFOV = false;
        bool SilentIgnoreKnocked = false;
        bool ShowFovCircle = false;
        float fovThickness = 2.0f;

        bool  AimFovEnabled = false;
        int   AimFovKey = VK_RBUTTON;
        int   AimFovFOV = 150;
        int   AimFovMaxDistance = 300;
        int   AimFovMode = 0;
        bool  AimFovIgnoreKnock = false;
        int   AimFovSpeed = 92;

        std::thread pullThread;

    } AimBot;

    struct General {
        float PullEnemy_Speed = 0.010f;
        int   SelectPull = 0;
        bool MuteCheckboxSound = false;
        char FeedbackDiscordName[256] = "";
        int FeedbackStars = 5;
        char FeedbackText[2048] = "";
        bool  pendiclear = false;
        bool ShowPanelLogs = true;
        int CurrentGameVersion = 0;
        bool  ShowMenu = true;

        bool is_loading2 = false;
        float timer_loading2 = 0.0f;
        bool menu_opened;

        int selected_ff_version = -1;
        bool  mainmenu = false;
        char  Username[255] = "";
        char  Password[255] = "";
        char  Key[255] = "";
        char  License[255] = "";
        bool  isLoggedIn = false;
        bool  ShutDown = false;
        bool  Capture = false;
        int   SHUTDOWNKEY = VK_DELETE;
        int   Delay = 10;
        int   MenuKey = VK_INSERT;
        int   page = 0;
        int   Mode = 0;
        int   VariantIndex = 0;
        bool  hookstatus = false;
        bool  ishooked = false;
        int   refeshmemo = 1000;
        std::atomic<bool> pullThreadRunning{ false };
        std::atomic<bool> pullStop{ false };
        std::atomic<bool> silentThreadRunning{ false };
        std::atomic<bool> silentStop{ false };
    } General;

    struct timer {
        bool  timerStarted = true;
        bool  foundValidEnemy = false;
        bool  oneflagtime = false;
        bool  reset = false;
        bool  flagrefe = false;
        bool  entity = false;
        bool  showCountdown = false;
        bool  countdownActive = false;
        std::chrono::steady_clock::time_point countdownStart;
        const int COUNTDOWN_DURATION = 180;
        float timerStartTime = 0.0f;
        float countdownDuration = 180.0f;
    } time;

    struct Visuals {
        bool  ShowDead = false;
        bool  Enable = false;
        bool  Enemy = false;
        bool  EspInvisible = false;
        bool  Observe = false;
        bool  Watermark = false;
        bool  FPS = false;
        int   DistanceEsp = 130;
        bool  Glows = false;
        bool  RGB = false;
        bool  RainbowLines = false;
        bool  Lines = true;
        int   EspLines = 1;
        int   line_style = 0;
        bool  countdownx = true;
        float LinesColor[4] = { 1.f, 1.f, 1.f, 1.f };
        float KnockedColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
        bool  GlowLines = true;
        float GlowRadius = 0.10f;
        float GlowFeather = 0.10f;
        float LineThickness = 0.50f;
        bool  Box = true;
        int   players_box = 2;
        float BoxColor[4] = { 1.f, 1.f, 1.f, 1.f };
        bool  FilledBox = false;
        float Filledboxcolor[4] = { 0.f, 0.f, 0.f, 0.4f };
        bool  FillColorBox = false;
        float FillColor[4] = { 1.f, 1.f, 1.f, 0.2f };
        bool  ESPInfo = true;
        bool  HealthBar = true;
        int   players_healthbar = 1;
        float texthColor[4] = { 1.f, 1.f, 1.f, 1.f };
        bool  ShieldBar = false;
        int   players_shieldbar = 0;
        bool  ESPWeapon = false;
        bool  ESPWeaponIcon = true;
        float ESPWeaponColor[4] = { 1.f, 1.f, 1.f, 1.f };
        bool  esparmas = true;
        float GunColor[4] = { 1.f, 1.f, 1.f, 1.f };
        bool  showIcons = false;
        bool  Skeleton = true;
        float SkeletonColor[4] = { 1.f, 1.f, 1.f, 1.f };
        float SkeletonThickness = 0.5f;
        bool  Alvo = false;
        float AlvoColor[4] = { 1.f, 1.f, 1.f, 1.f };
        bool  AimPointESP = false;
        bool  Name = false;
        float NameColor[4] = { 1.f, 1.f, 1.f, 1.f };
        bool  Distance = true;
        float DistColor[4] = { 0.7f, 0.7f, 0.7f, 1.f };
        bool  ESPGranada = false;
        float ESPGranadaColor[4] = { 0.184f, 0.424f, 0.6f, 1.0f };
        bool  Wukong = false;
        bool  moco = false;
        bool  Prevision = false;
        bool  SnakeText = true;
        bool  RadarHack = false;
        bool  Radar = false;
        bool  minimap = false;
        float RadarPosX = 100.0f;
        float RadarPosY = 100.0f;
        bool  ShowArrow = false;
        float ArrowOffsetY = 7.0f;
        bool  ShowTopLineAndArrow = false;
        bool  ShowAvatar = false;
        bool  Level = false;
        bool  ID = false;
        bool  Pose = false;
        bool  HeadDot = false;
        float HeadDotSize = 1.11f;
        float HeadDotColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
        bool  g_UpActive = false;
        float g_OriginalY = 0.0f;
        float g_LiftedY = 0.0f;
        Player* g_ActiveEnemy = nullptr;
        float TextSize = 12.0f;
        float LogoScale = 1.0f;
        float Thickness = 1.0f;
        float WatermarkColor[4] = { 0.184f, 0.424f, 0.6f, 1.0f };
        float EnemyColor[4] = { 0.184f, 0.424f, 0.6f, 1.0f };
        bool  Debug = false;
        float x = 0.0f;
        float y = 0.0f;
        float LeftKneeOffset = -0.25f;
        float RightKneeOffset = -0.35f;
        float HipHeightOffset = 0.7f;
        float HipWidthOffset = 0.33f;
        float HipWidthScale = 0.19f;
        float LeftHipHeightOffset = 0.82f;
        float RightHipHeightOffset = 0.98f;
        float LeftKneeHeightOffset = 0.5f;
        float RightKneeHeightOffset = 0.5f;
        float KneeWidthScale = 0.5f;
        float LeftKneeWidthOffset = 0.1f;
        float RightKneeWidthOffset = 0.1f;
        float LeftKneeFlexionOffset = 0.1f;
        float RightKneeFlexionOffset = 0.1f;
    } Visuals;

    struct Chams {
        bool  Chams3D = false;
        bool  WhiteBody = false;
        bool  BlackBody = false;
        bool  ColorTexture = false;
        bool  Wireframe = false;
        bool  Solido = false;
        bool  Black = false;
        bool  Pick = false;
        bool  Outline_Enabled = false;
        int   ComboOutline = 0;
        bool  OutlineAnimated = false;
        bool  Outline = false;
        bool  Spiderman = false;
        bool  Peru = false;
        bool  Brazil = false;
        bool  Mexico = false;
        bool  Creeper = false;
        bool  Disco = false;
        bool  Lava = false;
        bool  Glitch = false;
        bool  Matrix = false;
        bool  LataGlitch = false;
        bool  TxGlitch = false;
        bool  TextureMap_Enabled = false;
        int   ComboMap = 0;
        bool  TextureHDRMap = false;
        bool  TextureShadowMap = false;
        bool  TextureWukong_Enabled = false;
        int   ComboWukong = 0;
        bool  TextureWukong = false;
        bool  TextureWukongEx = false;
        bool  TextureCombo_Enabled = false;
        bool  MultiComboTextures[2] = { false, false };
        bool  TextureOrion = false;
        bool  TextureSkins = false;
        int   countThreshold = 6000;
        float ThicknessOutline = 1.0f;
    } Chams;

    struct Exploits {
        bool DownLocalPlayer = false;
        int DownLocalPlayerBind = 0;
        bool DownPlayer = false;
        int DownPlayerBind = 0;
        bool EnemyPullEnabled = false;
        int EnemyPullKey = 0;
        float PullHeightOffset = 0.0f;
        bool SpinBot = false;
        int SpinBotBind = 0;
        int SpinSpeed = 180;
        bool TeleportMark = false;
        int TeleportMarkBind = 0;
        bool FlyHack = false;
        int FlyHackBind = 0;
    } Exploits;
};

inline Globals g_Globals;