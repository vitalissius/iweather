#include "WindowBrush.h"

WindowBrush::WindowBrush(COLORREF colorref)
{
    m_brushHandle = CreateSolidBrush(colorref);
}



void WindowBrush::SetWindowColor(HWND hWnd, COLORREF colorref)
{
    DeleteObject(m_brushHandle);
    m_brushHandle = CreateSolidBrush(colorref);
    HDC hDc = GetDC(hWnd);
    RECT rect;
    GetClientRect(hWnd, &rect);
    FillRect(hDc, &rect, m_brushHandle);
}



HBRUSH WindowBrush::GetWindowBrushHandle() const
{
    return m_brushHandle;
}



WindowBrush::~WindowBrush()
{
    DeleteObject(m_brushHandle);
}
