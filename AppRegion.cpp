#include "AppRegion.h"

namespace window_region
{
    void WindowRegion::reassign()
    {
        if (m_region)
        {
            DeleteObject(m_region);
        }
        m_region = CreateRoundRectRgn(m_x1, m_y1, m_x2, m_y2, m_roundw, m_roundh);
    }

    void WindowRegion::updateRound(int roundw, int roundh)
    {
        m_roundw = roundw;
        m_roundh = roundh;
    }

    void WindowRegion::updateCoord(int x1, int y1, int x2, int y2)
    {
        m_x1 = x1;
        m_y1 = y1;
        m_x2 = x2;
        m_y2 = y2;
    }

    WindowRegion::WindowRegion(int x1, int y1, int x2, int y2, int roundw, int roundh)
        : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
        , m_roundw(roundw), m_roundh(roundh)
        , m_region(CreateRoundRectRgn(x1, y1, x2, y2, m_roundw, m_roundh))
    {}

    WindowRegion& WindowRegion::SetRound(int roundw, int roundh)
    {
        updateRound(roundw, roundh);
        reassign();
        return *this;
    }

    WindowRegion& WindowRegion::UnsetRound()
    {
        return SetRound(0, 0);
    }

    WindowRegion& WindowRegion::ResetRegion(int x1, int y1, int x2, int y2)
    {
        updateCoord(x1, y1, x2, y2);
        reassign();
        return *this;
    }

    int WindowRegion::ApplyToWindow(HWND hwnd) const
    {
        return SetWindowRgn(hwnd, m_region, true);
    }

    WindowRegion::~WindowRegion()
    {
        DeleteObject(m_region);
    }
}



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
