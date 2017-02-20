#include "IconNotifier.h"

IconNotifier::~IconNotifier()
{
    Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

IconNotifier::IconNotifier(IconNotifier&& rhs)
{
    CopyMemory(this, &rhs, sizeof(rhs));
    ZeroMemory(&rhs, sizeof(rhs));
}

IconNotifier& IconNotifier::operator=(IconNotifier&& rhs)
{
    CopyMemory(this, &rhs, sizeof(rhs));
    ZeroMemory(&rhs, sizeof(rhs));
    return *this;
}

IconNotifier::IconNotifier(const HWND hWnd, const UINT iconId, const UINT callbackMessage, const Sound sound)
    : m_handle(hWnd)
    , m_sound(sound)
{
    HINSTANCE hInstance = HINSTANCE(GetWindowLong(m_handle, GWL_HINSTANCE));
    HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WEATHERAPPLICATION));

    m_nid.cbSize = sizeof(NOTIFYICONDATA);
    m_nid.hWnd = m_handle;
    m_nid.uID = iconId;
    m_nid.uFlags = NIF_MESSAGE | NIF_ICON;
    m_nid.uCallbackMessage = callbackMessage;
    m_nid.hIcon = hIcon;
    Shell_NotifyIcon(NIM_ADD, &m_nid);
    if (hIcon) { DestroyIcon(hIcon); }
}

void IconNotifier::Make(const HWND hWnd, const UINT iconId, const UINT callbackMessage, const Sound sound)
{
    *this = IconNotifier(hWnd, iconId, callbackMessage, sound);
}

void IconNotifier::SetSound(IconNotifier::Sound sound)
{
    m_sound = sound;
}

void IconNotifier::Modify(const TCHAR* const textInfo, const TCHAR* const textInfoTitle,
    const TCHAR* const textTip, const UINT iconId)
{
    HINSTANCE hInstance = HINSTANCE(GetWindowLong(m_handle, GWL_HINSTANCE));
    HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(iconId));

    m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;
    m_nid.hIcon = hIcon;
    m_nid.dwInfoFlags = NIIF_USER | NIIF_RESPECT_QUIET_TIME | (m_sound == Sound::OFF ? NIIF_NOSOUND : 0);
    updateText(m_nid.szInfo, ARRAYSIZE(m_nid.szInfo), textInfo);
    updateText(m_nid.szInfoTitle, ARRAYSIZE(m_nid.szInfoTitle), textInfoTitle);
    updateText(m_nid.szTip, ARRAYSIZE(m_nid.szTip), textTip);
    Shell_NotifyIcon(NIM_MODIFY, &m_nid);
    if (hIcon) { DestroyIcon(hIcon); }
}

void IconNotifier::ShowMenu(const bool pin, const bool forecast)
{
    ContextMenu cmenu(m_handle);
    cmenu.Show(pin, forecast);
}

void IconNotifier::updateText(TCHAR* const dest, const std::size_t destSize, const TCHAR* const src)
{
    if (src) { StringCchCopy(dest, destSize, src); }
    else { dest[0] = '\0'; }
}
