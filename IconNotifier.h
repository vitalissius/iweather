#pragma once

#include "ContextMenu.h"
#include "LanguagePack.h"
#include "Resource.h"
#include "Utility.h"

#include <strsafe.h>
#include <Windows.h>

class IconNotifier : private Noncopyable {
public:
    enum class Sound { OFF, ON };

public:
    IconNotifier() = default;
    ~IconNotifier();
    IconNotifier(IconNotifier&& rhs);
    IconNotifier& operator=(IconNotifier&& rhs);
    IconNotifier(const HWND hWnd, const UINT iconId, const UINT callbackMessage=WM_USER,
        const Sound sound=Sound::ON);
    void Make(const HWND hWnd, const UINT iconId, const UINT callbackMessage=WM_USER,
        const Sound sound=Sound::ON);
    void SetSound(IconNotifier::Sound sound);
    void Modify(const TCHAR* const textInfo, const TCHAR* const textInfoTitle,
        const TCHAR* const textTip, const UINT iconId);
    void ShowMenu(const LanguagePack::LanguageUnits langUnit, const bool pin, const bool forecast);

private:
    void updateText(TCHAR* const dest, const std::size_t destSize, const TCHAR* const src);

private:
    HWND m_handle{};
    Sound m_sound{};
    NOTIFYICONDATA m_nid{};
};
