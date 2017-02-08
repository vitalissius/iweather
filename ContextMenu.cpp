#include "ContextMenu.h"

ContextMenu::ContextMenu(const HWND hWnd)
    : m_handle(hWnd)
{}

ContextMenu::ContextMenu(ContextMenu&& rhs)
{
    m_handle = rhs.m_handle;
}

ContextMenu& ContextMenu::operator=(ContextMenu&& rhs)
{
    m_handle = rhs.m_handle;
    return *this;
}

HMENU ContextMenu::create(const bool pin, const bool forecast)
{
    GuiLang guiLang;
    HMENU hMenu;
    hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MFT_STRING, IDM_MENU_REFRESH, guiLang.GetWord(IDS_REFRESH).data());
    AppendMenu(hMenu, MFT_STRING | (pin ? MF_CHECKED : MF_UNCHECKED), IDM_MENU_PIN, guiLang.GetWord(IDS_PIN).data());
    AppendMenu(hMenu, MFT_STRING | (forecast ? MF_CHECKED : MF_UNCHECKED), IDM_MENU_FORECAST, guiLang.GetWord(IDS_FORECAST).data());
    AppendMenu(hMenu, MFT_SEPARATOR, 0, NULL);
    AppendMenu(hMenu, MFT_STRING, IDM_MENU_OPTION, guiLang.GetWord(IDS_SETTINGS).data());
    AppendMenu(hMenu, MFT_SEPARATOR, 0, NULL);
    AppendMenu(hMenu, MFT_STRING, IDM_MENU_CLOSE, guiLang.GetWord(IDS_CLOSE).data());
    return hMenu;
}

void ContextMenu::show(const HMENU hMenu, const POINT point)
{
    TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, m_handle, NULL);
    DestroyMenu(hMenu);
}

void ContextMenu::Show(const bool pin, const bool forecast)
{
    POINT point;
    GetCursorPos(&point);
    SetForegroundWindow(m_handle);
    show(create(pin, forecast), point);
}
