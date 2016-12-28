#include "AppRegion.h"

AppRegion::AppRegion(HWND hwnd, int x, int y, POINT min, POINT max)
    : m_hWnd(hwnd)
    , m_min(min)
    , m_max(max)
    , m_windowRegion(x, y, m_max.x, m_max.y)
{}



int AppRegion::ApplyToWindow() const
{
    return m_windowRegion.ApplyToWindow(m_hWnd);
}



int AppRegion::UnmakeRounding()
{
    return m_windowRegion.UnsetRound().ApplyToWindow(m_hWnd);
}



int AppRegion::MakeRounding(int width, int height)
{
    return m_windowRegion.SetRound(width, height).ApplyToWindow(m_hWnd);
}



int AppRegion::ShowMaxWindow()
{
    return m_windowRegion.ResetRegion(10, 32, m_max.x, m_max.y).ApplyToWindow(m_hWnd);
}



int AppRegion::ShowMinWindow()
{
    return m_windowRegion.ResetRegion(10, 32, m_min.x, m_min.y).ApplyToWindow(m_hWnd);
}
