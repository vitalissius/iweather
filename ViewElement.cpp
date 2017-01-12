#include "ViewElement.h"

void InitLabelText(Label& label, const HFONT hFont, const TCHAR* labelText,
    const TCHAR* tipText, const TCHAR* tipTitleText)
{
    label.SetFont(hFont);
    label.SetText(labelText);

    if (label.GetLabelType() == Label::LabelType::WithTip)
    {
        if (tipText) { label.SetTipText(PTSTR(tipText)); }
        if (tipTitleText) { label.SetTipTitle(tipTitleText); }
    }
}

Label::Label(Label&& rhs)
{
    memcpy_s(this, sizeof(*this), &rhs, sizeof(rhs));
    ZeroMemory(&rhs, sizeof(rhs));
}

Label& Label::operator=(Label&& rhs)
{
    memcpy_s(this, sizeof(*this), &rhs, sizeof(rhs));
    ZeroMemory(&rhs, sizeof(rhs));
    return *this;
}

Label::Label(const HWND hWndParent, const UINT id, const RECT rect, const LabelType type, const UINT additStyle)
    : m_parent(hWndParent)
    , m_handle(0)
    , m_id(id)
    , m_type(type)
{
    const HINSTANCE hInst = HINSTANCE(GetWindowLong(m_parent, GWL_HINSTANCE));

    m_handle = CreateWindowEx(WS_EX_TRANSPARENT, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | additStyle,
        0, 0, 0, 0, m_parent, (HMENU)m_id, hInst, NULL);

    SetWindowPos(m_handle, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);

    if (m_type == LabelType::WithTip)
    {
        const LONG complStyle = GetWindowLong(m_handle, GWL_STYLE);

        SetWindowLong(m_handle, GWL_STYLE, complStyle | SS_NOTIFY);

        m_tipHandle = CreateWindowEx(NULL, TOOLTIPS_CLASS, TEXT(""), WS_POPUP | TTS_ALWAYSTIP,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, m_parent, NULL, hInst, NULL);

        m_tipToolInfo.cbSize = sizeof(TOOLINFO);
        m_tipToolInfo.hwnd = m_parent;
        m_tipToolInfo.hinst = hInst;
        m_tipToolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS | SS_CENTER;
        m_tipToolInfo.uId = UINT_PTR(m_handle);
        m_tipToolInfo.lpszText = TEXT("");

        SendMessage(m_tipHandle, TTM_ADDTOOL, WPARAM(BOOL(0)), LPARAM(&m_tipToolInfo));
    }
}

void Label::Make(const HWND hWndParent, const UINT id, const RECT rect, const LabelType type, const UINT additStyle)
{
    *this = Label(hWndParent, id, rect, type, additStyle);
}

