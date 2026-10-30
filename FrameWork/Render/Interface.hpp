#pragma once

#include <FrameWork/FrameWork.hpp>
#include <d3d11.h>
// #include <d3dx11.h> // Đã comment để build trên Github Actions

namespace FrameWork
{
    class Interface
    {
    public:
        Interface(HWND Window, HWND TargetWindow, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
        {
            Initialize(Window, TargetWindow, Device, DeviceContext);
        }
        ~Interface() { ShutDown(); }

        void Initialize(HWND Window, HWND TargetWindow, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);
        void UpdateStyle();
        void RenderGui();
        void HandleMenuKey();
        void WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        void ShutDown();
        void SetMenuOpen(bool state) { bIsMenuOpen = state; }

        bool GetMenuOpen() const { return bIsMenuOpen; }

        UINT ResizeWidht = 0;
        UINT ResizeHeight = 0;

    private:
        void AimbotTab();
        void VisualsTab();
        void MiscellaneousTab();
        void VehiclesTab();
        void PlayersTab();
        void SettingsTab();

        void ApplyWindowStyle(bool menuOpen);

        HWND          hWindow = nullptr;
        HWND          hTargetWindow = nullptr;
        ID3D11Device* IDevice = nullptr;

        int  CurrentTab = 0;
        int  OldCurrentTab = 0;
        bool bIsMenuOpen = false;
        bool wasKeyDown = false;
    };
}
