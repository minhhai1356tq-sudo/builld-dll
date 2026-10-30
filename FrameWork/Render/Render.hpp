#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <string>
#include <vector>
#include <TlHelp32.h>

// Không dùng d3dx11.h nữa

namespace Render {
    HWND FindRenderWindow();
    
    // Nếu file gốc có khai báo class Overlay thì dán vào đây
    class Overlay {
    public:
        bool Init(HWND hwnd);
        void Render();
        void Shutdown();
    };
}
