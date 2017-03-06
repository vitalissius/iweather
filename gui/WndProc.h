#pragma once

#include "../core/YahooPlaces.h"
#include "../core/AccuPlaces.h"
#include "../core/AccuWeather.h"
#include "../core/Utility.h"
#include "AppRegion.h"
#include "ContextMenu.h"
#include "GdiplusDrawer.h"
#include "GuiLang.h"
#include "ViewElement.h"
#include "IconNotifier.h"
#include "ViewTools.h"
#include "Resource.h"
#include "Tiles.h"
#include "Settings.h"
#include "Conv.h"
#include "WindowOperations.h"
#include "WndProcOptions.h"

#include <atomic>
#include <array>
#include <cassert>
#include <ctime>
#include <locale>
#include <memory>
#include <fstream>
#include <thread>
#include <mutex>

Settings settingsCurrent;

LRESULT WINAPI WndProc(HWND hWndProc, UINT uMessage, UINT wParam, LONG lParam)
{
    static const UINT WMU_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));

    static HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWndProc, GWL_HINSTANCE);

    static const int roundValue = 23;
    static AppRegion appRegion(hWndProc, 10, 32, POINT{ 236, 282 }, POINT{ 236, 382 });

    const TCHAR* const fontName = TEXT("Segoe UI Light");
    static Font fontTitle(26, 0, FW_DONTCARE, fontName);
    //static Font fontBig(160, 30, FW_DONTCARE, fontName);
    static Font fontBig(130, 30, FW_DONTCARE, fontName);
    static Font fontCommon(18, 0, FW_DONTCARE, fontName);

    static constexpr std::array<RECT, 13> labelPos{
        RECT{  10,  10, 250,  26 },
        RECT{ 104,  22, 130, 120 },
        RECT{  10, 158, 240,  20 },
        RECT{  34, 180, 100,  20 },
        RECT{  34, 202,  76,  20 },
        RECT{  34, 226,  40,  20 },
        RECT{ 176, 191,  60,  20 },
        RECT{ 176, 213,  60,  20 },
        RECT{   4, 252,  36,  88 },
        RECT{  50, 252,  36,  88 },
        RECT{  96, 252,  36,  88 },
        RECT{ 142, 252,  36,  88 },
        RECT{ 188, 252,  36,  88 },
    };

    static Label label00;
    static Label label01;
    static Label label02;
    static Label label03;
    static Label label04;
    static Label label05;
    static Label label06;
    static Label label07;
    static std::array<Label, 5> forecastLabels;

    static Brush brush;

    static const UINT SHOW_BORDER = 0;          // WS_BORDER

    static IconNotifier notifier(hWndProc, IDI_WEATHERAPPLICATION, WMU_NOTIFICATIONAREA, IconNotifier::Sound::ON);

    static WindowOperations windowOperations;

    static GdiplusDrawer drawler;

    static std::unique_ptr<AbstractWeather> weatherData = std::make_unique<AccuWeather>();  // YahooWeather
    static std::vector<int> weatherCodes(6, 48);    // 48 - Not Available Code

    static GuiLang guiLang;

    static POINTS position{};

    switch (uMessage)
    {
        case WM_CREATE:
        {
            SetTimer(hWndProc, IDT_TIMER_UPDATE, 1200000, NULL);

            settingsCurrent.LoadSettings();

            position = settingsCurrent.GetWindowPosition();
            SetWindowPos(hWndProc, HWND_TOP, position.x, position.y, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);

            // Region
            if (settingsCurrent.GetForecastFlag()) { appRegion.ShowMaxWindow(); }
            else { appRegion.ShowMinWindow(); }

            // Transparancy
            windowOperations.SetTransparency(hWndProc, settingsCurrent.GetTransparancy());
            // Color value
            brush.Make(settingsCurrent.GetColorColorref());
            // Sound
            notifier.SetSound(IconNotifier::Sound(settingsCurrent.GetSoundFlag()));
            // Forecast
            if (settingsCurrent.GetForecastFlag())
            {
                appRegion.ShowMaxWindow();
            }
            // Language
            guiLang.SetLanguageUnit(LanguagePack::LanguageUnits(settingsCurrent.GetLanguage()));
            guiLang.SetPressureUnit(units::PressureUnits(settingsCurrent.GetPressure()));
            guiLang.SetSpeedUnit(units::SpeedUnits(settingsCurrent.GetSpeed()));
            guiLang.SetTemperatureUnit(units::TemperatureUnits(settingsCurrent.GetTemperature()));
            // Round
            if (settingsCurrent.GetRoundFlag())
            {
                appRegion.MakeRounding(roundValue, roundValue);
            }

            // Apply
            appRegion.ApplyToWindow();

            label00.Make(hWndProc, IDL_PLACE, labelPos[0], Label::LabelType::WithTip, SHOW_BORDER);
            label01.Make(hWndProc, IDL_TEMP, labelPos[1], Label::LabelType::Simple, SS_CENTER | SHOW_BORDER);
            label02.Make(hWndProc, IDL_CONDITION, labelPos[2], Label::LabelType::Simple, SHOW_BORDER);
            label03.Make(hWndProc, IDL_PRESSURE, labelPos[3], Label::LabelType::WithTip, SHOW_BORDER);
            label04.Make(hWndProc, IDL_WIND, labelPos[4], Label::LabelType::WithTip, SHOW_BORDER);
            label04.SetMaxTipWidth(200);
            label05.Make(hWndProc, IDL_HUMIDITY, labelPos[5], Label::LabelType::Simple, SHOW_BORDER);
            label06.Make(hWndProc, IDL_SUNRISE, labelPos[6], Label::LabelType::Simple, SHOW_BORDER);
            label07.Make(hWndProc, IDL_SUNSET, labelPos[7], Label::LabelType::WithTip, SHOW_BORDER);
            for (size_t i = 0; i < forecastLabels.size(); ++i)
            {
                forecastLabels[i].Make(hWndProc, IDL_DAY1 + i, labelPos[8 + i], Label::LabelType::WithTip, SS_CENTER);
            }

            PostMessage(hWndProc, WMU_UPDATE_DATA, 0, 0);
        }
        break;

        case WM_PAINT:
        {
            drawler.Add(IDP_PNG24_PRESSURE, POINT{ 10, 178 }, 1.f);
            drawler.Add(IDP_PNG24_WIND, POINT{ 10, 200 }, 1.f);
            drawler.Add(IDP_PNG24_HUMIDITY, POINT{ 10, 224 }, 1.f);
            drawler.Add(IDP_PNG24_SUNRISE, POINT{ 152, 189 }, 1.f);
            drawler.Add(IDP_PNG24_SUNSET, POINT{ 152, 211 }, 1.f);

            UINT iconStyle = settingsCurrent.GetStyle();
            drawler.Add(iconStyle + weatherCodes.at(0), POINT{ 3, 36 }, 1.f);
            drawler.Add(iconStyle + weatherCodes.at(1), POINT{ 4, 268 }, 0.3f);
            drawler.Add(iconStyle + weatherCodes.at(2), POINT{ 50, 268 }, 0.3f);
            drawler.Add(iconStyle + weatherCodes.at(3), POINT{ 96, 268 }, 0.3f);
            drawler.Add(iconStyle + weatherCodes.at(4), POINT{ 142, 268 }, 0.3f);
            drawler.Add(iconStyle + weatherCodes.at(5), POINT{ 188, 268 }, 0.3f);

            drawler.Draw(hWndProc, brush.GetHandle());
        }
        break;

        case WMU_NOTIFICATIONAREA:
        {
            switch (lParam)
            {
                case WM_LBUTTONDBLCLK:
                    windowOperations.MinimizeOrRestore(hWndProc);
                    break;

                case WM_LBUTTONDOWN:
                    windowOperations.ForegroundWindow(hWndProc);
                    break;

                case WM_RBUTTONDOWN:
                    notifier.ShowMenu(true, true);
                    break;
            }
        }
        break;

        case WMU_UPDATE_DATA:
        {
            struct flag_locker {
                std::atomic<bool>& flag;

                explicit flag_locker(std::atomic<bool>& f) : flag(f) {}
                ~flag_locker() { flag = false; }
                bool try_lock()
                {
                    if (!flag)
                        return (flag = true);
                    return false;
                }
                flag_locker(const flag_locker&) = delete;
                flag_locker& operator=(const flag_locker&) = delete;
            };

            static auto update = [](HWND hWnd, std::unique_ptr<AbstractWeather>& pWeather) -> void
            {
                static std::atomic<bool> isrun;
                flag_locker flg(isrun);

                if (flg.try_lock())
                {
                    try
                    {
                        pWeather->Update(conv(settingsCurrent.GetKey()));
                        PostMessage(hWnd, WMU_UPDATE_VIEW, WPARAM(0), LPARAM(0));
                    }
                    catch (std::exception& /*e*/)
                    {
                        /*MessageBox(hWnd, Widen(e.what()).data(), TEXT("Exception in thread"), MB_OK);*/
                    }
                }
            };

            std::thread thr(update, hWndProc, std::ref(weatherData));
            thr.detach();
        }
        break;

        case WMU_UPDATE_VIEW:
        {
            try {
                std::string lineLocation = weatherData->GetLocationCcLine();

                std::string linePubDate = weatherData->GetPublicationDateLine();

                std::string lineCondTemp = std::to_string(weatherData->GetConditionTemt()) + "°";

                std::string lineCondText = weatherData->GetConditionText();
                std::transform(lineCondText.begin(), lineCondText.end(), lineCondText.begin(), [](char c)
                {
                    return std::toupper(c);
                });

                std::string linePressure = weatherData->GetPressureLine();
                std::string linePressureState = weatherData->GetPressureStateLine();

                std::string lineWindSpeed = weatherData->GetWindSpeedLine();
                std::string lineWindTitle = weatherData->GetWindDescriptionLine();
                lineWindTitle += ", ";
                lineWindTitle += weatherData->GetWindDirectionLine();
                std::string lineWindText = weatherData->GetWindLandConditionsLine();

                std::string lineHumidity = weatherData->GetHumidityLine();

                std::string lineSunrise = weatherData->GetSunriseLine();

                std::string lineSunset = weatherData->GetSunsetLine();

                std::vector<std::tuple<std::string, std::string, std::string>> lines;
                const auto& forecastVectorRef = weatherData->GetForecastVector();
                for (const auto& forecast : forecastVectorRef)
                {
                    std::string tmp = forecast.GetDayName();
                    std::transform(tmp.begin(), tmp.end(), tmp.begin(), std::toupper);
                    tmp += "\n\n\n";
                    tmp += std::to_string(forecast.GetHighTemp()) + "°\n";
                    tmp += std::to_string(forecast.GetLowTemp()) + "°";
                    lines.emplace_back(tmp, forecast.GetDate(), forecast.GetDescriptionText());
                }

                InitLabelText(label00, fontTitle, widen(lineLocation), widen(linePubDate));
                InitLabelText(label01, fontBig, widen(lineCondTemp));
                InitLabelText(label02, fontCommon, widen(lineCondText));
                InitLabelText(label03, fontCommon, widen(linePressure), widen(linePressureState));
                InitLabelText(label04, fontCommon, widen(lineWindSpeed), widen(lineWindText), widen(lineWindTitle));
                InitLabelText(label05, fontCommon, widen(lineHumidity));
                InitLabelText(label06, fontCommon, widen(lineSunrise));
                InitLabelText(label07, fontCommon, widen(lineSunset));

                static const auto labelText = [](const decltype(lines)& lines, size_t pos)
                {
                    return widen(std::get<0>(lines[pos]));
                };
                static const auto tipText = [](const decltype(lines)& lines, size_t pos)
                {
                    return widen(std::get<1>(lines[pos]));
                };
                static const auto tipTitleText = [](const decltype(lines)& lines, size_t pos)
                {
                    return widen(std::get<2>(lines[pos]));
                };
                for (size_t i = 0; i < forecastLabels.size(); ++i)
                {
                    InitLabelText(forecastLabels[i], fontCommon,
                        labelText(lines, i), tipText(lines, i), tipTitleText(lines, i));
                }

                weatherCodes.clear();
                // Update codes (weatherCodes used for redraw pictures in main window):
                weatherCodes.push_back(weatherData->GetConditionCode());        // current condition code
                for (size_t i = 0; i < forecastVectorRef.size(); ++i)
                {
                    weatherCodes.push_back(forecastVectorRef[i].GetCode());     // 5-day forecast codes
                }

                if (settingsCurrent.GetForecastFlag())
                {
                    appRegion.ShowMaxWindow();
                }
                else
                {
                    appRegion.ShowMinWindow();
                }

                tstring textInfo = widen(weatherData->GetConditionText());
                tstring textInfoTitle = widen(std::to_string(weatherData->GetConditionTemt()));
                textInfoTitle += TEXT("°");
                tstring textTip = textInfoTitle;
                textTip += TEXT(", ");
                textTip += textInfo;
                notifier.Modify(textInfo.data(), textInfoTitle.data(), textTip.data(),
                    weatherCodes[0] + IDI_ICO48_STYLE1_00 - IDP_PNG128_STYLE1_00 + settingsCurrent.GetStyle());
            }
            catch (std::exception& e)
            {
                label00.SetText(widen(e.what()).data());
            }

            InvalidateRect(hWndProc, NULL, TRUE);
        }
        break;

        case WMU_UPDATE_TRANSPARANCY:
        {
            windowOperations.SetTransparency(hWndProc, int(wParam));
        }
        break;

        case WMU_UPDATE_COLORREF:
        {
            brush.UpdateColor(COLORREF(lParam));
            InvalidateRect(hWndProc, NULL, FALSE);
        }
        break;

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDM_MENU_REFRESH:
                    PostMessage(hWndProc, WMU_UPDATE_DATA, WPARAM(0), LPARAM(0));
                    break;

                case IDM_MENU_PIN:
                    settingsCurrent.SetPinFlag(!settingsCurrent.GetPinFlag());
                    settingsCurrent.SaveSettings();
                    break;

                case IDM_MENU_FORECAST:
                    settingsCurrent.SetForecastFlag(!settingsCurrent.GetForecastFlag());
                    settingsCurrent.SaveSettings();
                    PostMessage(hWndProc, WMU_UPDATE_VIEW, WPARAM(NULL), LPARAM(NULL));
                    break;

                case IDM_MENU_OPTION:
                {
                    auto ret = DialogBox(hInstance, TEXT("IDD_DIALOG_OPTIONS"), hWndProc, &WindowProcOptionDlg);
                    if (ret == IDV_BUTTON_SAVE || ret == IDV_BUTTON_APPLY)
                    {
                        // Sound
                        notifier.SetSound(IconNotifier::Sound(settingsCurrent.GetSoundFlag()));

                        // Forecast
                        if (settingsCurrent.GetForecastFlag())
                        {
                            appRegion.ShowMaxWindow();
                        }

                        // Language
                        guiLang.SetLanguageUnit(LanguagePack::LanguageUnits(settingsCurrent.GetLanguage()));
                        guiLang.SetPressureUnit(units::PressureUnits(settingsCurrent.GetPressure()));
                        guiLang.SetSpeedUnit(units::SpeedUnits(settingsCurrent.GetSpeed()));
                        guiLang.SetTemperatureUnit(units::TemperatureUnits(settingsCurrent.GetTemperature()));

                        // Round
                        int rv = settingsCurrent.GetRoundFlag() ? roundValue : 0;
                        appRegion.MakeRounding(rv, rv);
                        appRegion.ApplyToWindow();

                        settingsCurrent.SaveSettings();

                        if (ret == IDV_BUTTON_APPLY)
                        {
                            PostMessage(hWndProc, WM_COMMAND, IDM_MENU_OPTION, 0);
                        }

                        PostMessage(hWndProc, WMU_UPDATE_DATA, WPARAM(0), LPARAM(0));
                    }
                    else if (ret == IDV_BUTTON_CANCEL)
                    {
                        // Если нажали Esc или кнопку Cancel, то восстановить возможные изменения
                        // прозрачности и цвета окна (величины, изменение которых в диалоговом
                        // окне сразу же влияют на вид главного окна)
                        windowOperations.SetTransparency(hWndProc, settingsCurrent.GetTransparancy());
                        brush.UpdateColor(settingsCurrent.GetColorColorref());
                        InvalidateRect(hWndProc, NULL, FALSE);
                    }
                    else
                    {
                        break;
                    }
                }
                break;

                case IDM_MENU_CLOSE:
                    SendMessage(hWndProc, WM_CLOSE, WPARAM(0), LPARAM(0));
                    break;
            }
        }
        break;

        case WM_CONTEXTMENU:
        {
            ContextMenu cmenu(hWndProc);
            cmenu.Show(settingsCurrent.GetPinFlag(), settingsCurrent.GetForecastFlag());
        }
        break;

        case WM_CTLCOLORSTATIC:
        {
            SetBkMode((HDC)wParam, TRANSPARENT);
            SetTextColor((HDC)wParam, RGB(255, 255, 255));
            return (LONG)GetStockObject(NULL_BRUSH);
        }

        case WM_LBUTTONDOWN:
        {
            if (!settingsCurrent.GetPinFlag())
            {
                windowOperations.Capture(hWndProc, lParam);
            }
        }
        break;

        case WM_LBUTTONUP:
        {
            if (!settingsCurrent.GetPinFlag())
            {
                windowOperations.Release();
                settingsCurrent.SetWindowPosition(hWndProc);
            }
        }
        break;

        case WM_MOUSEMOVE:
        {
            if (!settingsCurrent.GetPinFlag())
            {
                windowOperations.MoveWnd(hWndProc, wParam, lParam);
            }
        }
        break;

        case WM_TIMER:
        {
            SendMessage(hWndProc, WMU_UPDATE_DATA, WPARAM(NULL), LPARAM(NULL));
        }
        break;

        case WM_CLOSE:
        {
            settingsCurrent.SaveSettings();
            KillTimer(hWndProc, IDT_TIMER_UPDATE);
            DestroyWindow(hWndProc);
        }
        break;

        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;

        default:
        {
            if (uMessage == WMU_TASKBARCREATED)
            {
                notifier.Make(hWndProc, IDI_WEATHERAPPLICATION, WMU_NOTIFICATIONAREA,
                    IconNotifier::Sound(settingsCurrent.GetSoundFlag()));
                SendMessage(hWndProc, WMU_UPDATE_DATA, wParam, lParam);
            }
        }
        break;
    }

    return DefWindowProc(hWndProc, uMessage, wParam, lParam);
}
