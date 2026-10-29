#include "Misc.hpp"

#include <FrameWork/FrameWork.hpp>
#include <algorithm>
#include <winhttp.h>
#include <sstream>
static BOOL CheckForUIAccess(DWORD* pdwErr, BOOL* pfUIAccess)
{
	BOOL result = FALSE;
	HANDLE hToken;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	{
		DWORD dwRetLen;

		if (GetTokenInformation(hToken, TokenUIAccess, pfUIAccess, sizeof(*pfUIAccess), &dwRetLen))
		{
			result = TRUE;
		}
		else
		{
			*pdwErr = GetLastError();
		}
		CloseHandle(hToken);
	}
	else
	{
		*pdwErr = GetLastError();
	}

	return result;
}

namespace FrameWork
{
	int Misc::RandomInt(int Lower, int Max)
	{
		return (rand() % (Max - Lower + 1)) + Lower;
	}

	std::wstring Misc::RandomString(size_t Length)
	{
		auto Randchar = []() -> char
			{
				const char* Charset = ("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
				const size_t MaxIndex = (sizeof(Charset) - 1);
				return Charset[rand() % MaxIndex];
			};

		std::wstring Str(Length, 0);
		std::generate_n(Str.begin(), Length, Randchar);
		return Str;
	}

	std::string Misc::Wstring2String(std::wstring Input)
	{
		return std::string(Input.begin(), Input.end());
	}

	std::wstring Misc::String2WString(std::string Input)
	{
		return std::wstring(Input.begin(), Input.end());
	}

	void Misc::InitializeConsole()
	{
		(AllocConsole)();
		freopen(("CONIN$"), ("r"), stdin);
		freopen(("CONOUT$"), ("w"), stdout);
		(SetConsoleTitleA)(("Debug Console"));
	}

	void Misc::ShutDownConsole()
	{
		(FreeConsole)();
	}

	std::string Misc::DownloadServerInfo(std::wstring IP, int PORT)
	{
		HINTERNET hSession = WinHttpOpen((L"A WinHTTP FiveM Request Program/1.0"), WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

		if (hSession)
		{
			HINTERNET hConnect = WinHttpConnect(hSession, IP.c_str(), PORT, 0);

			if (hConnect)
			{
				HINTERNET hRequest = WinHttpOpenRequest(hConnect, (L"GET"), (L"/players.json"), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

				if (hRequest)
				{
					if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
					{
						if (WinHttpReceiveResponse(hRequest, NULL))
						{
							DWORD dwSize = 0;
							DWORD dwDownloaded = 0;
							LPSTR pszOutBuffer;
							BOOL  bResults = FALSE;
							std::string Response;

							do
							{
								dwSize = 0;
								if (WinHttpQueryDataAvailable(hRequest, &dwSize))
								{
									pszOutBuffer = new char[dwSize + 1];
									if (!pszOutBuffer)
									{
										dwSize = 0;
									}
									else
									{
										ZeroMemory(pszOutBuffer, dwSize + 1);

										if (WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
											Response.append(pszOutBuffer, dwDownloaded);
									}

									delete[] pszOutBuffer;
								}

							} while (dwSize > 0);

							return Response;
						}
					}

					WinHttpCloseHandle(hRequest);
				}

				WinHttpCloseHandle(hConnect);
			}

			WinHttpCloseHandle(hSession);
		}

		return NULL;
	}

	RTL_OSVERSIONINFOW Misc::GetRealOSVersion()
	{
		HMODULE hMod = (GetModuleHandleW)((L"ntdll.dll"));
		if (hMod)
		{
			RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)(GetProcAddress)(hMod, ("RtlGetVersion"));
			if (fxPtr != nullptr)
			{
				RTL_OSVERSIONINFOW rovi = { 0 };
				rovi.dwOSVersionInfoSize = sizeof(rovi);
				if (fxPtr(&rovi) == 0x00000000)
				{
					return rovi;
				}
			}
		}
		RTL_OSVERSIONINFOW rovi = { 0 };
		return rovi;
	}

	std::string Misc::GetWindowsFullBuildNumber()
	{
		DWORD Dummy;
		DWORD VersionSize = (GetFileVersionInfoSizeA)(("kernel32.dll"), &Dummy);
		if (VersionSize == 0)
			return NULL;

		std::unique_ptr<BYTE[]> pBuffer(new BYTE[VersionSize]);
		if (!(GetFileVersionInfoA)(("kernel32.dll"), 0, VersionSize, pBuffer.get()))
			return NULL;

		VS_FIXEDFILEINFO* pFileInfo;
		UINT VersionLength;
		if (!(VerQueryValueA)(pBuffer.get(), ("\\"), reinterpret_cast<void**>(&pFileInfo), &VersionLength))
			return NULL;

		std::stringstream aa;

		aa << HIWORD(pFileInfo->dwProductVersionMS) << (".") << LOWORD(pFileInfo->dwProductVersionMS) << (".") << HIWORD(pFileInfo->dwProductVersionLS) << (".") << LOWORD(pFileInfo->dwProductVersionLS);

		return aa.str();
	}

	bool Misc::HaveUIAccessPermission()
	{
		DWORD dwErr;
		HANDLE hTokenUIAccess;
		BOOL fUIAccess;
		if (CheckForUIAccess(&dwErr, &fUIAccess))
		{
			if (fUIAccess)
			{
				return true;
			}

			return false;
		}

		return false;
	}
}