BOOL Label::SetWindowPosition(const RECT rect) const
{
    return SetWindowPos(m_handle, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
}

int Label::SetText(const TCHAR* text) const
{
    const int ret = SetDlgItemText(m_parent, m_id, text);
    RECT rect{};
    GetClientRect(m_handle, &rect);
    InvalidateRect(m_handle, &rect, TRUE);
    MapWindowPoints(m_handle, m_parent, reinterpret_cast<POINT*>(&rect), 2);
    /* Prev line in detail:
    POINT point[2]{
        { rect.left, rect.top },
        { rect.right, rect.bottom }
    };
    MapWindowPoints(m_handle, m_parent, point, ARRAYSIZE(point));
    */
    RedrawWindow(m_parent, &rect, NULL, RDW_ERASE | RDW_INVALIDATE);
    return ret;
}

LRESULT Label::SetFont(const HFONT hfont) const
{
    return SendMessage(m_handle, WM_SETFONT, WPARAM(hfont), LPARAM(TRUE));
}

LRESULT Label::SetTipText(const TCHAR* text)
{
    m_tipToolInfo.lpszText = (text != nullptr ? text : TEXT(""));
    return SendMessage(m_tipHandle, TTM_UPDATETIPTEXT, WPARAM(0), LPARAM(&m_tipToolInfo));
}

LRESULT Label::SetTipTitle(const TCHAR* text) const
{
    return SendMessage(m_tipHandle, TTM_SETTITLE, WPARAM(0), LPARAM(text));
}

Label::LabelType Label::GetLabelType() const
{
    return m_type;
}



Combobox::Combobox(Combobox&& rhs)
{
    m_parent = rhs.m_parent;
    m_handle = rhs.m_handle;
    m_id = rhs.m_id;
}

Combobox& Combobox::operator=(Combobox&& rhs)
{
    m_parent = rhs.m_parent;
    m_handle = rhs.m_handle;
    m_id = rhs.m_id;
    return *this;
}

Combobox::Combobox(const HWND hWndParent, const RECT rect, const UINT id, const UINT additStyle)
    : m_parent(hWndParent)
    , m_id(id)
{
    HINSTANCE hInstance = HINSTANCE(GetWindowLong(m_parent, GWL_HINSTANCE));

    m_handle = CreateWindowEx(0, WC_COMBOBOX, NULL, WS_VISIBLE | WS_CHILD | CBS_HASSTRINGS | additStyle,
        rect.left, rect.top, rect.right, rect.bottom, m_parent, HMENU(m_id), hInstance, NULL);
}

void Combobox::Make(const HWND hWndParent, const RECT rect, const UINT id, const UINT additStyle)
{
    *this = std::move(Combobox(hWndParent, rect, id, additStyle));
}

LRESULT Combobox::SetFont(const HFONT hFont) const
{
    return SendMessage(m_handle, WM_SETFONT, WPARAM(hFont), MAKELPARAM(TRUE, 0));
}

BOOL Combobox::SetCueBannerText(const TCHAR* text) const
{
    return ComboBox_SetCueBannerText(m_handle, text);
}

BOOL Combobox::IsEmpty() const
{
    return SendMessage(m_handle, CB_GETCOUNT, 0, 0) == CB_ERR;
}

int Combobox::Clear() const
{
    int num = SendMessage(m_handle, CB_GETCOUNT, 0, 0);
    if (num != CB_ERR)
    {
        for (int ind = num - 1; ind >= 0; --ind)
        {
            SendMessage(m_handle, CB_DELETESTRING, WPARAM(ind), 0);
        }
    }
    return num;
}

LRESULT Combobox::Add(const TCHAR* text) const
{
    return SendMessage(m_handle, CB_ADDSTRING, 0, LPARAM(text));
}

BOOL Combobox::ShowDropdown() const
{
    return ComboBox_ShowDropdown(m_handle, TRUE);
}

HWND Combobox::GetHandle() const
{
    return m_handle;
}

tstring Combobox::GetText() const
{
    int length = ComboBox_GetTextLength(m_handle) + 1;
    tstring tszBuffer(length, '\0');
    ComboBox_GetText(m_handle, &tszBuffer[0], length);
    tszBuffer.pop_back();       // Remove Null sign in the end of the string
    return tszBuffer;
}

LRESULT Combobox::SetText(const TCHAR* text) const
{
    return SendMessage(m_handle, WM_SETTEXT, WPARAM(0), LPARAM(text));
}



Checkbox::Checkbox(Checkbox&& rhs)
{
    m_handle = rhs.m_handle;
}

Checkbox& Checkbox::operator=(Checkbox&& rhs)
{
    m_handle = rhs.m_handle;
    return *this;
}

Checkbox::Checkbox(const HWND parentWindow, const RECT rect, const UINT id)
{
    HINSTANCE hInstance = HINSTANCE(GetWindowLong(parentWindow, GWL_HINSTANCE));

    m_handle = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
        rect.left, rect.top, rect.right, rect.bottom, parentWindow, HMENU(id), hInstance, NULL);
}

void Checkbox::Make(const HWND parentWindow, const RECT rect, const UINT id)
{
    *this = Checkbox(parentWindow, rect, id);
}

LRESULT Checkbox::SetFont(const HFONT hFont) const
{
    return SendMessage(m_handle, WM_SETFONT, WPARAM(hFont), TRUE);
}

LRESULT Checkbox::SetText(const TCHAR* text) const
{
    return SendMessage(m_handle, WM_SETTEXT, WPARAM(NULL), LPARAM(text));
}

LRESULT Checkbox::SetChecked(bool checked) const
{
    WPARAM wParam = checked == true ? BST_CHECKED : BST_UNCHECKED;
    return SendMessage(m_handle, BM_SETCHECK, wParam, LPARAM(NULL));
}

bool Checkbox::IsChecked() const
{
    auto ret = SendMessage(m_handle, BM_GETCHECK, WPARAM(NULL), LPARAM(NULL));
    return ret == BST_UNCHECKED ? false : true;
}



Button::Button(Button&& rhs)
{
    m_handle = rhs.m_handle;
}

Button& Button::operator=(Button&& rhs)
{
    m_handle = rhs.m_handle;
    return *this;
}

