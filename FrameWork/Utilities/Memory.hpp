#pragma once
#include <windows.h>
#include <random>
#include <memory>
#include <array>
#include <string_view>
#include <TlHelp32.h>
#include <tchar.h>
#include <iterator>
#include <winternl.h>
#include <FrameWork/FrameWork.hpp>
#include <map>

namespace FrameWork
{
	namespace Memory
	{
		HWND GetWindowHandleByPID(int Pid);
		HWND GetWindowHandleByName(std::wstring WindowName);
		DWORD GetProcessPidByName(std::wstring ProcessName);
		uint64_t GetModuleBaseByName(DWORD Pid, std::wstring ModuleName);

		void AttachProces(DWORD Pid);
		void DetachProcess();

		void ReadProcessMemoryImpl(uint64_t ReadAddress, LPVOID Read, SIZE_T Size);
		bool writepmi(uint64_t WriteAddress, LPVOID Write, SIZE_T Size);
		std::string GetProcessName(const std::string& cmp, const std::vector<std::string>& skips);

		std::string ReadProcessMemoryString(uint64_t ReadAddress, SIZE_T StringSize = 256);

		template <typename T, typename B>
		T ReadMemory(B ReadAddress)
		{
			T Read;
			ReadProcessMemoryImpl((uint64_t)ReadAddress, &Read, sizeof(T));
			return Read;
		}

		template <typename T, typename B>
		bool wrm(B WriteAddress, T Value)
		{
			return writepmi((uint64_t)WriteAddress, &Value, sizeof(T));
		}
	}
}