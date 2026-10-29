//#include "ConfigSystem.hpp"
//#include <fstream>
//
//
//	std::unordered_map <std::string, float[4]> colors;
//
//	ConfigManager* g_ConfigSystem = new ConfigManager();
//
//	extern ImColor accentcolor;
//
//	void ConfigManager::Setup()
//	{
//		// Aimbot
//	
//		SetupItem(&vpx_opt.LegitBot.AimBot.Enabled, false, RGS("abt_enabled"));
//		SetupItem(&vpx_opt.LegitBot.AimBot.KeyBind, 0, RGS("abt_key"));
//		SetupItem(&vpx_opt.LegitBot.AimBot.TargetNPC, false, RGS("abt_targetnpc"));
//		SetupItem(&vpx_opt.LegitBot.AimBot.HitBox, 0, RGS("abt_hitbox"));
//		SetupItem(&vpx_opt.LegitBot.AimBot.MaxDistance, 250, RGS("abt_maxdistance"));
//		SetupItem(&vpx_opt.LegitBot.AimBot.FOV, 10, RGS("abt_fov"));
//		SetupItem(&vpx_opt.LegitBot.AimBot.SmoothVertical, 80, RGS("abt_smoothvertical"));
//		SetupItem(&vpx_opt.LegitBot.AimBot.SmoothHorizontal, 80, RGS("abt_smoothhorizontal"));
//		SetupItem(&vpx_opt.LegitBot.AimBot.VisibleCheck, false, RGS("VisibleCheck"));
//
//		// TriggerBot
//		SetupItem(&vpx_opt.LegitBot.Trigger.Enabled, false, RGS("trtg_enabled"));
//		SetupItem(&vpx_opt.LegitBot.Trigger.KeyBind, 0, RGS("trtg_key"));
//		SetupItem(&vpx_opt.LegitBot.Trigger.ShotNPC, false, RGS("trtg_shotnpc"));
//		SetupItem(&vpx_opt.LegitBot.Trigger.MaxDistance, 250, RGS("trtg_maxdistance"));
//		SetupItem(&vpx_opt.LegitBot.Trigger.ReactionTime, 0, RGS("trtg_reactiontime"));
//		SetupItem(&vpx_opt.LegitBot.Trigger.VisibleCheck, false, RGS("VisibleCheck_trigger"));
//
//		// SilentAim
//		SetupItem(&vpx_opt.LegitBot.SilentAim.Enabled, false, RGS("slt_enabled"));
//		SetupItem(&vpx_opt.LegitBot.SilentAim.Fov, 0, RGS("slt_fov"));
//		SetupItem(&vpx_opt.LegitBot.SilentAim.KeyBind, 0, RGS("slt_key"));
//		SetupItem(&vpx_opt.LegitBot.SilentAim.ShotNPC, false, RGS("slt_shotnpc"));
//		SetupItem(&vpx_opt.LegitBot.SilentAim.MaxDistance, 250, RGS("slt_maxdistance"));
//		SetupItem(&vpx_opt.LegitBot.SilentAim.HitBox, 0, RGS("slt_hitbox"));
//		SetupItem(&vpx_opt.LegitBot.SilentAim.VisibleCheck, false, RGS("VisibleCheck_silent"));
//		SetupItem(&vpx_opt.LegitBot.MagicBullet.Enabled, false, RGS("mgc_bullet"));
//
//		// MISC
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.norecoil, false, RGS("norecoil"));
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.nospread, false, RGS("nospread"));
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.noreload, false, RGS("noreload"));
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.SemiGod, false, RGS("SemiGod"));
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.God, false, RGS("God"));
//
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.Noclip, false, RGS("Noclip"));
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.NoclipBind, false, RGS("NoclipBind"));
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.NoclipSpeed, 100, RGS("NoclipSpeed"));
//		//
//
//		// Esp-Players
//		SetupItem(&vpx_opt.Visuals.ESP.Players.Enabled, true, RGS("esp_players_enabled"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.ShowLocalPlayer, false, RGS("esp_players_localplayer"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.ShowNPCs, false, RGS("esp_players_npscs"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.RenderDistance, 300, RGS("esp_players_renderdist"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.Box, false, RGS("esp_players_box"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.Skeleton, true, RGS("esp_players_skel"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.Name, true, RGS("esp_players_name"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.HealthBar, 0, RGS("esp_players_healthbar"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.ArmorBar, 0, RGS("esp_players_armorbar"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.WeaponName, 0, RGS("esp_players_weapname"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.Distance, false, RGS("esp_players_distance"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.SnapLines, false, RGS("esp_players_snampli"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.Head, false, RGS("esp_players_head"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.FilledBox, false, RGS("esp_players_filledbox"));
//
//		// Some ESP-pLAYER-cOLOR
//		SetupItem(&vpx_opt.Visuals.ESP.Players.SkeletonColor[0], 1.f, RGS("esp_players_skelco0"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.SkeletonColor[1], 1.f, RGS("esp_players_skelco1"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.SkeletonColor[2], 1.f, RGS("esp_players_skelco2"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.SkeletonColor[3], 1.f, RGS("esp_players_skelco3"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.NameColor[0], 1.f, RGS("esp_players_NAMEco0"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.NameColor[1], 1.f, RGS("esp_players_NAMEco1"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.NameColor[2], 1.f, RGS("esp_players_NAMEco2"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.NameColor[3], 1.f, RGS("esp_players_NAMEco3"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.WeaponNameColor[0], 1.f, RGS("esp_players_weaponNAMEco0"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.WeaponNameColor[1], 1.f, RGS("esp_players_weaponNAMEco1"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.WeaponNameColor[2], 1.f, RGS("esp_players_weaponNAMEco2"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.WeaponNameColor[3], 1.f, RGS("esp_players_weaponNAMEco3"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.DistanceColor[0], 1.f, RGS("esp_players_distanceco0"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.DistanceColor[1], 1.f, RGS("esp_players_distanceco1"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.DistanceColor[2], 1.f, RGS("esp_players_distanceco2"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.DistanceColor[3], 1.f, RGS("esp_players_distanceco3"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.SnaplinesColor[0], 1.f, RGS("esp_players_snaplinesco0"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.SnaplinesColor[1], 1.f, RGS("esp_players_snaplinesco1"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.SnaplinesColor[2], 1.f, RGS("esp_players_snaplinesco2"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.SnaplinesColor[3], 1.f, RGS("esp_players_snaplinesco3"));
//
//		SetupItem(&vpx_opt.Visuals.ESP.Players.FilledBoxColor[4], 1.f, RGS("esp_players_filledboxco"));
//		SetupItem(&vpx_opt.Visuals.ESP.Players.HeadColor[4], 1.f, RGS("esp_players_headco"));
//
//		// Esp-Vehicles
//		SetupItem(&vpx_opt.Visuals.ESP.Vehicles.Enabled, false, RGS("esp_vehicles_enabled"));
//		SetupItem(&vpx_opt.Visuals.ESP.Vehicles.Color[0], 1.f, RGS("esp_vehicles_col0"));
//		SetupItem(&vpx_opt.Visuals.ESP.Vehicles.Color[1], 1.f, RGS("esp_vehicles_col1"));
//		SetupItem(&vpx_opt.Visuals.ESP.Vehicles.Color[2], 1.f, RGS("esp_vehicles_col2"));
//		SetupItem(&vpx_opt.Visuals.ESP.Vehicles.Color[3], 1.f, RGS("esp_vehicles_col3"));
//		SetupItem(&vpx_opt.Visuals.ESP.Vehicles.Name, false, RGS("esp_vehicles_name"));
//		SetupItem(&vpx_opt.Visuals.ESP.Vehicles.Distance, false, RGS("esp_vehicles_distance"));
//		SetupItem(&vpx_opt.Visuals.ESP.Vehicles.Marker, false, RGS("esp_vehicles_marker"));
//		SetupItem(&vpx_opt.Visuals.ESP.Vehicles.IgnoreOccupiedVehicles, false, RGS("esp_vehicles_ignoreoccupied"));
//
//		// Screen
//		SetupItem(&vpx_opt.Misc.Screen.ShowAimbotFov, 0, RGS("misc_localplayer_showaimbotfov"));
//		SetupItem(&vpx_opt.Misc.Screen.ShowSilentAimFov, 0, RGS("misc_localplayer_showsilentfov"));
//
//
//		// self nigger
//
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.infiniteroll, false, RGS("misc_localplayer_infroll"));
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.antihs, false, RGS("misc_localplayer_antihs"));
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.superjump, false, RGS("misc_localplayer_superjump"));
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.nocollision, false, RGS("misc_localplayer_nocollision"));
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.noragdoll, false, RGS("misc_localplayer_noragdoll"));
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.shrink, false, RGS("misc_localplayer_shrink"));
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.infinitestamina, false, RGS("misc_localplayer_infinitestamina"));
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.speed, false, RGS("misc_localplayer_speedwalk"));
//		SetupItem(&vpx_opt.Misc.Exploits.LocalPlayer.speed_value, 1.f, RGS("misc_localplayer_speedwalkmultiplier"));
//		SetupItem(&vpx_opt.Misc.Exploits.Weapon.customrecoil, false, RGS("misc_localplayer_customrecoil"));
//		SetupItem(&vpx_opt.Misc.Exploits.Weapon.customrecoilvalue, 1.f, RGS("misc_localplayer_customrecoilvalue"));
//
//		SetupItem(&vpx_opt.Misc.Exploits.Weapon.customspread, false, RGS("misc_localplayer_customspread"));
//		SetupItem(&vpx_opt.Misc.Exploits.Weapon.customspreadvalue, 1.f, RGS("misc_localplayer_customspreadvalue"));
//		SetupItem(&vpx_opt.Misc.Exploits.Weapon.fastreload,false, RGS("misc_localplayer_fastreload"));
//	
//		// vehicle selff
//		SetupItem(&vpx_opt.Misc.Exploits.m_Vehicle.b_Vehicle, false, RGS("misc_selfveh_enabled"));
//		SetupItem(&vpx_opt.Misc.Exploits.m_Vehicle.v_Boost, 1, RGS("misc_selfveh_boost"));
//		SetupItem(&vpx_opt.Misc.Exploits.m_Vehicle.v_brake, 1, RGS("misc_selfveh_brake"));
//		SetupItem(&vpx_opt.Misc.Exploits.m_Vehicle.v_Traction, 1, RGS("misc_selfveh_traction"));
//
//		// Misc-LocalVeh
//		SetupItem(&vpx_opt.Misc.FriendColor[0], 1.f, RGS("esp_FriendsColor0"));
//		SetupItem(&vpx_opt.Misc.FriendColor[1], 1.f, RGS("esp_FriendsColor1"));
//		SetupItem(&vpx_opt.Misc.FriendColor[2], 1.f, RGS("esp_FriendsColor2"));
//		SetupItem(&vpx_opt.Misc.FriendColor[3], 1.f, RGS("esp_FriendsColor3"));
//
//		SetupItem(&vpx_opt.General.vpxdelay, 0, RGS("thd_delay"));
//		SetupItem(&vpx_opt.General.CaptureBypass, true, RGS("captbypss"));
//		SetupItem(&vpx_opt.General.MenuKey, '4', RGS("mnkey"));
//	}
//#include <Shlobj.h> 
//#include <filesystem> 
//#include <fstream>
//#include <string>
//#include <set>
//
//#include <Windows.h>
//#include <Utils/LazyImporter.hpp>
//
//
//	void ConfigManager::AddItem(void* pointer, const char* name, const std::string& type)
//	{
//		items.push_back(new C_ConfigItem(std::string(name), pointer, type));
//	}
//
//	void ConfigManager::SetupItem(int* pointer, int value, const std::string& name)
//	{
//		AddItem(pointer, name.c_str(), XorStr("int"));
//		*pointer = value;
//	}
//
//	void ConfigManager::SetupItem(bool* pointer, bool value, const std::string& name)
//	{
//		AddItem(pointer, name.c_str(), XorStr("bool"));
//		*pointer = value;
//	}
//
//	void ConfigManager::SetupItem(float* pointer, float value, const std::string& name)
//	{
//		AddItem(pointer, name.c_str(), XorStr("float"));
//		
//	}
//
//	void ConfigManager::SetupItem(std::vector< int >* pointer, int size, const std::string& name)
//	{
//		AddItem(pointer, name.c_str(), XorStr("vector<int>"));
//		pointer->clear();
//
//		for (int i = 0; i < size; i++)
//			pointer->push_back(FALSE);
//	}
//
//	void ConfigManager::SetupItem(std::vector< uintptr_t >* pointer, int size, const std::string& name)
//	{
//		AddItem(pointer, name.c_str(), XorStr("vector<uintptr_t>"));
//		pointer->clear();
//
//		for (int i = 0; i < size; i++)
//			pointer->push_back(FALSE);
//	}
//
//	void ConfigManager::SetupItem(std::vector< float >* pointer, int size, const std::string& name)
//	{
//		AddItem(pointer, name.c_str(), XorStr("vector<float>"));
//		pointer->clear();
//
//		for (int i = 0; i < size; i++)
//			pointer->push_back(FALSE);
//	}
//
//	void ConfigManager::SetupItem(std::vector< std::string >* pointer, const std::string& name)
//	{
//		AddItem(pointer, name.c_str(), XorStr("vector<string>"));
//	}
//
//	void ConfigManager::SetupItem(std::string* pointer, const std::string& value, const std::string& name)
//	{
//		AddItem(pointer, name.c_str(), XorStr("string"));
//		*pointer = value;
//	}
//
//	BOOL ConfigManager::DirectoryExists(LPCTSTR szPath) {
//		DWORD dwAttrib = GetFileAttributes(szPath);
//
//		return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
//			(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
//	}
//
//
//	std::vector<std::string> ConfigManager::GetConfigs() {
//		static bool created_cfg = true;
//		if (created_cfg) {
//			
//			char path[MAX_PATH];
//			if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path))) {
//				CitizenFX = std::string(path) + "\\discord\\";
//				CreateDirectoryA(CitizenFX.c_str(), nullptr);
//			}
//			created_cfg = false;
//		}
//
//		std::string config_extension = XorStr(".cfg");
//		std::vector<std::string> names;
//
//		WIN32_FIND_DATAA find_data;
//		HANDLE preset_file = FindFirstFileA((CitizenFX + XorStr("*") + config_extension).c_str(), &find_data);
//
//		if (preset_file != INVALID_HANDLE_VALUE) {
//			do {
//				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//					continue;
//
//				std::string s = find_data.cFileName;
//
//			
//				int pos = s.find(XorStr(".cfg"));
//
//				// NOT FOUND
//				if (pos == -1)
//					continue;
//
//				s.erase(s.begin() + pos, s.begin() + pos + 5);
//
//				names.push_back(s.c_str());
//			} while (FindNextFileA(preset_file, &find_data));
//
//			FindClose(preset_file);
//		}
//
//		return names;
//	}
//
//
//	
//
//	void ConfigManager::LoadConfig(std::string configname) {
//		std::ifstream input_file = std::ifstream((CitizenFX + configname + XorStr(".cfg")).c_str());
//		if (!input_file.good())
//			return;
//
//		static auto find_item = [](std::vector< C_ConfigItem* > items, std::string name) -> C_ConfigItem*
//			{
//				for (int i = 0; i < (int)items.size(); i++)
//					if (!items[i]->name.compare(name))
//						return items[i];
//
//				return nullptr;
//			};
//
//		try {
//			std::stringstream content{ };
//
//			content << input_file.rdbuf();
//
//			if (content.str().empty())
//				return;
//
//			auto decoded_string = base64::decode(content.str());
//			// this a vpx config?
//			if (decoded_string[0] != '[' ||
//				decoded_string[1] != 'v' ||
//				decoded_string[2] != 'p' ||
//				decoded_string[3] != 'x' ||
//				decoded_string[4] != ']' ||
//				decoded_string[5] != ' ' ||
//				decoded_string[6] != '-' ||
//				decoded_string[7] != ' ')
//				return;
//
//			// yes it is
//			auto parsed_config = nlohmann::json::parse(decoded_string.erase(0, 8));
//
//			json allJson = parsed_config;
//
//			for (auto it = allJson.begin(); it != allJson.end(); ++it)
//			{
//				json j = *it;
//
//				std::string name = j[XorStr("name")];
//				std::string type = j[XorStr("type")];
//
//				auto item = find_item(items, name);
//
//				if (item)
//				{
//					if (!type.compare(XorStr("int")))
//						*(int*)item->pointer = j[XorStr("value")].get<int>();
//					else if (!type.compare(XorStr("float")))
//						*(float*)item->pointer = j[XorStr("value")].get<float>();
//					else if (!type.compare(XorStr("bool")))
//						*(bool*)item->pointer = j[XorStr("value")].get<bool>();
//					else if (!type.compare(XorStr("vector<int>")))
//					{
//						auto ptr = static_cast<std::vector <int>*> (item->pointer);
//						ptr->clear();
//
//						json ja = json::parse(j[XorStr("value")].get<std::string>().c_str());
//
//						for (json::iterator it = ja.begin(); it != ja.end(); ++it)
//							ptr->push_back(*it);
//					}
//					else if (!type.compare(XorStr("vector<uintptr_t>")))
//					{
//						auto ptr = static_cast<std::vector <uintptr_t>*> (item->pointer);
//						ptr->clear();
//
//						json ja = json::parse(j[XorStr("value")].get<std::string>().c_str());
//
//						for (json::iterator it = ja.begin(); it != ja.end(); ++it)
//							ptr->push_back(*it);
//					}
//					else if (!type.compare(XorStr("vector<float>")))
//					{
//						auto ptr = static_cast<std::vector <float>*> (item->pointer);
//						ptr->clear();
//
//						json ja = json::parse(j[XorStr("value")].get<std::string>().c_str());
//
//						for (json::iterator it = ja.begin(); it != ja.end(); ++it)
//							ptr->push_back(*it);
//					}
//					else if (!type.compare(XorStr("vector<string>")))
//					{
//						auto ptr = static_cast<std::vector <std::string>*> (item->pointer);
//						ptr->clear();
//
//						json ja = json::parse(j[XorStr("value")].get<std::string>().c_str());
//
//						for (json::iterator it = ja.begin(); it != ja.end(); ++it)
//							ptr->push_back(*it);
//					}
//					else if (!type.compare(XorStr("vector<waypoint>")))
//					{
//						json ja = json::parse(j[XorStr("value")].get<std::string>().c_str());
//						json ja2 = j[XorStr("value_x")].get<float>();
//						json ja3 = j[XorStr("value_y")].get<float>();
//						json ja4 = j[XorStr("value_z")].get<float>();
//
//					}
//					else if (!type.compare(XorStr("string")))
//						*(std::string*)item->pointer = j[XorStr("value")].get<std::string>();
//				}
//			}
//
//			input_file.close();
//		}
//		catch (...) {
//			input_file.close();
//			return;
//		}
//	}
//
//	void ConfigManager::SaveConfig(std::string configname) {
//		std::ofstream o(CitizenFX + configname + XorStr(".cfg"));
//		if (!o.is_open())
//			return;
//
//		json allJson;
//		std::set<std::string> seenItems;
//
//		for (auto it : items)
//		{
//			// Check if this item has already been added
//			if (seenItems.count(it->name) > 0) {
//				continue;
//			}
//
//			json j;
//
//			j[XorStr("name")] = it->name;
//			j[XorStr("type")] = it->type;
//
//			if (!it->type.compare(XorStr("int")))
//				j[XorStr("value")] = (int)*(int*)it->pointer;
//			else if (!it->type.compare(XorStr("float")))
//				j[XorStr("value")] = (float)*(float*)it->pointer;
//			else if (!it->type.compare(XorStr("bool")))
//				j[XorStr("value")] = (bool)*(bool*)it->pointer;
//			else if (!it->type.compare(XorStr("vector<int>")))
//			{
//				auto& ptr = *(std::vector<int>*)(it->pointer);
//				json ja;
//
//				for (auto& i : ptr)
//					ja.push_back(i);
//
//				j[XorStr("value")] = ja.dump();
//			}
//			else if (!it->type.compare(XorStr("vector<uintptr_t>")))
//			{
//				auto& ptr = *(std::vector<uintptr_t>*)(it->pointer);
//				json ja;
//
//				for (auto& i : ptr)
//					ja.push_back(i);
//
//				j[XorStr("value")] = ja.dump();
//			}
//			else if (!it->type.compare(XorStr("vector<float>")))
//			{
//				auto& ptr = *(std::vector<float>*)(it->pointer);
//				json ja;
//
//				for (auto& i : ptr)
//					ja.push_back(i);
//
//				j[XorStr("value")] = ja.dump();
//			}
//			else if (!it->type.compare(XorStr("vector<string>")))
//			{
//				auto& ptr = *(std::vector<std::string>*)(it->pointer);
//				json ja;
//
//				for (auto& i : ptr)
//					ja.push_back(i);
//
//				j[XorStr("value")] = ja.dump();
//			}
//			else if (!it->type.compare(XorStr("vector<rWaypointSystem>")))
//			{
//
//
//			}
//			else if (!it->type.compare(XorStr("string")))
//				j[XorStr("value")] = (std::string) * (std::string*)it->pointer;
//
//			allJson.push_back(j);
//			seenItems.insert(it->name);
//		}
//
//		o.clear();
//		auto str = base64::encode(std::string(XorStr("[vpx] - ")).append(allJson.dump(-1, '~'/*, true*/)));
//		o << str;
//		o.close();
//	}
//
//	void ConfigManager::RemoveConfig(std::string configname) {
//		std::remove((CitizenFX + configname + XorStr(".cfg")).c_str());
//	}
//
//	void ConfigManager::CreateConfig(std::string configname) {
//		std::ifstream input_file = std::ifstream((CitizenFX + configname + XorStr(".cfg")).c_str());
//		if (input_file.good())
//			return;
//
//		std::ofstream o((CitizenFX + configname + XorStr(".cfg")).c_str());
//		SaveConfig(configname);
//	}
//
//	void ConfigManager::ResetConfig() {
//		//for ( auto &child : g_Vars.m_children ) {
//		//	child->Load( g_Vars.m_json_default_cfg [ std::to_string( child->GetName( ) ) ] );
//		//}
//	}
//
//	void ConfigManager::OpenConfigFolder() {
//		namespace fs = std::experimental::filesystem;
//
//		std::wstring str = std::wstring(CitizenFX.begin(), CitizenFX.end());
//
//		PIDLIST_ABSOLUTE pidl;
//		if (SUCCEEDED(SHParseDisplayName(str.c_str(), 0, &pidl, 0, 0))) {
//			// we don't want to actually select anything in the folder, so we pass an empty
//			// PIDL in the array. if you want to select one or more items in the opened
//			// folder you'd need to build the PIDL array appropriately
//			ITEMIDLIST idNull = { 0 };
//			LPCITEMIDLIST pidlNull[1] = { &idNull };
//			LI_FN(SHOpenFolderAndSelectItems)(pidl, 1, pidlNull, 0);
//
//			// LIFEEEEHAAAACK BITCH!!! (◣_◢)
//		
//		}
//	}
//
//
//	void ConfigManager::ExportToClipboard()
//	{
//		static auto CopyToClipboard = [](const std::string& str) {
//			LI_FN(OpenClipboard)(nullptr);
//			LI_FN(EmptyClipboard)();
//
//			void* hg = LI_FN(GlobalAlloc)(GMEM_MOVEABLE, str.size() + 1);
//
//			if (!hg) {
//				LI_FN(CloseClipboard)();
//				return;
//			}
//
//			memcpy(LI_FN(GlobalLock)(hg), str.c_str(), str.size() + 1);
//			LI_FN(GlobalUnlock)(hg);
//			LI_FN(SetClipboardData)(CF_TEXT, hg);
//			LI_FN(CloseClipboard)();
//			LI_FN(GlobalFree)(hg);
//			};
//
//		json allJson;
//		std::set<std::string> seenItems;
//
//		for (auto it : items)
//		{
//			// Check if this item has already been added
//			if (seenItems.count(it->name) > 0) {
//				continue;
//			}
//
//			json j;
//
//			j[XorStr("name")] = it->name;
//			j[XorStr("type")] = it->type;
//
//			if (!it->type.compare(XorStr("int")))
//				j[XorStr("value")] = (int)*(int*)it->pointer;
//			else if (!it->type.compare(XorStr("float")))
//				j[XorStr("value")] = (float)*(float*)it->pointer;
//			else if (!it->type.compare(XorStr("bool")))
//				j[XorStr("value")] = (bool)*(bool*)it->pointer;
//			else if (!it->type.compare(XorStr("vector<int>")))
//			{
//				auto& ptr = *(std::vector<int>*)(it->pointer);
//				json ja;
//
//				for (auto& i : ptr)
//					ja.push_back(i);
//
//				j[XorStr("value")] = ja.dump();
//			}
//			else if (!it->type.compare(XorStr("vector<uintptr_t>")))
//			{
//				auto& ptr = *(std::vector<uintptr_t>*)(it->pointer);
//				json ja;
//
//				for (auto& i : ptr)
//					ja.push_back(i);
//
//				j[XorStr("value")] = ja.dump();
//			}
//			else if (!it->type.compare(XorStr("vector<float>")))
//			{
//				auto& ptr = *(std::vector<float>*)(it->pointer);
//				json ja;
//
//				for (auto& i : ptr)
//					ja.push_back(i);
//
//				j[XorStr("value")] = ja.dump();
//			}
//			else if (!it->type.compare(XorStr("vector<string>")))
//			{
//				auto& ptr = *(std::vector<std::string>*)(it->pointer);
//				json ja;
//
//				for (auto& i : ptr)
//					ja.push_back(i);
//
//				j[XorStr("value")] = ja.dump();
//			}
//			else if (!it->type.compare(XorStr("vector<rWaypointSystem>")))
//			{
//
//
//			}
//			else if (!it->type.compare(XorStr("string")))
//				j[XorStr("value")] = (std::string) * (std::string*)it->pointer;
//
//			allJson.push_back(j);
//			seenItems.insert(it->name);
//		}
//
//		auto str = base64::encode((std::string(XorStr("[vpx] - ")).append(allJson.dump(-1, '~'/*, true*/))).c_str());
//
//		CopyToClipboard(str);
//	}
//
//	void ConfigManager::ImportFromClipboard()
//	{
//		static auto GetClipBoardText = []()
//			{
//				LI_FN(OpenClipboard)(nullptr);
//
//				void* data = LI_FN(GetClipboardData)(CF_TEXT);
//				char* text = static_cast<char*>(LI_FN(GlobalLock)(data));
//
//				std::string str_text(text);
//
//				LI_FN(GlobalUnlock)(data);
//				LI_FN(CloseClipboard)();
//
//				return str_text;
//			};
//
//		static auto find_item = [](std::vector< C_ConfigItem* > items, std::string name) -> C_ConfigItem*
//			{
//				for (int i = 0; i < (int)items.size(); i++)
//					if (!items[i]->name.compare(name))
//						return items[i];
//
//				return nullptr;
//			};
//
//		if (GetClipBoardText().empty()) {
//			return;
//		}
//
//		auto decoded_string = base64::decode(GetClipBoardText());
//		// this a vpx config?
//		if (decoded_string[0] != '[' ||
//			decoded_string[1] != 'v' ||
//			decoded_string[2] != 'p' ||
//			decoded_string[3] != 'x' ||
//			decoded_string[4] != ']' ||
//			decoded_string[5] != ' ' ||
//			decoded_string[6] != '-' ||
//			decoded_string[7] != ' ')
//			return;
//
//		// yes it is
//		auto parsed_config = nlohmann::json::parse(decoded_string.erase(0, 8));
//
//		json allJson = parsed_config;
//
//		for (auto it = allJson.begin(); it != allJson.end(); ++it)
//		{
//			json j = *it;
//
//			std::string name = j[XorStr("name")];
//			std::string type = j[XorStr("type")];
//
//			auto item = find_item(items, name);
//
//			if (item)
//			{
//				if (!type.compare(XorStr("int")))
//					*(int*)item->pointer = j[XorStr("value")].get<int>();
//				else if (!type.compare(XorStr("float")))
//					*(float*)item->pointer = j[XorStr("value")].get<float>();
//				else if (!type.compare(XorStr("bool")))
//					*(bool*)item->pointer = j[XorStr("value")].get<bool>();
//				else if (!type.compare(XorStr("vector<int>")))
//				{
//					auto ptr = static_cast<std::vector <int>*> (item->pointer);
//					ptr->clear();
//
//					json ja = json::parse(j[XorStr("value")].get<std::string>().c_str());
//
//					for (json::iterator it = ja.begin(); it != ja.end(); ++it)
//						ptr->push_back(*it);
//				}
//				else if (!type.compare(XorStr("vector<uintptr_t>")))
//				{
//					auto ptr = static_cast<std::vector <uintptr_t>*> (item->pointer);
//					ptr->clear();
//
//					json ja = json::parse(j[XorStr("value")].get<std::string>().c_str());
//
//					for (json::iterator it = ja.begin(); it != ja.end(); ++it)
//						ptr->push_back(*it);
//				}
//				else if (!type.compare(XorStr("vector<float>")))
//				{
//					auto ptr = static_cast<std::vector <float>*> (item->pointer);
//					ptr->clear();
//
//					json ja = json::parse(j[XorStr("value")].get<std::string>().c_str());
//
//					for (json::iterator it = ja.begin(); it != ja.end(); ++it)
//						ptr->push_back(*it);
//				}
//				else if (!type.compare(XorStr("vector<string>")))
//				{
//					auto ptr = static_cast<std::vector <std::string>*> (item->pointer);
//					ptr->clear();
//
//					json ja = json::parse(j[XorStr("value")].get<std::string>().c_str());
//
//					for (json::iterator it = ja.begin(); it != ja.end(); ++it)
//						ptr->push_back(*it);
//				}
//				else if (!type.compare(XorStr("vector<waypoint>")))
//				{
//
//				}
//				else if (!type.compare(XorStr("string")))
//					*(std::string*)item->pointer = j[XorStr("value")].get<std::string>();
//			}
//		}
//	}