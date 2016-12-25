#pragma once

#include <Windows.h>

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
    WindowRegion(int x1, int y1, int x2, int y2, int roundw=0, int roundh=0);
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
