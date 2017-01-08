#pragma once

#include "Utility.h"

#include <Windows.h>
#include <CommCtrl.h>       // Must be included after including Windows.h

#include <utility>

class Label;

void InitLabelText(Label& label, const HFONT hFont, const TCHAR* labelText,
    const TCHAR* tipText = nullptr, const TCHAR* tipTitleText = nullptr);

class Label : private Noncopyable {
public:
    enum LabelType { Simple, WithTip };

public:
    Label() = default;
    ~Label() = default;
    Label(Label&& rhs);
    Label& operator=(Label&& rhs);
    Label(const HWND hWndParent, const UINT id,
        const RECT rect=RECT{}, const LabelType type=LabelType::Simple, const UINT additStyle=0);
    void Make(const HWND hWndParent, const UINT id,
        const RECT rect=RECT{}, const LabelType type=LabelType::Simple, const UINT additStyle=0);
    BOOL SetWindowPosition(const RECT rect) const;
    int SetText(const TCHAR* text) const;
    LRESULT SetFont(const HFONT hfont) const;
    LRESULT SetTipText(const TCHAR* text);
    LRESULT SetTipTitle(const TCHAR* text) const;
    LabelType GetLabelType() const;

private:
    HWND m_parent{};
    HWND m_handle{};
    UINT m_id{};
    LabelType m_type{};

    HWND m_tipHandle{};
    UINT m_tipId{};
    TOOLINFO m_tipToolInfo{};
};
