#pragma once
#include <string>
#include <vector>

class ConfigManager {
public:
    void CreateConfig(std::string name);
    void SaveConfig(std::string name);
    void LoadConfig(std::string name);
    void ResetConfig();
    void OpenConfigFolder();
    void ExportToClipboard();
    void ImportFromClipboard();
    std::vector<std::string> GetConfigs();
};

extern ConfigManager* g_ConfigSystem;