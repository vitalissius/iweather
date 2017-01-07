#pragma once

#include <Windows.h>

namespace window_region
{
    class WindowRegion {
    private:
        int m_x1;
        int m_y1;
        int m_x2;
        int m_y2;
        int m_roundw;
        int m_roundh;
        HRGN m_region{};

    private:
        void reassign();
        void updateRound(int roundw, int roundh);
        void updateCoord(int x1, int y1, int x2, int y2);

    public:
        WindowRegion(int x1, int y1, int x2, int y2, int roundw = 0, int roundh = 0);
        WindowRegion(const WindowRegion&) = delete;
        WindowRegion& operator=(const WindowRegion&) = delete;
        WindowRegion(WindowRegion&&) = default;
        WindowRegion& operator=(WindowRegion&&) = default;
        WindowRegion& SetRound(int roundw, int roundh);
        WindowRegion& UnsetRound();
        WindowRegion& ResetRegion(int x1, int y1, int x2, int y2);
        int ApplyToWindow(HWND hwnd) const;
        ~WindowRegion();
    };
}



class AppRegion {
private:
    HWND m_hWnd;
    POINT m_min;
    POINT m_max;
    window_region::WindowRegion m_windowRegion;

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
