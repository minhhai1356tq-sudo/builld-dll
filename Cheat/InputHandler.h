#pragma once

#include <windows.h> // For using Windows API
#include <iostream>  // For console output (optional)

class InputHandler
{
public:
    static float GetMouseDeltaX()
    {
   
        POINT p;
        GetCursorPos(&p);
        static POINT lastPos = p;

        float deltaX = static_cast<float>(p.x - lastPos.x);
        lastPos = p;
        return deltaX;
    }

    static float GetMouseDeltaY()
    {
  
        POINT p;
        GetCursorPos(&p);
        static POINT lastPos = p;

        float deltaY = static_cast<float>(p.y - lastPos.y);
        lastPos = p;
        return deltaY;
    }

    static bool IsKeyPressed(int key)
    {
        return (GetAsyncKeyState(key) & 0x8000) != 0;
    }
};
