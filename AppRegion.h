#pragma once

#include "WindowRegion.h"

class AppRegion {
private:
    HWND m_hWnd;
    POINT m_min;
    POINT m_max;
    WindowRegion m_windowRegion;

public:
    AppRegion(HWND hwnd, int x, int y, POINT min, POINT max);
    AppRegion(const AppRegion&) = delete;
    AppRegion& operator=(const AppRegion&) = delete;
    AppRegion(AppRegion&&) = default;
    AppRegion& operator=(AppRegion&&) = default;
    ~AppRegion() = default;
    // Must be invoked while receiving message WM_CREATE
    int ApplyToWindow() const;
    int UnmakeRounding();
    int MakeRounding(int width, int height);
    int ShowMaxWindow();
    int ShowMinWindow();
};
