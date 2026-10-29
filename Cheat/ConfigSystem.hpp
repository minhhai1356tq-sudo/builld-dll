
#pragma once

#include <string>
#include <vector>
#include <set>

#include <FrameWork/FrameWork.hpp>
#include <FrameWork/Utilities/Base64.hpp>

#include "Options.hpp"

using json = nlohmann::json;

struct waypointsystem2
{
	std::string name;
	float x;
	float y;
	float z;
};

class ConfigManager
{
public:
	class C_ConfigItem
	{
	public:
		std::string name;
		void* pointer;
		std::string type;

		C_ConfigItem(std::string name, void* pointer, std::string type)  //-V818
		{
			this->name = name; //-V820
			this->pointer = pointer;
			this->type = type; //-V820
		}
	};

	void AddItem(void* pointer, const char* name, const std::string& type);
	void SetupItem(int*, int, const std::string&);
	void SetupItem(bool*, bool, const std::string&);
	void SetupItem(float*, float, const std::string&);
	void SetupItem(std::vector< int >*, int, const std::string&);
	void SetupItem(std::vector< uintptr_t >* pointer, int size, const std::string& name);
	void SetupItem(std::vector< float >*, int, const std::string&);
	void SetupItem(std::vector< std::string >*, const std::string&);
	void SetupItem(std::string*, const std::string&, const std::string&);

	std::vector <C_ConfigItem*> items;
	std::string CitizenFX;

	ConfigManager()
	{
		Setup();
	};

	void Setup();

	BOOL DirectoryExists(LPCTSTR szPath);
	std::vector<std::string> GetConfigs();
	void LoadConfig(std::string configname);
	void SaveConfig(std::string configname);
	void RemoveConfig(std::string configname);
	void CreateConfig(std::string configname);
	void ResetConfig();
	void OpenConfigFolder();
	void ExportToClipboard();
	void ImportFromClipboard();
};

extern ConfigManager* g_ConfigSystem;

extern std::unordered_map <std::string, float[4]> colors1;