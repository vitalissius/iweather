#pragma once

#include "WindowRegion.h"

class AppRegion {
private:
    HWND m_hWnd;
    POINT m_max;
    POINT m_min;
    WindowRegion m_windowRegion;

public:
    AppRegion(HWND hwnd, int x, int y, POINT max, POINT min);
    // Must be invoked while receiving message WM_CREATE
    int ApplyToWindow() const;
    int UnmakeRounding();
    int MakeRounding(int width, int height);
    int ShowMaxWindow();
    int ShowMinWindow();
};
