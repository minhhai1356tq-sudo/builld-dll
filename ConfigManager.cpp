#include "ConfigManager.hpp"

ConfigManager* g_ConfigSystem = nullptr;

void ConfigManager::CreateConfig(std::string name) {}
void ConfigManager::SaveConfig(std::string name) {}
void ConfigManager::LoadConfig(std::string name) {}
void ConfigManager::ResetConfig() {}
void ConfigManager::OpenConfigFolder() {}
void ConfigManager::ExportToClipboard() {}
void ConfigManager::ImportFromClipboard() {}

std::vector<std::string> ConfigManager::GetConfigs() {
    return {};
}