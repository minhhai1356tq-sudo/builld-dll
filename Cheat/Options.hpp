#pragma once
#include <cmath>  // For sin/cos functions for camera movement
#include "InputHandler.h"

namespace Cheat
{	

	class Options
	{
	public:
		struct LegitBot

		{
			struct Health
			{
			
				int KeyBind = 0;

			}Health;
			struct AimBot
			{
				bool Enabled;
				int KeyBind = 0;
				bool TargetNPC;
				bool VisibleCheck;
				int HitBox = 0;
				int MaxDistance = 600;
				int FOV = 10;
				int SmoothHorizontal = 80;
				int SmoothVertical = 80;
			}AimBot;
			struct TriggerBot
			{

				bool Enabled;
				int KeyBind = 0;
				bool ShotNPC;
				bool VisibleCheck;
				int MaxDistance = 600;
				int ReactionTime;
				void RunThread();
			}Trigger;

			struct MagicBullet
			{
				bool Enabled;
			}MagicBullet;

			struct SilentAim
			{
				bool Enabled;
				int Fov;
				int KeyBind = 0;
				bool ShotNPC;
				bool VisibleCheck;
				int MaxDistance = 600;
				int HitBox;
				float MissChance;

			}SilentAim;
		}LegitBot;
		enum AlignmentOption {
			LEFT,
			CENTER,
			RIGHT
		};
		struct Visuals
		{
			struct ESP
			{
				struct Players
				{
					bool Head = false;
			

					bool Enabled = true;
					bool ShowLocalPlayer = false;
					bool ShowNPCs;
					bool VisibleOnly = false;
					int RenderDistance = 600;
					bool Box;
					bool FilledBox = false;
					bool Skeleton = true;
					float SkeletonColor[4] = {1.f, 1.f, 1.f, 1.f };
					float FilledBoxColor[4] = {1.f, 1.f, 1.f, 1.f};
					float HeadColor[4] = {1.f, 1.f, 1.f, 1.f };
					bool Name = true;
					float NameColor[4] = {1.f, 1.f, 1.f, 1.f };
					//bool HealthBar = true;
					int HealthBar = 0;
					int ArmorBar = 0;
					bool WeaponName;
					bool Weapon_Misc = true;
					float WeaponNameColor[4] = {1.f, 1.f, 1.f, 1.f };
					bool Distance;
					float DistanceColor[4] = {1.f, 1.f, 1.f, 1.f };
					bool SnapLines;
					float SnaplinesColor[4] = {1.f, 1.f, 1.f, 1.f };
					

				
					AlignmentOption WeaponNameAlignment = CENTER;
				} Players;

				struct Vehicles
				{
					bool Enabled;
					bool IgnoreOccupiedVehicles;
					bool Marker;
					bool Distance;
					bool Name;
					float Color[4] = {1.f, 1.f, 1.f, 1.f };
				} Vehicles;
			} ESP;
		} Visuals;

	
		struct Misc
		{
	
			struct Screen
			{
				bool ShowAimbotFov;
				bool ShowSilentAimFov;
			}Screen;
			struct Exploits
			{
				

				struct LocalPlayer
				{

					//new
				
					bool InvisibleNC = false;
					bool invisible;
					
				
					bool shrink;
					bool superjump;
					bool infinitestamina;
					bool antihs;
					bool nocollision;
					bool noragdoll;
					bool speed;
					float speed_value = 1.0f;
					bool custom_fov;
					float fov_value;
					int godbind = 0;
					int unlockbind = 0;
					
					
				
					bool usegodmodebind;
					int KeyBind = 0;
					//new
					
					bool infiniteroll = false;
					bool Noclip;
					int NoclipBind;
					int NoclipSpeed;

					bool SemiGod;

					bool norecoil;
					bool nospread;
					bool noreload;
					int health_ammount;

					bool Start_Health;

					bool God;
				}LocalPlayer;
				struct Weapon
				{
					bool infiniteammo;
					bool customspread;
					bool customrecoil;
					bool fastreload;
					float customspreadvalue;
					float customrecoilvalue;
					bool change_ammo = false;
					int selected_explosion = 1;

				}Weapon;
				struct Vehicle
				{
					bool b_Vehicle;
					float v_brake = 1.f;
					float v_Boost = 1.f;
					float v_Traction = 1.f;
				} m_Vehicle;
			}Exploits;
			float FriendColor[4] = { 0.f, 0.f, 1.f, 1.f };
		}Misc;
		struct General
		{
			bool ShutDown = false;
			int PanicKey = 0;
			int MenuKey = '4';
			bool CaptureBypass = true;
			int vpxdelay = 5;
		}General;
	};
}

inline Cheat::Options vpx_opt;