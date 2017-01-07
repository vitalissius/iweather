#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Utility.h"

#include <Windows.h>

#include <cassert>

class Application {
private:
    HWND m_mainWnd{};
    WNDCLASSEX m_wcex{};

public:
    Application(
        LPCTSTR windowName,
        HINSTANCE hInstance,
        int cmdShow,
        LRESULT(WINAPI* pWndProc)(HWND, UINT, WPARAM, LPARAM),
        int width=CW_USEDEFAULT,
        int height=0,
        UINT classStyle=(CS_HREDRAW | CS_VREDRAW),
        UINT windowStyle=WS_OVERLAPPEDWINDOW,
        UINT windowStyleEx=(WS_EX_LAYERED | WS_EX_TOOLWINDOW),
        int xPos=CW_USEDEFAULT,
        int yPos=0);
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;
    ~Application() = default;
    WPARAM Run();
    BOOL SetTransparency(const int percent=0);
    static bool IsAlreadyRunning();
};

#endif // APPLICATION_H
