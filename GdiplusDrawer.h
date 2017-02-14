#pragma once

#include "core/Utility.h"

#include <Windows.h>
#include <gdiplus.h>

#include <vector>
#include <memory>

#pragma comment (lib, "gdiplus.lib")

class GdiplusDrawer : private Noncopyable {
private:
    struct Data {
        UINT id;
        POINT pt;
        float rt;

        Data(UINT identifier, POINT point, float ratio);
    };

public:
    GdiplusDrawer();
    ~GdiplusDrawer();
    void Add(UINT idPng, POINT coord, float factor);
    void Draw(HWND hWnd, HBRUSH brush);

private:
    bool loadPic(const HINSTANCE hinst, UINT IDP_PNG, std::unique_ptr<Gdiplus::Bitmap>& bitmapPtrOut);

private:
    std::vector<Data> m_data;

private:
    static Gdiplus::GdiplusStartupInput ms_input;
    static ULONG_PTR ms_token;
    static int ms_initCounter;
};
