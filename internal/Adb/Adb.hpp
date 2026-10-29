#ifndef ADB_UTILS_HPP
#define ADB_UTILS_HPP

#include <Windows.h>
#include <iostream>
#include <Psapi.h>
#include <sstream>
#include <string>

namespace adb {
    std::string GetExecutableDirectory();
    std::string GetExecutableDirectoryPID(DWORD processID);
    bool ChangeDirectory(const std::string& path);
    void TerminateAdbProcesses();
    std::string ExtractLibAddress(const std::string& input);
    uintptr_t ConvertToUintPtr(std::string str, int base = 16);
    std::string ExecuteShellCommand(const std::string& firstCommand, const std::string& secondCommand);
    std::string ExecuteShellCommandNoSu(const std::string& command);
    void ExecuteADBCommand(const std::string& command);
    void ExecuteADBRawCommand(const std::string& command);
}

#endif