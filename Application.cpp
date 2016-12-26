#include "Application.h"

bool Application::IsAlreadyRunning()
{
    HANDLE mtx = CreateMutex(NULL, FALSE, TEXT("{A393562E-EF7A-459F-A920-0286403D4A9B++}"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        static const TCHAR* message = TEXT("Application is already running");
        MessageBox(NULL, message, TEXT("Info"), MB_ICONINFORMATION | MB_OK);
        CloseHandle(mtx);
        return true;
    }
    return false;
}



Application::Application(
    LPCTSTR windowName,
    HINSTANCE hInstance,
    int cmdShow,
    LRESULT(WINAPI* pWndProc)(HWND, UINT, WPARAM, LPARAM),
    int width,
    int height,
    UINT classStyle,
    UINT windowStyle,
    UINT windowStyleEx,
    int xPos,
    int yPos)
{
    static constexpr TCHAR* tcsClassName = TEXT("WeatherApp-42");

    m_wcex.cbSize = sizeof(WNDCLASSEX);
    m_wcex.style = classStyle;
    m_wcex.lpfnWndProc = pWndProc;
    m_wcex.cbClsExtra = 0;
    m_wcex.cbWndExtra = 0;
    m_wcex.hInstance = hInstance;
    m_wcex.hIcon = NULL;
    m_wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    m_wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);// WHITE_BRUSH);
    m_wcex.lpszMenuName = NULL;
    m_wcex.lpszClassName = tcsClassName;

    if (!RegisterClassEx(&m_wcex))
    {
        const auto message = (tstring(TEXT("Cannot register class: ")) + tcsClassName);
        MessageBox(NULL, message.data(), TEXT("Error"), MB_ICONERROR | MB_OK);
        return;
    }

    m_mainWnd = CreateWindowEx(windowStyleEx, tcsClassName, windowName, windowStyle,
        xPos, yPos, width, height, NULL, NULL, hInstance, NULL);

    if (!m_mainWnd)
    {
        const auto message = tstring(TEXT("Cannot create window: ")) + windowName;
        MessageBox(NULL, message.data(), TEXT("Error"), MB_ICONERROR | MB_OK);
        return;
    }

    ShowWindow(m_mainWnd, cmdShow);
    UpdateWindow(m_mainWnd);
}



WPARAM Application::Run()
{
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}



BOOL Application::SetTransparency(const int percent)
{
    assert(percent <= 100 && percent >= 0);
    BYTE alpha = 255 - BYTE(2.55f * percent);
    return ::SetLayeredWindowAttributes(m_mainWnd, RGB(0, 0, 0), alpha, LWA_ALPHA);
}