Button::Button(const HWND hWndParent, const RECT rect, const UINT id)
{
    HINSTANCE hInstance = HINSTANCE(GetWindowLong(hWndParent, GWL_HINSTANCE));

    m_handle = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        rect.left, rect.top, rect.right, rect.bottom, hWndParent, HMENU(id), hInstance, NULL);
}

void Button::Make(const HWND hWndParent, const RECT rect, const UINT id)
{
    *this = Button(hWndParent, rect, id);
}

LRESULT Button::SetText(const TCHAR* text) const
{
    return SendMessage(m_handle, WM_SETTEXT, WPARAM(NULL), LPARAM(text));
}

LRESULT Button::SetFont(const HFONT hFont) const
{
    return SendMessage(m_handle, WM_SETFONT, WPARAM(hFont), MAKELPARAM(TRUE, 0));
}

BOOL Button::SetEnabled(const bool isEnabled) const
{
    return EnableWindow(m_handle, isEnabled);
}



Trackbar::Trackbar(Trackbar&& rhs)
{
    m_parent = rhs.m_parent;
    m_handle = rhs.m_handle;
}

Trackbar& Trackbar::operator=(Trackbar&& rhs)
{
    m_parent = rhs.m_parent;
    m_handle = rhs.m_handle;
    return *this;
}

Trackbar::Trackbar(const HWND hWndParent, const RECT rect, const UINT id, const UINT additStyle)
    : m_parent(hWndParent)
{
    HINSTANCE hInstance = HINSTANCE(GetWindowLong(m_parent, GWL_HINSTANCE));

    m_handle = CreateWindowEx(0, TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | additStyle,
        rect.left, rect.top, rect.right, rect.bottom, m_parent, (HMENU)id, hInstance, NULL);
}

void Trackbar::Make(const HWND hWndParent, const RECT rect, const UINT id, const UINT additStyle)
{
    *this = Trackbar(hWndParent, rect, id, additStyle);
}

void Trackbar::SetBuddy(const RECT rect, const BuddyLocation buddy,
    const TCHAR* text, const BuddyTextAlignment bta, const HFONT hfont) const
{
    HINSTANCE hInstance = HINSTANCE(GetWindowLong(m_parent, GWL_HINSTANCE));

    HWND hWndBuddy = CreateWindowEx(0, WC_STATIC, text, UINT(bta) | WS_CHILD | WS_VISIBLE,
        rect.left, rect.top, rect.right, rect.bottom, m_parent, NULL, hInstance, NULL);
    SendMessage(m_handle, TBM_SETBUDDY, WPARAM(buddy), LPARAM(hWndBuddy));
    SendMessage(hWndBuddy, WM_SETFONT, WPARAM(hfont), TRUE);
}

void Trackbar::SetRange(const int min, const int max, const BOOL redraw) const
{
    SendMessage(m_handle, TBM_SETRANGE, WPARAM(redraw), LPARAM(MAKELONG(min, max)));
}

void Trackbar::SetTicFrequency(const UINT interval) const
{
    SendMessage(m_handle, TBM_SETTICFREQ, WPARAM(interval), LPARAM(NULL));
}

void Trackbar::SetTrackPosition(const int position) const
{
    SendMessage(m_handle, TBM_SETPOS, WPARAM(TRUE), position);
}

LRESULT Trackbar::GetTrackPosition() const
{
    return SendMessage(m_handle, TBM_GETPOS, WPARAM(NULL), LPARAM(NULL));
}



EdgeView::EdgeView(EdgeView&& rhs)
{
    m_handle = rhs.m_handle;
}

EdgeView& EdgeView::operator=(EdgeView&& rhs)
{
    m_handle = rhs.m_handle;
    return *this;
}

EdgeView::EdgeView(const HWND hWndParent, const RECT rect, const UINT id)
{
    HINSTANCE hInstance = HINSTANCE(GetWindowLong(hWndParent, GWL_HINSTANCE));

    m_handle = CreateWindowEx(WS_EX_WINDOWEDGE, WC_BUTTON, NULL, WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
        rect.left, rect.top, rect.right, rect.bottom, hWndParent, HMENU(id), hInstance, NULL);
}

void EdgeView::Make(const HWND hWndParent, const RECT rect, const UINT id)
{
    *this = EdgeView(hWndParent, rect, id);
}

LRESULT EdgeView::SetFont(const HFONT hFont) const
{
    return SendMessage(m_handle, WM_SETFONT, WPARAM(hFont), MAKELPARAM(TRUE, 0));
}

LRESULT EdgeView::SetText(const TCHAR* text) const
{
    return SendMessage(m_handle, WM_SETTEXT, WPARAM(0), LPARAM(text));
}
