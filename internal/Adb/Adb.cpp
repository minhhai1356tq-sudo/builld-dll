#include "adb.hpp"
#include "adb_utils.hpp"
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace ProcessUtils;

/*

if you see this file,
this adb is old and bad
i recommend you to change it to something more optimized
it works yeah but unefficient.


*/

namespace adb {

    std::string GetExecutableDirectory() {
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);

        std::string fullPath(path);
        size_t lastSlashIndex = fullPath.find_last_of("\\/");
        if (lastSlashIndex != std::string::npos) {
            return fullPath.substr(0, lastSlashIndex);
        }
        return "";
    }

    std::string GetExecutableDirectoryPID(DWORD processID) {
        char path[MAX_PATH];
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

        if (hProcess) {
            if (GetModuleFileNameExA(hProcess, NULL, path, MAX_PATH)) {
                std::string fullPath(path);
                size_t lastSlashIndex = fullPath.find_last_of("\\/");
                CloseHandle(hProcess);
                if (lastSlashIndex != std::string::npos) {
                    return fullPath.substr(0, lastSlashIndex);
                }
            }
            CloseHandle(hProcess);
        }

        return "";
    }

    bool ChangeDirectory(const std::string& path) {
        return SetCurrentDirectoryA(path.c_str());
    }

    void TerminateAdbProcesses() {
        DWORD ProcIdAdb = GetProc(L"adb.exe");
        DWORD ProcIdAdb2 = 0;
        KillProc(ProcIdAdb);
        if (IsProcRun(L"adb.exe", ProcIdAdb2)) {
            ForgeKillProc(ProcIdAdb2);
        }

        DWORD ProcIdAdbHD = GetProc(L"HD-Adb.exe");
        DWORD ProcIdAdbHD2 = 0;
        KillProc(ProcIdAdbHD);
        if (IsProcRun(L"HD-Adb.exe", ProcIdAdbHD2)) {
            ForgeKillProc(ProcIdAdbHD2);
        }
    }

    std::vector<DWORD> GetAllProcessIds() {
        std::vector<DWORD> pids(1024);
        DWORD bytesReturned;
        if (EnumProcesses(pids.data(), static_cast<DWORD>(pids.size() * sizeof(DWORD)), &bytesReturned)) {
            pids.resize(bytesReturned / sizeof(DWORD));
        }
        else {
            pids.clear();
        }
        return pids;
    }

    std::wstring GetProcessPath(DWORD pid) {
        wchar_t buffer[MAX_PATH] = { 0 };
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (hProcess) {
            HMODULE hMod;
            DWORD cbNeeded;
            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                GetModuleFileNameExW(hProcess, hMod, buffer, MAX_PATH);
            }
            CloseHandle(hProcess);
        }
        return std::wstring(buffer);
    }

    bool IsProcessRunning(DWORD pid) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
        if (hProcess == NULL) {
            return false; // El proceso ya no existe o no se puede acceder
        }

        DWORD exitCode;
        if (GetExitCodeProcess(hProcess, &exitCode)) {
            CloseHandle(hProcess);
            return (exitCode == STILL_ACTIVE);
        }

        CloseHandle(hProcess);
        return false;
    }

    void KillBlueStacksAdbProcesses() {
        std::vector<DWORD> pids = GetAllProcessIds();

        for (DWORD pid : pids) {
            std::wstring exePath = GetProcessPath(pid);
            std::transform(exePath.begin(), exePath.end(), exePath.begin(), ::towlower);

            // Lista de patrones para detectar BlueStacks (todas versiones)
            const std::vector<std::wstring> bluestacksPatterns = {
                L"BlueStacks_msi5",       // Todas las versiones
                L"bstk",             // Algunas versiones usan este prefijo
                L"hd-adb",           // ADB de BlueStacks
                L"BlueStacks_nxt",    // BlueStacks 5
                L"bluestack_x"       // BlueStacks X
            };

            // Lista de procesos ADB a cerrar
            const std::vector<std::wstring> adbProcesses = {
                L"adb.exe",
                L"hd-adb.exe",
                L"bstkadb.exe"
            };

            bool isBluestacks = false;
            for (const auto& pattern : bluestacksPatterns) {
                if (exePath.find(pattern) != std::wstring::npos) {
                    isBluestacks = true;
                    break;
                }
            }

            bool isAdbProcess = false;
            for (const auto& adbProc : adbProcesses) {
                if (exePath.find(adbProc) != std::wstring::npos) {
                    isAdbProcess = true;
                    break;
                }
            }

            if (isBluestacks && isAdbProcess) {
                KillProc(pid);


                if (IsProcessRunning(pid)) {
                    ForgeKillProc(pid);
                }
            }
        }


        DWORD adbPid = GetProc(L"adb.exe");
        if (adbPid != 0) {
            std::wstring path = GetProcessPath(adbPid);
            std::transform(path.begin(), path.end(), path.begin(), ::towlower);
            if (path.find(L"bluestacks") != std::wstring::npos) {
                KillProc(adbPid);
            }
        }

        DWORD hdAdbPid = GetProc(L"HD-Adb.exe");
        if (hdAdbPid != 0) {
            KillProc(hdAdbPid);
        }
    }

    std::string ExtractLibAddress(const std::string& input) {
        std::size_t pos = input.find('-');
        if (pos != std::string::npos && pos >= 8) {
            return input.substr(pos - 8, 8);
        }
        return "";
    }

    uintptr_t ConvertToUintPtr(std::string str, int base) {
        if (sizeof(uintptr_t) == sizeof(unsigned long)) {
            return strtoul(str.c_str(), nullptr, base);
        }
        return strtoull(str.c_str(), nullptr, base);
    }

    std::string ExecuteShellCommand(const std::string& firstCommand, const std::string& secondCommand) {
        HANDLE hStdOutRead, hStdOutWrite;
        HANDLE hStdInRead, hStdInWrite;

        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = TRUE;
        sa.lpSecurityDescriptor = NULL;

        if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) {
            std::cerr << "CreatePipe failed: " << GetLastError() << std::endl;
            return "";
        }

        if (!SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0)) {
            std::cerr << "SetHandleInformation failed: " << GetLastError() << std::endl;
            return "";
        }

        if (!CreatePipe(&hStdInRead, &hStdInWrite, &sa, 0)) {
            std::cerr << "CreatePipe failed: " << GetLastError() << std::endl;
            return "";
        }

        if (!SetHandleInformation(hStdInWrite, HANDLE_FLAG_INHERIT, 0)) {
            std::cerr << "SetHandleInformation failed: " << GetLastError() << std::endl;
            return "";
        }

        STARTUPINFOA si;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.hStdError = hStdOutWrite;
        si.hStdOutput = hStdOutWrite;
        si.hStdInput = hStdInRead;
        si.dwFlags |= STARTF_USESTDHANDLES;
        si.wShowWindow = SW_HIDE;

        PROCESS_INFORMATION pi;
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcessA(NULL, (LPSTR)".\\HD-Adb shell", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            std::cerr << "CreateProcess failed: " << GetLastError() << std::endl;
            return "";
        }

        CloseHandle(hStdOutWrite);
        CloseHandle(hStdInRead);

        DWORD written;
        std::string commands = firstCommand + "\n" + secondCommand + "\n";
        if (!WriteFile(hStdInWrite, commands.c_str(), commands.length(), &written, NULL)) {
            std::cerr << "WriteFile failed: " << GetLastError() << std::endl;
            return "";
        }
        CloseHandle(hStdInWrite);

        CHAR buffer[128];
        DWORD read;
        std::string output;
        while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &read, NULL) && read > 0) {
            buffer[read] = '\0';
            output += buffer;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(hStdOutRead);

        KillBlueStacksAdbProcesses();

        return ExtractLibAddress(output);
    }

    std::string ExecuteShellCommandNoSu(const std::string& command) {
        HANDLE hStdOutRead, hStdOutWrite;
        HANDLE hStdInRead, hStdInWrite;

        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = TRUE;
        sa.lpSecurityDescriptor = NULL;

        if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) {
            std::cerr << "CreatePipe failed: " << GetLastError() << std::endl;
            return "";
        }

        if (!SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0)) {
            std::cerr << "SetHandleInformation failed: " << GetLastError() << std::endl;
            return "";
        }

        if (!CreatePipe(&hStdInRead, &hStdInWrite, &sa, 0)) {
            std::cerr << "CreatePipe failed: " << GetLastError() << std::endl;
            return "";
        }

        if (!SetHandleInformation(hStdInWrite, HANDLE_FLAG_INHERIT, 0)) {
            std::cerr << "SetHandleInformation failed: " << GetLastError() << std::endl;
            return "";
        }

        STARTUPINFOA si;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.hStdError = hStdOutWrite;
        si.hStdOutput = hStdOutWrite;
        si.hStdInput = hStdInRead;
        si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;

        PROCESS_INFORMATION pi;
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcessA(NULL, (LPSTR)".\\HD-Adb shell \"getprop ro.secure ; /boot/android/android/system/xbin/bstk/su\"", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            std::cerr << "CreateProcess failed: " << GetLastError() << std::endl;
            return "";
        }

        CloseHandle(hStdOutWrite);
        CloseHandle(hStdInRead);

        DWORD written;
        std::string commands = command + "\n";
        if (!WriteFile(hStdInWrite, commands.c_str(), commands.length(), &written, NULL)) {
            std::cerr << "WriteFile failed: " << GetLastError() << std::endl;
            return "";
        }
        CloseHandle(hStdInWrite);

        CHAR buffer[128];
        DWORD read;
        std::string output;
        while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &read, NULL) && read > 0) {
            buffer[read] = '\0';
            output += buffer;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(hStdOutRead);

        KillBlueStacksAdbProcesses();

        return ExtractLibAddress(output);
    }

    void ExecuteADBCommand(const std::string& command) {
        std::string fullCommand = ".\\HD-Adb shell " + command;
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.wShowWindow = SW_HIDE;
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcessA(NULL, const_cast<LPSTR>(fullCommand.c_str()), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            std::cerr << "Failed to start process. Error: " << GetLastError() << std::endl;
            return;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        KillBlueStacksAdbProcesses();
    }

    void ExecuteADBRawCommand(const std::string& command) {
        std::string fullCommand = ".\\HD-Adb " + command;
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.wShowWindow = SW_HIDE;
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcessA(NULL, const_cast<LPSTR>(fullCommand.c_str()), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            std::cerr << "Failed to start process. Error: " << GetLastError() << std::endl;
            return;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        KillBlueStacksAdbProcesses();
    }
}