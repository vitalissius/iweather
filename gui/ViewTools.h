#pragma once

#include "../core/Utility.h"

#include <Shlobj.h>
#include <Windowsx.h>

class Font : private Noncopyable {
public:
    Font() = default;
    Font(const int height, const int width, const int weight, const TCHAR* const faceName);
    void Make(const int height, const int width, const int weight, const TCHAR* const faceName);
    Font(Font&& rhs);
    Font& operator=(Font&& rhs);
    HFONT GetHandle() const;
    ~Font();

private:
    HFONT m_handle{};
};



class Pen : private Noncopyable {
public:
    Pen() = default;
    Pen(const UINT style, const int width, const COLORREF colorref);
    void Make(const UINT style, const int width, const COLORREF colorref);
    Pen(Pen&& rhs);
    Pen& operator=(Pen&& rhs);
    HPEN GetPenHandle() const;
    ~Pen();

private:
    HPEN m_penHandle{};
};



class Brush : private Noncopyable {
public:
    Brush() = default;
    explicit Brush(const COLORREF colorref);
    void Make(const COLORREF colorref);
    Brush(Brush&& rhs);
    Brush& operator=(Brush&& rhs);
    void UpdateColor(const COLORREF colorref);
    HBRUSH GetHandle() const;
    ~Brush();
    static void SetColorToWindow(const COLORREF colorref, const HWND hWnd);

private:
    HBRUSH m_handle{};
};
