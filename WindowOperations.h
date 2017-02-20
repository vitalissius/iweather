#pragma once

#include <Windows.h>

#include <assert.h>

class WindowOperations {
public:
    BOOL SetTransparency(const HWND hWnd, const int percent);

    // Must be invoked while receiving message WM_LBUTTONDOWN
    void Capture(HWND hWnd, LPARAM lParam);

    // Must be invoked while receiving message WM_LBUTTONUP
    void Release();

    // Must be invoked while receiving message WM_MOUSEMOVE
    void MoveWnd(HWND hWnd, WPARAM wParam, LPARAM lParam);

    // Must be invoked when double clicking on applications icon in notification area
    void MinimizeOrRestore(HWND hWnd);

    // Must be invoked when processing click LMB on application icon in notification area
    void ForegroundWindow(HWND hWnd);

private:
    POINTS m_coord{};
    bool m_minimize{};
};
