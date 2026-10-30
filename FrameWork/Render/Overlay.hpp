#pragma once
#include <Windows.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <TlHelp32.h>
#include <d3d11.h>
#include <functional>

namespace Render {
    HWND FindRenderWindow();
}

class Overlay {
public:
    void Setup(HWND TargetHWND);
    void Initialize();
    void ShutDown();
    void UpdateWindowPos();
    void SetupWindowProcHook(std::function<void(HWND, UINT, WPARAM, LPARAM)> Funtion);

    void dxInitialize();
    void dxRefresh();
    void dxShutDown();
    void dxCreateRenderTarget();
    void dxCleanupRenderTarget();
    void dxCleanupDeviceD3D();

    bool IsSettuped();
    bool IsInitialized();
    HWND GetOverlayWindow();
    HWND GetTargetWindow();

    ID3D11Device* dxGetDevice();
    ID3D11DeviceContext* dxGetDeviceContext();
    IDXGISwapChain* dxGetSwapChain();
    ID3D11RenderTargetView* dxGetRenderTarget();
};
