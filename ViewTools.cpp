#include "ViewTools.h"

Font::Font(const int height, const int width, const int weight, const TCHAR* const faceName)
{
    m_handle = CreateFont(height, width, 0, 0, weight, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, faceName);
}

void Font::Make(const int height, const int width, const int weight, const TCHAR* const faceName)
{
    *this = std::move(Font(height, width, weight, faceName));
}

Font::Font(Font&& rhs)
{
    m_handle = rhs.m_handle;
    rhs.m_handle = 0;
}

Font& Font::operator=(Font&& rhs)
{
    m_handle = rhs.m_handle;
    rhs.m_handle = 0;
    return *this;
}

HFONT Font::GetHandle() const
{
    return m_handle;
}

Font::~Font()
{
    DeleteFont(m_handle);
}



Pen::Pen(const UINT style, const int width, const COLORREF colorref)
{
    m_penHandle = CreatePen(style, width, colorref);
}

void Pen::Make(const UINT style, const int width, const COLORREF colorref)
{
    *this = Pen(style, width, colorref);
}

Pen::Pen(Pen&& rhs)
{
    m_penHandle = rhs.m_penHandle;
    rhs.m_penHandle = 0;
}

Pen& Pen::operator=(Pen&& rhs)
{
    m_penHandle = rhs.m_penHandle;
    rhs.m_penHandle = 0;
    return *this;
}

HPEN Pen::GetPenHandle() const
{
    return m_penHandle;
}

Pen::~Pen()
{
    DeletePen(m_penHandle);
}



Brush::Brush(const COLORREF colorref)
{
    m_handle = CreateSolidBrush(colorref);
}

void Brush::Make(const COLORREF colorref)
{
    *this = Brush(colorref);
}

Brush::Brush(Brush&& rhs)
{
    m_handle = rhs.m_handle;
    rhs.m_handle = 0;
}

Brush& Brush::operator=(Brush&& rhs)
{
    m_handle = rhs.m_handle;
    rhs.m_handle = 0;
    return *this;
}

void Brush::UpdateColor(const COLORREF colorref)
{
    DeleteBrush(m_handle);
    m_handle = CreateSolidBrush(colorref);
}

HBRUSH Brush::GetHandle() const
{
    return m_handle;
}

Brush::~Brush()
{
    DeleteBrush(m_handle);
}

void Brush::SetColorToWindow(const COLORREF colorref, const HWND hWnd)
{
    HDC deviceContextHandle = GetDC(hWnd);
    HBRUSH newBrush = CreateSolidBrush(colorref);
    HBRUSH oldBrush = SelectBrush(deviceContextHandle, newBrush);
    RECT rect;
    GetClientRect(hWnd, &rect);
    FillRect(deviceContextHandle, &rect, newBrush);
    DeleteBrush(SelectBrush(deviceContextHandle, oldBrush));
    ReleaseDC(hWnd, deviceContextHandle);
}
