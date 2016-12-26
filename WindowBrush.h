#pragma once

#include <Windows.h>

class WindowBrush {
private:
    HBRUSH m_brushHandle{};

public:
    explicit WindowBrush(COLORREF colorref);
    WindowBrush(const WindowBrush&) = delete;
    WindowBrush& operator=(const WindowBrush&) = delete;
    WindowBrush(WindowBrush&&) = delete;
    WindowBrush& operator=(WindowBrush&&) = delete;
    void SetWindowColor(HWND hWnd, COLORREF colorref);
    HBRUSH GetWindowBrushHandle() const;
    ~WindowBrush();
};
