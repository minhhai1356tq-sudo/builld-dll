#pragma once
#include <Windows.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <TlHelp32.h>
#include <d3d11.h>

namespace Render {
    HWND FindRenderWindow();
}

class Overlay {
public:
    bool dxInitialize(HWND hwnd);
    void dxRefresh();
    void dxShutDown();
    void dxCreateRenderTarget();
    void dxCleanupRenderTarget();
    void dxCleanupDeviceD3D();

    bool IsSettuped() const { return bIsSetup; }
    bool IsInitialized() const { return bIsInitialized; }
    HWND GetOverlayWindow() const { return hWindow; }
    HWND GetTargetWindow() const { return hTargetWindow; }

    ID3D11Device* dxGetDevice() const { return IDevice; }
    ID3D11DeviceContext* dxGetDeviceContext() const { return IDeviceContext; }
    IDXGISwapChain* dxGetSwapChain() const { return ISwapChain; }
    ID3D11RenderTargetView* dxGetRenderTarget() const { return IRenderTarget; }

private:
    bool bIsSetup = false;
    bool bIsInitialized = false;
    HWND hWindow = nullptr;
    HWND hTargetWindow = nullptr;
    ID3D11Device* IDevice = nullptr;
    ID3D11DeviceContext* IDeviceContext = nullptr;
    IDXGISwapChain* ISwapChain = nullptr;
    ID3D11RenderTargetView* IRenderTarget = nullptr;
};
