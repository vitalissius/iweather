#include "GdiplusDrawer.h"

Gdiplus::GdiplusStartupInput GdiplusDrawer::ms_input{};
ULONG_PTR GdiplusDrawer::ms_token{};
int GdiplusDrawer::ms_initCounter{};

GdiplusDrawer::Data::Data(UINT identifier, POINT point, float ratio)
    : id(identifier), pt(point), rt(ratio) {}

GdiplusDrawer::GdiplusDrawer()
{
    if (ms_initCounter++ == 0)
    {
        Gdiplus::GdiplusStartup(&ms_token, &ms_input, nullptr);
    }
}

GdiplusDrawer::~GdiplusDrawer()
{
    if (--ms_initCounter == 0)
    {
        Gdiplus::GdiplusShutdown(ms_token);
    }
}

void GdiplusDrawer::Add(UINT idPng, POINT coord, float factor)
{
    m_data.emplace_back(idPng, coord, factor);
}

void GdiplusDrawer::Draw(HWND hWnd, HBRUSH brush)
{
    HINSTANCE hInstance = HINSTANCE(GetWindowLong(hWnd, GWLP_HINSTANCE));
    PAINTSTRUCT paintStruct;
    HDC beginPaint = BeginPaint(hWnd, &paintStruct);
    HDC dc = GetDC(hWnd);
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    FillRect(dc, &clientRect, brush);
    std::unique_ptr<Gdiplus::Bitmap> bitmapPtr = nullptr;

    for (const auto& e : m_data)
    {
        loadPic(hInstance, e.id, bitmapPtr);
        Gdiplus::Graphics graphics(beginPaint);
        Gdiplus::Rect gdiRect(e.pt.x, e.pt.y,
            INT(bitmapPtr->GetWidth() * e.rt), INT(bitmapPtr->GetHeight() * e.rt));
        graphics.DrawImage(bitmapPtr.get(), gdiRect);
    }
    m_data.clear();

    ReleaseDC(hWnd, dc);
    EndPaint(hWnd, &paintStruct);
}

bool GdiplusDrawer::loadPic(const HINSTANCE hinst, UINT IDP_PNG,
    std::unique_ptr<Gdiplus::Bitmap>& bitmapPtrOut)
{
    HRSRC findResource = FindResource(hinst, MAKEINTRESOURCE(IDP_PNG), TEXT("png"));
    if (!findResource) { return false; }

    DWORD sizeOfResource = SizeofResource(hinst, findResource);
    if (!sizeOfResource) { return false; }

    HGLOBAL loadResource = LoadResource(hinst, findResource);
    if (!loadResource) { return false; }

    const void* lockResource = LockResource(loadResource);
    if (!lockResource)
    {
        FreeResource(loadResource);
        return false;
    }

    HGLOBAL globalAlloc = GlobalAlloc(GMEM_MOVEABLE, sizeOfResource);
    if (!globalAlloc)
    {
        FreeResource(loadResource);
        return false;
    }

    LPVOID globalLock = GlobalLock(globalAlloc);
    if (globalLock != 0)
    {
        CopyMemory(globalLock, lockResource, sizeOfResource);
        IStream* streamPtr = nullptr;
        if (CreateStreamOnHGlobal(globalAlloc, FALSE, &streamPtr) == S_OK)
        {
            bitmapPtrOut.reset(new Gdiplus::Bitmap(streamPtr));
            streamPtr->Release();
        }
        GlobalUnlock(globalAlloc);
    }

    GlobalFree(globalAlloc);
    FreeResource(loadResource);
    return true;
}
