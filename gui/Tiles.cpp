#include "Tiles.h"

extern Settings settingsCurrent;

Tiles::Tiles()
{
    for (int j = 0; j < m_rows; ++j)
    {
        std::vector<std::pair<COLORREF, RECT>> tiles1d;
        for (int i = 0; i < m_columns; ++i)
        {
            RECT tmpRect{
                i * m_width + m_slitw,  // left
                j * m_height + m_slith, // top
                (i + 1) * m_width,      // right
                (j + 1) * m_height      // bottom
            };
            tiles1d.emplace_back(m_colors[j * m_columns + i], tmpRect);
        }
        m_tiles2d.push_back(std::move(tiles1d));
    }
}

void Tiles::SetLeftTopCorner(POINT point)
{
    m_leftTopCorner = point;
    m_point = point;
}

void Tiles::SetCurrentColor(COLORREF colorref, POINT position)
{
    m_colorref = colorref;
    m_positionColor = position;
}

tstring Tiles::GetColorDescription()
{
    tstringstream tss;
    tss << TEXT("rgb(") << GetRValue(m_colorref)
        << TEXT(", ") << GetGValue(m_colorref)
        << TEXT(", ") << GetBValue(m_colorref)
        << TEXT(")");
    return tss.str();
}

COLORREF Tiles::GetCurrentColorColorref() const
{
    return m_colorref;
}

POINT Tiles::GetCurrentColorPosition() const
{
    return m_positionColor;
}

bool Tiles::OnClick(HWND hWnd, LPARAM lParam)
{
    m_point.x = GET_X_LPARAM(lParam);
    m_point.y = GET_Y_LPARAM(lParam);

    int j = (m_point.x - m_leftTopCorner.x) / m_width;
    int i = (m_point.y - m_leftTopCorner.y) / m_height;

    if (((m_point.x >= m_leftTopCorner.x) && (m_point.x < (m_leftTopCorner.x + m_width * m_columns)))
     && ((m_point.y >= m_leftTopCorner.y) && (m_point.y < (m_leftTopCorner.y + m_height * m_rows))))
    {
        m_colorref = m_colors.at(i * m_columns + j);
        m_positionColor = POINT{ j, i };
        InvalidateRect(hWnd, NULL, FALSE);
        SendMessage(hWnd, WM_PAINT, WPARAM(TRUE), LPARAM(NULL));
        return true;
    }
    return false;
}

void Tiles::DrawTiles(HWND hWnd, RECT rect, BOOL isClick)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    HDC hdcc = CreateCompatibleDC(hdc);
    HBITMAP hbmpc = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
    HBITMAP hbmpOld = SelectBitmap(hdcc, hbmpc);

    HBRUSH grayBrush = CreateSolidBrush(RGB(240, 240, 240));
    HBRUSH brushOld = SelectBrush(hdcc, grayBrush);
    PatBlt(hdcc, rect.left, rect.top, rect.right, rect.bottom, PATCOPY);
    SelectBrush(hdcc, brushOld);
    DeleteBrush(grayBrush);

    for (const auto& tiles1d : m_tiles2d)
    {
        for (const auto& tile : tiles1d)
        {
            HBRUSH brush = CreateSolidBrush(tile.first);
            FillRect(hdcc, &tile.second, brush);
            DeleteBrush(brush);
        }
    }

    Pen pen(PS_SOLID, 2, RGB(0, 0, 0));
    SelectObject(hdcc, pen.GetPenHandle());

    int j = m_positionColor.x;
    int i = m_positionColor.y;

    RECT rc = m_tiles2d[i][j].second;
    Brush brush(m_tiles2d[i][j].first);
    SelectBrush(hdcc, brush.GetHandle());

    COLORREF chooseColor = m_colors[i * m_tiles2d[0].size() + j];
    if (chooseColor == 0xffffff)
    {
        if (isClick)
        {
            chooseColor = RGB(rand() % 255, rand() % 255, rand() % 255);
        }
        else
        {
            chooseColor = settingsCurrent.GetColorColorref();
        }
    }
    m_colorref = chooseColor;

    Rectangle(hdcc, rc.left, rc.top, rc.right, rc.bottom);

    RECT blt{
        ps.rcPaint.left + m_leftTopCorner.x,
        ps.rcPaint.top + m_leftTopCorner.y,
        ps.rcPaint.right,
        ps.rcPaint.bottom,
    };
    BitBlt(hdc, blt.left, blt.top, blt.right, blt.bottom,
        hdcc, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);

    SelectObject(hdcc, hbmpOld);
    DeleteObject(hbmpc);
    DeleteDC(hdcc);
    EndPaint(hWnd, &ps);

    SendMessage(GetParent(hWnd), WMU_UPDATE_COLORREF, WPARAM(NULL), LPARAM(m_colorref));
}
