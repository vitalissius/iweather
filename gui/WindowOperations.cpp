#include "WindowOperations.h"

BOOL WindowOperations::SetTransparency(const HWND hWnd, const int percent)
{
    assert(percent <= 100 && percent >= 0);
    BYTE alpha = 255 - BYTE(2.55f * percent);
    return ::SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), alpha, LWA_ALPHA);
}

void WindowOperations::Capture(HWND hWnd, LPARAM lParam)
{
    ::SetCapture(hWnd);
    m_coord = MAKEPOINTS(lParam);
}

void WindowOperations::Release()
{
    ::ReleaseCapture();
}

void WindowOperations::MoveWnd(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    if (wParam == MK_LBUTTON)
    {
        POINTS coord = MAKEPOINTS(lParam);
        RECT rect;
        ::GetWindowRect(hWnd, &rect);
        rect.left += (coord.x - m_coord.x);
        rect.top += (coord.y - m_coord.y);
        ::SetWindowPos(hWnd, HWND_TOP, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
    }
}

void WindowOperations::MinimizeOrRestore(HWND hWnd)
{
    if (m_minimize)
    {
        SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
        m_minimize = false;
    }
    else
    {
        SendMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
        m_minimize = true;
    }
}

void WindowOperations::ForegroundWindow(HWND hWnd)
{
    ::SetForegroundWindow(hWnd);
}
