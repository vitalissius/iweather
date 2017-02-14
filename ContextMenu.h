#pragma once

#include "GuiLang.h"
#include "core/LanguagePack.h"
#include "Resource.h"

class ContextMenu : private Noncopyable {
public:
    ContextMenu() = default;
    ~ContextMenu() = default;
    explicit ContextMenu(const HWND hWnd);
    ContextMenu(ContextMenu&& rhs);
    ContextMenu& operator=(ContextMenu&& rhs);
    void Show(const bool pin, const bool forecast);

private:
    HMENU create(const bool pin, const bool forecast);
    void show(const HMENU hMenu, const POINT point);

private:
    HWND m_handle{};
};
