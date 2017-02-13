#pragma once

#include "Settings.h"
#include "ViewTools.h"
#include "Utility.h"
#include "Resource.h"

#include <Windows.h>
#include <Windowsx.h>

#include <array>
#include <vector>

class Tiles : private Noncopyable {
public:
    Tiles();
    void SetLeftTopCorner(POINT point=POINT{ 21, 180 });
    void SetCurrentColor(COLORREF colorref, POINT position);
    tstring GetColorDescription();
    COLORREF GetCurrentColorColorref() const;
    POINT GetCurrentColorPosition() const;
    bool OnClick(HWND hWnd, LPARAM lParam);
    void DrawTiles(HWND hWnd, RECT rect=RECT{ 0, 0, 442, 156 }, BOOL isClick=FALSE);

private:
    std::vector<std::vector<std::pair<COLORREF, RECT>>> m_tiles2d;
    POINT m_leftTopCorner;
    POINT m_point{};
    COLORREF m_colorref{};
    POINT m_positionColor{};

    static constexpr int m_rows = 7;
    static constexpr int m_columns = 20;
    static constexpr int m_width = 22;
    static constexpr int m_height = 22;
    static constexpr int m_slitw = 2;
    static constexpr int m_slith = 2;
    static constexpr std::array<COLORREF, 140> m_colors{
        0x000000, 0xbc4c41, 0x7c1670, 0x3459c6, 0x994232, 0x325a55, 0xfa7a9e, 0x3c7a1f,
        0xa17937, 0xb2472d, 0xf0a053, 0xd77a16, 0x591791, 0x4f3041, 0x694b16, 0x0c3f29,
        0x8f0471, 0xe48709, 0xf16f28, 0x745070, 0xe47579, 0xe5aa32, 0x1b4f04, 0xaf4cf3,
        0xea45ed, 0x9fa507, 0x442423, 0x907a54, 0xfc2f50, 0x1b6f46, 0xce978e, 0x313a84,
        0x7f0929, 0xc13343, 0xba517d, 0xa32a2f, 0xce651c, 0x407150, 0x97462c, 0xd31098,
        0xeb51a9, 0x3e112f, 0x2fa452, 0x445b1a, 0x12401e, 0xa31d0b, 0x508f0c, 0x027252,
        0x9abe23, 0x765e67, 0x25953e, 0x41a53f, 0xec7f69, 0x2f2104, 0xcc683f, 0xe94c03,
        0xa50b3c, 0x7b153e, 0xb00ffb, 0x92597c, 0xde3275, 0x5ec323, 0xba5a45, 0xb61b7d,
        0x661d1e, 0xc907da, 0x4a812f, 0xd22f79, 0x330e0f, 0x077794, 0x7b8e0a, 0x2366b9,
        0x3b3584, 0x165f10, 0x9abe23, 0xaa2977, 0x771f12, 0xc6718b, 0x1c8e23, 0x6a4fb5,
        0x9a507a, 0x808471, 0x5f0988, 0x0fa208, 0x0e2883, 0xbe0e3c, 0x045a38, 0xfa1176,
        0x6b2ca7, 0x0c3302, 0x95407d, 0xde09c9, 0x6774b5, 0xac3318, 0xec4d2c, 0x70061b,
        0x384013, 0x781a0d, 0xd42324, 0x211350, 0x6f796a, 0x9b873e, 0xd3b517, 0x5ca300,
        0xce9869, 0xf92a17, 0xf909dc, 0x117c32, 0x844e70, 0x1f18c5, 0xd56c3f, 0xae4420,
        0x3101b7, 0xfe7f0b, 0xc70ea2, 0x083213, 0x42ae33, 0xae2a2b, 0x31008a, 0xdd089d,
        0x4d0533, 0x276917, 0x105e6d, 0x8d5ae1, 0x5b0820, 0x5e337e, 0x039160, 0x345b1d,
        0xec589e, 0x218265, 0x135ce7, 0x751f67, 0x1b2049, 0x4b2751, 0x1d7c2d, 0xed4907,
        0x0e06a4, 0x358f01, 0x406a3c, 0xffffff
    };
};
