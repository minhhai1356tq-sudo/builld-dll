#pragma once

#include <Windows.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

class MemoryUtils {
public:
    inline static void* vmPtr = nullptr;
    inline static void* pVMAddr = nullptr;
    inline static void* cpuAddr = nullptr;

    inline static std::unordered_map<uintptr_t, uintptr_t> Cache;

    using PGMPhysReadFunc = int(__cdecl*)(void*, uintptr_t, void*, size_t);
    using VMMGetCpuByIdFunc = void* (__cdecl*)(void*, int);
    using PGMPhysGCPtr2GCPhysFunc = int(__cdecl*)(void*, uintptr_t, uintptr_t*);
    using PGMPhysSimpleWriteGCPhysFunc = int(__cdecl*)(void*, uintptr_t, void*, size_t);

    inline static PGMPhysReadFunc               ogPhysRead = nullptr;
    inline static VMMGetCpuByIdFunc             ogCPU = nullptr;
    inline static PGMPhysGCPtr2GCPhysFunc       ogCast = nullptr;
    inline static PGMPhysSimpleWriteGCPhysFunc  ogWrite = nullptr;

    static int __cdecl HookedPGMPhysRead(void* pVM, uintptr_t GCPhys, void* pvBuf, size_t cbRead) {
        if (!vmPtr) {
            vmPtr = pVM;
            std::cout << "Memory initialized: " << vmPtr << std::endl;
        }
        return ogPhysRead(pVM, GCPhys, pvBuf, cbRead);
    }

    static int HookWrite(void* pVM, uintptr_t GCPhys, void* pvBuf, size_t cbRead) {
        return ogWrite(pVM, GCPhys, pvBuf, cbRead);
    }

    static int HookRead(void* pVM, uintptr_t GCPhys, void* pvBuf, size_t cbRead) {
        return ogPhysRead(pVM, GCPhys, pvBuf, cbRead);
    }

    static void* CPU(void* pVM, int cpuId) {
        return ogCPU(pVM, cpuId);
    }

    static int Cast(void* pVCpu, uintptr_t address, uintptr_t* physAddress) {
        return ogCast(pVCpu, address, physAddress);
    }

    static void Initialize(void* pVM) {
        pVMAddr = pVM;
        cpuAddr = CPU(pVM, 0);
        Cache.clear();
    }

    static constexpr uint32_t  MAX_CPU = 4U;
    static constexpr uintptr_t MIN_VALID_ADDRESS = 0x1000;

    static bool Convert(uintptr_t address, uintptr_t& phys) {
        phys = 0;
        if (address <= MIN_VALID_ADDRESS) return false;

        auto it = Cache.find(address);
        if (it != Cache.end() && it->second != 0) {
            phys = it->second;
            return true;
        }

        for (uint32_t i = 0; i < MAX_CPU; ++i) {
            void* cpu = CPU(pVMAddr, i);
            if (!cpu) continue;

            uintptr_t tempPhys = 0;
            if (Cast(cpu, address, &tempPhys) == 0) {
                phys = tempPhys;
                Cache[address] = tempPhys;
                return true;
            }
        }
        return false;
    }

    template<typename T>
    bool Read(uintptr_t address, T& out) {
        uintptr_t physAddress;
        if (!Convert(address, physAddress)) return false;
        return HookRead(pVMAddr, physAddress, &out, sizeof(T)) == 0;
    }

    template<typename T>
    T Read(uintptr_t address) {
        T result{};
        Read(address, result);
        return result;
    }

    template<typename T>
    static void Write(uintptr_t address, const T& value) {
        uintptr_t physAddress;
        if (Convert(address, physAddress))
            HookWrite(pVMAddr, physAddress, (void*)&value, sizeof(T));
    }

    template<typename T>
    static bool ReadFast2(uintptr_t address, T* data) {
        uintptr_t physAddress;
        if (!Convert(address, physAddress)) return false;
        return HookRead(pVMAddr, physAddress, data, sizeof(T)) == 0;
    }

    template<typename T>
    static T ReadS(uintptr_t address) {
        T result{};
        uintptr_t physAddress;
        if (Convert(address, physAddress))
            HookRead(pVMAddr, physAddress, &result, sizeof(T));
        return result;
    }

    template<typename T>
    bool ReadArray(uintptr_t address, std::vector<T>& array) {
        uintptr_t convertedAddress;
        if (!Convert(address, convertedAddress)) return false;
        return HookRead(pVMAddr, convertedAddress, array.data(), sizeof(T) * array.size()) == 0;
    }

    template<typename T>
    bool ReadArrayZ2(uintptr_t address, std::vector<T>& array) {
        uintptr_t convertedAddress;
        if (!Convert(address, convertedAddress)) return false;
        return HookRead(pVMAddr, convertedAddress, array.data(), sizeof(T) * array.size()) == 0;
    }

    std::string utf16_to_utf8(const std::wstring& wstr) {
        if (wstr.empty()) return "";
        int needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
        if (needed <= 0) return "";
        std::string result(needed, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), result.data(), needed, nullptr, nullptr);
        return result;
    }

    std::string ReadStringZ2(uintptr_t address, int size, bool unicode = true) {
        if (size <= 0 || size >= 256) return "";
        if (unicode) {
            std::vector<uint8_t> stringBytes(size * 2);
            if (!ReadArrayZ2(address, stringBytes)) return "";
            const wchar_t* wptr = reinterpret_cast<const wchar_t*>(stringBytes.data());
            int needed = WideCharToMultiByte(CP_UTF8, 0, wptr, size, nullptr, 0, nullptr, nullptr);
            if (needed <= 0) return "";
            std::string result(needed, '\0');
            WideCharToMultiByte(CP_UTF8, 0, wptr, size, result.data(), needed, nullptr, nullptr);
            auto null_pos = result.find('\0');
            if (null_pos != std::string::npos) result.resize(null_pos);
            return result;
        }
        else {
            std::vector<uint8_t> stringBytes(size);
            if (!ReadArrayZ2(address, stringBytes)) return "";
            auto end = std::find(stringBytes.begin(), stringBytes.end(), uint8_t(0));
            return std::string(stringBytes.begin(), end);
        }
    }

    std::string String(uintptr_t address, int size, bool unicode = true) {
        std::vector<uint8_t> stringBytes(size);
        if (!ReadArray(address, stringBytes)) return "";
        std::string result;
        if (unicode) {
            std::wstring wstr(reinterpret_cast<wchar_t*>(stringBytes.data()), size / 2);
            result = utf16_to_utf8(wstr);
        }
        else {
            result = std::string(reinterpret_cast<char*>(stringBytes.data()), size);
        }
        auto null_pos = result.find('\0');
        if (null_pos != std::string::npos) result.resize(null_pos);
        return result;
    }
};

inline MemoryUtils Mem;
