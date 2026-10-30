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

// Khung class Overlay tối thiểu để Overlay.cpp biên dịch được
class Overlay {
public:
    bool Init(HWND hwnd);
    void Render();
    void Shutdown();

    // Các hàm phụ trợ mà Overlay.cpp đang gọi
    void dxCreateRenderTarget();
    void dxCleanupRenderTarget();
    void dxCleanupDeviceD3D();
};
