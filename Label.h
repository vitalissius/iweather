#pragma once

#include <Windows.h>
#include <CommCtrl.h>

#include <utility>

class Label;

void InitLabelText(Label& label, HFONT font, const TCHAR* labelText,
    const PTSTR tipText=nullptr, const TCHAR* tipTitleText=nullptr);



class Label {
public:
    enum LabelType { Simple, WithTip };

private:
    HWND m_parentWindowHandle{};
    HWND m_labelWindowHandle{};
    UINT m_labelId{};
    LabelType m_labelType{ LabelType::Simple };

    HWND m_tipWindowHandle{};
    DWORD m_tipId{};
    TOOLINFO m_tipToolInfo{};

public:
    Label() = default;
    Label(const Label&) = delete;
    Label& operator=(const Label&) = delete;
    Label(Label&&) = default;
    Label& operator=(Label&&) = default;
    Label(HWND parentWnd, UINT labelId, RECT rect, LabelType labelType=LabelType::Simple, DWORD additStyle=0);
    void Make(HWND parentWnd, UINT labelId, RECT rect, LabelType labelType=LabelType::Simple, DWORD additStyle=0);
    BOOL SetWindowPosition(RECT rect);
    int SetText(const TCHAR* text);
    LRESULT SetFont(HFONT hfont);
    LRESULT SetTipText(const PTSTR text);
    LRESULT SetTipTitle(const TCHAR* text=nullptr);
    LabelType GetLabelType() const;
};
