#include "Label.h"

void InitLabelText(Label& label, HFONT font, const TCHAR* labelText, const PTSTR tipText, const TCHAR* tipTitleText)
{
    label.SetFont(font);
    label.SetText(labelText);

    if (label.GetLabelType() == Label::LabelType::WithTip)
    {
        if (tipText) { label.SetTipText(tipText); }
        if (tipTitleText) { label.SetTipTitle(tipTitleText); }
    }
};



Label::Label(HWND parentWnd, UINT labelId, RECT rect, LabelType labelType, DWORD additStyle)
    : m_parentWindowHandle(parentWnd)
    , m_labelWindowHandle(0)
    , m_labelId(labelId)
    , m_labelType(labelType)
{
    HINSTANCE hInst = HINSTANCE(GetWindowLong(m_parentWindowHandle, GWL_HINSTANCE));

    m_labelWindowHandle = CreateWindowEx(WS_EX_TRANSPARENT, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | additStyle,
        0, 0, 0, 0, m_parentWindowHandle, (HMENU)m_labelId, hInst, NULL);

    SetWindowPos(m_labelWindowHandle, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);

    if (m_labelType == LabelType::WithTip)
    {
        LONG complStyle = GetWindowLong(m_labelWindowHandle, GWL_STYLE);

        SetWindowLong(m_labelWindowHandle, GWL_STYLE, complStyle | SS_NOTIFY);

        m_tipWindowHandle = CreateWindowEx(NULL, TOOLTIPS_CLASS, TEXT(""), WS_POPUP | TTS_ALWAYSTIP,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, m_parentWindowHandle, NULL, hInst, NULL);

        m_tipToolInfo.cbSize = sizeof(TOOLINFO);
        m_tipToolInfo.hwnd = m_parentWindowHandle;
        m_tipToolInfo.hinst = hInst;
        m_tipToolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS | SS_CENTER;
        m_tipToolInfo.uId = (UINT_PTR)m_labelWindowHandle;
        m_tipToolInfo.lpszText = TEXT("");

        SendMessage(m_tipWindowHandle, TTM_ADDTOOL, (WPARAM)(BOOL)0, (LPARAM)&m_tipToolInfo);
    }
}



void Label::Make(HWND parentWnd, UINT labelId, RECT rect, LabelType labelType, DWORD additStyle)
{
    *this = std::move(Label(parentWnd, labelId, rect, labelType, additStyle));
}



BOOL Label::SetWindowPosition(RECT rect)
{
    return SetWindowPos(m_labelWindowHandle, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
}



int Label::SetText(const TCHAR* text)
{
    int rval = SetDlgItemText(m_parentWindowHandle, m_labelId, text);
    RECT rect;
    GetClientRect(m_labelWindowHandle, &rect);
    InvalidateRect(m_labelWindowHandle, &rect, TRUE);
    MapWindowPoints(m_labelWindowHandle, m_parentWindowHandle, reinterpret_cast<POINT*>(&rect), 2);
    /* Prev line in detail:
    POINT point[2]{
        { rect.left, rect.top },
        { rect.right, rect.bottom }
    };
    MapWindowPoints(m_labelWindowHandle, m_parentWindowHandle, point, ARRAYSIZE(point));
    */
    RedrawWindow(m_parentWindowHandle, &rect, NULL, RDW_ERASE | RDW_INVALIDATE);
    return rval;
}



LRESULT Label::SetFont(HFONT hfont)
{
    return SendMessage(m_labelWindowHandle, WM_SETFONT, (WPARAM)hfont, (LPARAM)TRUE);
}



LRESULT Label::SetTipText(const PTSTR text)
{
    m_tipToolInfo.lpszText = (text != nullptr ? text : TEXT(""));
    return SendMessage(m_tipWindowHandle, TTM_UPDATETIPTEXT, 0, (LPARAM)&m_tipToolInfo);
}



LRESULT Label::SetTipTitle(const TCHAR* text)
{
    return SendMessage(m_tipWindowHandle, TTM_SETTITLE, (WPARAM)0, (LPARAM)text);
}



Label::LabelType Label::GetLabelType() const
{
    return m_labelType;
}
