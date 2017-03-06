#pragma once

#include "../core/Utility.h"
#include "ViewTools.h"

#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>       // Must be included after including Windows.h

#include <utility>

class Label;

void InitLabelText(Label& label, const Font& font, const tstring& labelText,
    const tstring& tipText=TEXT(""), const tstring& tipTitleText=TEXT(""));

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
    LRESULT SetMaxTipWidth(size_t maxWidth);
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



class Combobox : private Noncopyable {
private:
    static constexpr std::size_t m_maxLength = 128;

public:
    Combobox() = default;
    ~Combobox() = default;
    Combobox(Combobox&& rhs);
    Combobox& operator=(Combobox&& rhs);
    Combobox(const HWND hWndParent, const RECT rect, const UINT id, const UINT additStyle=0);
    void Make(const HWND hWndParent, const RECT rect, const UINT id, const UINT additStyle=0);
    LRESULT SetFont(const HFONT hFont) const;
    BOOL SetCueBannerText(const TCHAR* text) const;
    BOOL IsEmpty() const;
    int Clear() const;
    LRESULT Add(const TCHAR* text) const;
    BOOL ShowDropdown() const;
    HWND GetHandle() const;
    tstring GetText() const;
    LRESULT SetText(const TCHAR* text) const;
    int SetSelection(const int index) const;
    int GetSelection() const;

private:
    HWND m_parent{};
    HWND m_handle{};
    UINT m_id{};
};



class Checkbox : private Noncopyable {
public:
    Checkbox() = default;
    ~Checkbox() = default;
    Checkbox(Checkbox&& rhs);
    Checkbox& operator=(Checkbox&& rhs);
    Checkbox(const HWND parentWindow, const RECT rect, const UINT id);
    void Make(const HWND parentWindow, const RECT rect, const UINT id);
    LRESULT SetFont(const HFONT hFont) const;
    LRESULT SetText(const TCHAR* text) const;
    LRESULT SetChecked(bool checked) const;
    bool IsChecked() const;

private:
    HWND m_handle{};
};



class Button : private Noncopyable {
public:
    Button() = default;
    ~Button() = default;
    Button(Button&& rhs);
    Button& operator=(Button&& rhs);
    Button(const HWND hWndParent, const RECT rect, const UINT id);
    void Make(const HWND hWndParent, const RECT rect, const UINT id);
    LRESULT SetText(const TCHAR* text) const;
    LRESULT SetFont(const HFONT hFont) const;
    BOOL SetEnabled(const bool isEnabled) const;
    BOOL IsDisabled() const;

private:
    HWND m_handle{};
};



class Trackbar : private Noncopyable {
public:
    enum class BuddyLocation { Right = FALSE, Left = TRUE };
    enum class BuddyTextAlignment { Left = SS_LEFT, Center = SS_CENTER, Right = SS_RIGHT };

public:
    Trackbar() = default;
    ~Trackbar() = default;
    Trackbar(Trackbar&& rhs);
    Trackbar& operator=(Trackbar&& rhs);
    Trackbar(const HWND hWndParent, const RECT rect, const UINT id, const UINT additStyle);
    void Make(const HWND hWndParent, const RECT rect, const UINT id, const UINT additStyle);
    void SetBuddy(const RECT rect, const BuddyLocation buddy,
        const TCHAR* text, const BuddyTextAlignment bta, const HFONT hfont) const;
    void SetRange(const int min, const int max, const BOOL redraw=TRUE) const;
    void SetTicFrequency(const UINT interval) const;
    void SetTrackPosition(const int position) const;
    LRESULT GetTrackPosition() const;

private:
    HWND m_parent{};
    HWND m_handle{};
};



class EdgeView : private Noncopyable {
public:
    EdgeView() = default;
    ~EdgeView() = default;
    EdgeView(EdgeView&& rhs);
    EdgeView& operator=(EdgeView&& rhs);
    EdgeView(const HWND hWndParent, const RECT rect, const UINT id=-1);
    void Make(const HWND hWndParent, const RECT rect, const UINT id=-1);
    LRESULT SetFont(const HFONT hFont) const;
    LRESULT SetText(const TCHAR* text) const;

private:
    HWND m_handle{};
};
