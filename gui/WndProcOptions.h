#pragma once

#include "../core/AccuPlaces.h"
#include "../core/YahooPlaces.h"
#include "Conv.h"
#include "ViewElement.h"
#include "ViewTools.h"
#include "Tiles.h"
#include "GuiLang.h"
#include "Settings.h"

#include <Windows.h>

extern Settings settingsCurrent;

BOOL CALLBACK WindowProcOptionDlg(HWND hWndOption, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    static HINSTANCE hInstance = HINSTANCE(GetWindowLong(hWndOption, GWL_HINSTANCE));
    static Font font(18, 0, FW_DONTCARE, TEXT("Segoe UI Light"));

    static EdgeView groupPlace;
    static Combobox comboboxPlace;
    static Button buttonSearch;

    static EdgeView groupTransparancy;
    static Trackbar trackbarTransparancy;

    static EdgeView groupColor;
    static Tiles tiles;

    static EdgeView groupMiscellaneous;
    static Label label1;
    static Label label2;
    static Label label3;
    static Label label4;
    static Label label5;
    static Combobox comboboxLanguage;
    static Combobox comboboxStyle;
    static Combobox comboboxPressure;
    static Combobox comboboxSpeed;
    static Combobox comboboxTemperature;

    static EdgeView groupOther;
    static Checkbox checkboxAutorun;
    static Checkbox checkboxForecast;
    static Checkbox checkboxPin;
    static Checkbox checkboxSound;
    static Checkbox checkboxRound;

    static Button buttonSave;
    static Button buttonCancel;
    static Button buttonApply;

    static GuiLang guilang;

    static Settings settingsNew{};

    switch (uMessage)
    {
        case WM_INITDIALOG:
        {
            const int width = 500;
            const int height = 590;
            const int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
            const int y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

            SetWindowPos(hWndOption, HWND_TOP, x, y, width, height, SWP_SHOWWINDOW);
            SetWindowText(hWndOption, guilang.GetWord(IDS_SETTINGS).data());

            groupPlace.Make(hWndOption, RECT{ 10, 6, 464, 61 }, -1);
            groupPlace.SetFont(font.GetHandle());
            groupPlace.SetText((guilang.GetWord(IDS_PLACE) + settingsCurrent.GetPlace()).data());
            comboboxPlace.Make(hWndOption, RECT{ 21, 30, 314, 200 }, IDV_COMBOBOX_PLACE,
                CBS_DROPDOWN | WS_VSCROLL);
            comboboxPlace.SetFont(font.GetHandle());
            comboboxPlace.SetCueBannerText(guilang.GetWord(IDS_INPUTPLACE).data());
            buttonSearch.Make(hWndOption, RECT{ 344, 29, 120, 27 }, IDV_BUTTON_SEARCH);
            buttonSearch.SetFont(font.GetHandle());
            buttonSearch.SetText(guilang.GetWord(IDS_SEARCH).data());

            groupTransparancy.Make(hWndOption, RECT{ 10, 72, 464, 62 }, -1);
            groupTransparancy.SetFont(font.GetHandle());
            tstring tmp(guilang.GetWord(IDS_TRANSPARANCY)
                + to_tstring(settingsCurrent.GetTransparancy())
                + TEXT("%"));
            groupTransparancy.SetText(tmp.data());
            trackbarTransparancy.Make(hWndOption, RECT{ 70, 98, 343, 26 }, IDV_TRACKBAR_TRANSPARENCY,
                TBS_TOP | TBS_AUTOTICKS | TBS_HORZ | TBS_DOWNISLEFT | WS_TABSTOP);
            trackbarTransparancy.SetBuddy(RECT{ 0, 0, 50, 20 }, Trackbar::BuddyLocation::Left,
                guilang.GetWord(IDS_LESS).data(), Trackbar::BuddyTextAlignment::Left, font.GetHandle());
            trackbarTransparancy.SetBuddy(RECT{ 0, 0, 50, 20 }, Trackbar::BuddyLocation::Right,
                guilang.GetWord(IDS_MORE).data(), Trackbar::BuddyTextAlignment::Right, font.GetHandle());
            trackbarTransparancy.SetRange(0, 100);
            trackbarTransparancy.SetTicFrequency(10);
            trackbarTransparancy.SetTrackPosition(settingsCurrent.GetTransparancy());

            groupColor.Make(hWndOption, RECT{ 10, 139, 464, 188 }, -1);
            groupColor.SetFont(font.GetHandle());
            tiles.SetLeftTopCorner(POINT{ 21, 161 });
            tiles.SetCurrentColor(settingsCurrent.GetColorColorref(), settingsCurrent.GetColorPosition());

            groupMiscellaneous.Make(hWndOption, RECT{ 10, 332, 241, 169 }, -1);
            groupMiscellaneous.SetFont(font.GetHandle());
            groupMiscellaneous.SetText(guilang.GetWord(IDS_MISCELLANEOUS).data());
            label1.Make(hWndOption, IDL_SLANGUAGE, RECT{ 19, 356, 80, 20 });
            label2.Make(hWndOption, IDL_SSTYLE, RECT{ 19, 384, 80, 20 });
            label3.Make(hWndOption, IDL_SPRESSURE, RECT{ 19, 412, 80, 20 });
            label4.Make(hWndOption, IDL_SSPEED, RECT{ 19, 440, 80, 20 });
            label5.Make(hWndOption, IDL_STEMPERATURE, RECT{ 19, 468, 80, 20 });
            label1.SetFont(font.GetHandle());
            label2.SetFont(font.GetHandle());
            label3.SetFont(font.GetHandle());
            label4.SetFont(font.GetHandle());
            label5.SetFont(font.GetHandle());
            label1.SetText(guilang.GetWord(IDS_LANGUAGE).data());
            label2.SetText(guilang.GetWord(IDS_STYLE).data());
            label3.SetText(guilang.GetWord(IDS_PRESSURE).data());
            label4.SetText(guilang.GetWord(IDS_SPEED).data());
            label5.SetText(guilang.GetWord(IDS_TEMPERATURE).data());
            comboboxLanguage.Make(hWndOption, RECT{ 100, 353, 140, 200 }, IDV_COMBOBOX_LANGUAGE,
                CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL);
            comboboxLanguage.SetFont(font.GetHandle());
            comboboxLanguage.SetFont(font.GetHandle());
            comboboxLanguage.Add(guilang.GetWord(IDS_ENGLISH).data());
            comboboxLanguage.Add(guilang.GetWord(IDS_RUSSIAN).data());
            comboboxLanguage.Add(guilang.GetWord(IDS_UKRAINIAN).data());
            comboboxLanguage.SetSelection(int(guilang.GetLanguageUnit()));
            comboboxStyle.Make(hWndOption, RECT{ 100, 381, 140, 200 }, IDV_COMBOBOX_STYLE,
                CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL);
            comboboxStyle.SetFont(font.GetHandle());
            comboboxStyle.SetFont(font.GetHandle());
            comboboxStyle.Add(guilang.GetWord(IDS_STYLE1).data());
            comboboxStyle.Add(guilang.GetWord(IDS_STYLE2).data());
            comboboxStyle.Add(guilang.GetWord(IDS_STYLE3).data());
            comboboxStyle.Add(guilang.GetWord(IDS_STYLE4).data());
            comboboxStyle.SetSelection((settingsCurrent.GetStyle() - IDP_PNG128_STYLE1_00) / 100);
            comboboxPressure.Make(hWndOption, RECT{ 100, 409, 140, 200 }, IDV_COMBOBOX_PRESSURE,
                CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL);
            comboboxPressure.SetFont(font.GetHandle());
            comboboxPressure.Add(guilang.GetWord(IDS_PRESSURE1).data());
            comboboxPressure.Add(guilang.GetWord(IDS_PRESSURE2).data());
            comboboxPressure.Add(guilang.GetWord(IDS_PRESSURE3).data());
            comboboxPressure.Add(guilang.GetWord(IDS_PRESSURE4).data());
            comboboxPressure.SetSelection(guilang.GetPressureUnit());
            comboboxSpeed.Make(hWndOption, RECT{ 100, 437, 140, 200 }, IDV_COMBOBOX_SPEED,
                CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL);
            comboboxSpeed.SetFont(font.GetHandle());
            comboboxSpeed.Add(guilang.GetWord(IDS_SPEED1).data());
            comboboxSpeed.Add(guilang.GetWord(IDS_SPEED2).data());
            comboboxSpeed.Add(guilang.GetWord(IDS_SPEED3).data());
            comboboxSpeed.Add(guilang.GetWord(IDS_SPEED4).data());
            comboboxSpeed.SetSelection(guilang.GetSpeedUnit());
            comboboxTemperature.Make(hWndOption, RECT{ 100, 465, 140, 200 }, IDV_COMBOBOX_TEMPERATURE,
                CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL);
            comboboxTemperature.SetFont(font.GetHandle());
            comboboxTemperature.Add(guilang.GetWord(IDS_TEMPERATURE1).data());
            comboboxTemperature.Add(guilang.GetWord(IDS_TEMPERATURE2).data());
            comboboxTemperature.SetSelection(guilang.GetTemperatureUnit());

            groupOther.Make(hWndOption, RECT{ 265, 332, 208, 169 }, -1);
            groupOther.SetText(guilang.GetWord(IDS_OTHER).data());
            groupOther.SetFont(font.GetHandle());
            checkboxAutorun.Make(hWndOption, RECT{ 275, 353, 140, 20 }, IDV_CHECKBOX_AUTORUN);
            checkboxForecast.Make(hWndOption, RECT{ 275, 381, 140, 20 }, IDV_CHECKBOX_FORECAST);
            checkboxPin.Make(hWndOption, RECT{ 275, 409, 140, 20 }, IDV_CHECKBOX_PIN);
            checkboxSound.Make(hWndOption, RECT{ 275, 437, 140, 20 }, IDV_CHECKBOX_SOUND);
            checkboxRound.Make(hWndOption, RECT{ 275, 465, 140, 20 }, IDV_CHECKBOX_ROUND);
            checkboxAutorun.SetFont(font.GetHandle());
            checkboxForecast.SetFont(font.GetHandle());
            checkboxPin.SetFont(font.GetHandle());
            checkboxSound.SetFont(font.GetHandle());
            checkboxRound.SetFont(font.GetHandle());
            checkboxAutorun.SetText(guilang.GetWord(IDS_AUTORUN).data());
            checkboxForecast.SetText(guilang.GetWord(IDS_FORECAST).data());
            checkboxPin.SetText(guilang.GetWord(IDS_PIN).data());
            checkboxSound.SetText(guilang.GetWord(IDS_SOUND).data());
            checkboxRound.SetText(guilang.GetWord(IDS_ROUND).data());
            checkboxAutorun.SetChecked(settingsCurrent.GetAutorunFlag());
            checkboxForecast.SetChecked(settingsCurrent.GetForecastFlag());
            checkboxPin.SetChecked(settingsCurrent.GetPinFlag());
            checkboxSound.SetChecked(settingsCurrent.GetSoundFlag());
            checkboxRound.SetChecked(settingsCurrent.GetRoundFlag());

            buttonSave.Make(hWndOption, RECT{ 195, 512, 80, 27 }, IDV_BUTTON_SAVE);
            buttonSave.SetText(guilang.GetWord(IDS_SAVE).data());
            buttonSave.SetFont(font.GetHandle());
            buttonCancel.Make(hWndOption, RECT{ 295, 512, 80, 27 }, IDV_BUTTON_CANCEL);
            buttonCancel.SetText(guilang.GetWord(IDS_CANCEL).data());
            buttonCancel.SetFont(font.GetHandle());
            buttonApply.Make(hWndOption, RECT{ 395, 512, 80, 27 }, IDV_BUTTON_APPLY);
            buttonApply.SetText(guilang.GetWord(IDS_APPLY).data());
            buttonApply.SetFont(font.GetHandle());
            buttonApply.SetEnabled(false);

            settingsNew = settingsCurrent;
        }
        return TRUE;

        case WM_PAINT:
        {
            tiles.DrawTiles(hWndOption, RECT{ 0, 0, 442, 156 }, BOOL(wParam));
            groupColor.SetText((guilang.GetWord(IDS_COLOR) + tiles.GetColorDescription()).data());
        }
        return TRUE;

        case WM_HSCROLL:
        {
            switch (LOWORD(wParam))
            {
                case TB_LINEUP:
                case TB_LINEDOWN:
                case TB_PAGEUP:
                case TB_PAGEDOWN:
                case TB_THUMBPOSITION:
                case TB_THUMBTRACK:
                case TB_TOP:
                case TB_BOTTOM:
                case TB_ENDTRACK:
                {
                    int value = trackbarTransparancy.GetTrackPosition();
                    tstring transpStr((guilang.GetWord(IDS_TRANSPARANCY).data() + to_tstring(value)) += TEXT("%"));
                    groupTransparancy.SetText(transpStr.data());
                    settingsNew.SetTransparancy(value);
                    buttonApply.SetEnabled(true);
                    SendMessage(HWND(GetWindowLong(hWndOption, GWL_HWNDPARENT)),
                        WMU_UPDATE_TRANSPARANCY, WPARAM(settingsNew.GetTransparancy()), 0);
                }
            }
        }
        return TRUE;

        case WM_LBUTTONDOWN:
        {
            if (tiles.OnClick(hWndOption, lParam))
            {
                settingsNew.SetColorColorref(tiles.GetCurrentColorColorref());
                settingsNew.SetColorPosition(tiles.GetCurrentColorPosition());
                buttonApply.SetEnabled(true);
            }
        }
        return TRUE;

        case WM_COMMAND:
        {
            UINT wpId = LOWORD(wParam);
            UINT wpEvent = HIWORD(wParam);

            switch (wpId)
            {
                // --- START PROCEED COMBOBOXES
                case IDV_COMBOBOX_PLACE:
                {
                    if (wpEvent == CBN_SELCHANGE)
                    {
                        AbstractPlaces* places = AccuPlaces::Instance();    //YahooPlaces::Instance()
                        tstring selectString = comboboxPlace.GetText();
                        int selectIndex = comboboxPlace.GetSelection();

                        settingsNew.SetPlace(conv((places->begin() + selectIndex)->place));
                        settingsNew.SetWoeid(conv((places->begin() + selectIndex)->key));

                        buttonApply.SetEnabled(true);
                    }
                }
                return TRUE;

                case IDV_COMBOBOX_LANGUAGE:
                {
                    if (wpEvent == CBN_SELCHANGE)
                    {
                        settingsNew.SetLanguage(comboboxLanguage.GetSelection());
                        buttonApply.SetEnabled(true);
                    }
                }
                return TRUE;

                case IDV_COMBOBOX_STYLE:
                {
                    if (wpEvent == CBN_SELCHANGE)
                    {
                        settingsNew.SetStyle(comboboxStyle.GetSelection() * 100 + IDP_PNG128_STYLE1_00);
                        buttonApply.SetEnabled(true);
                    }
                }
                return TRUE;

                case IDV_COMBOBOX_PRESSURE:
                {
                    if (wpEvent == CBN_SELCHANGE)
                    {
                        settingsNew.SetPressure(comboboxPressure.GetSelection());
                        buttonApply.SetEnabled(true);
                    }
                }
                return TRUE;

                case IDV_COMBOBOX_SPEED:
                {
                    if (wpEvent == CBN_SELCHANGE)
                    {
                        settingsNew.SetSpeed(comboboxSpeed.GetSelection());
                        buttonApply.SetEnabled(true);
                    }
                }
                return TRUE;

                case IDV_COMBOBOX_TEMPERATURE:
                {
                    if (wpEvent == CBN_SELCHANGE)
                    {
                        settingsNew.SetTemperature(comboboxTemperature.GetSelection());
                        buttonApply.SetEnabled(true);
                    }
                }
                return TRUE;
                // --- END PROCEED COMBOBOXES

                // --- START PROCEED CHECKBOXES
                case IDV_CHECKBOX_AUTORUN:
                {
                    settingsNew.SetAutorunFlag(checkboxAutorun.IsChecked());
                    buttonApply.SetEnabled(true);
                }
                return TRUE;

                case IDV_CHECKBOX_FORECAST:
                {
                    settingsNew.SetForecastFlag(checkboxForecast.IsChecked());
                    buttonApply.SetEnabled(true);
                }
                return TRUE;

                case IDV_CHECKBOX_PIN:
                {
                    settingsNew.SetPinFlag(checkboxPin.IsChecked());
                    buttonApply.SetEnabled(true);
                }
                return TRUE;

                case IDV_CHECKBOX_SOUND:
                {
                    settingsNew.SetSoundFlag(checkboxSound.IsChecked());
                    buttonApply.SetEnabled(true);
                }
                return TRUE;

                case IDV_CHECKBOX_ROUND:
                {
                    settingsNew.SetRoundFlag(checkboxRound.IsChecked());
                    buttonApply.SetEnabled(true);
                }
                return TRUE;
                // --- END PROCEED CHECKBOXES


                case IDV_BUTTON_SEARCH:
                {
                    comboboxPlace.Clear();

                    tstring in = comboboxPlace.GetText();
                    std::string inn = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(in);
                    try
                    {
                        AbstractPlaces* places = AccuPlaces::Instance();    // YahooPlaces::Instance()
                        places->Update(inn);
                        for (const auto& e : *places)
                        {
                            comboboxPlace.Add(conv(e.place).data());
                        }
                        comboboxPlace.ShowDropdown();
                    }
                    catch (std::exception& e)
                    {
                        (void)e;
                        MessageBox(hWndOption, (in += TEXT(" not found")).data(), TEXT("Info"), MB_OK);
                    }
                }
                return TRUE;

                case IDV_BUTTON_SAVE:
                case IDV_BUTTON_APPLY:
                {
                    if (settingsCurrent != settingsNew)
                    {
                        settingsCurrent = settingsNew;
                    }
                    EndDialog(hWndOption, wpId);
                }
                return TRUE;

                case IDCANCEL:
                case IDV_BUTTON_CANCEL:
                {
                    EndDialog(hWndOption, IDV_BUTTON_CANCEL);
                }
                return TRUE;
            }
            return TRUE;
        }
    }
    return FALSE;
}
