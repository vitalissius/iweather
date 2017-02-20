#pragma once

#include "../core/Utility.h"

#include <Windows.h>
#include <Shlobj.h>

#include <mutex>

class Settings {
public:
    const tstring& GetPlace() const;
    const tstring& GetKey() const;
    const int GetLanguage() const;
    const long GetTransparancy() const;
    const COLORREF GetColorColorref() const;
    const POINT& GetColorPosition() const;
    const bool GetAutorunFlag() const;
    const bool GetSoundFlag() const;
    const bool GetPinFlag() const;
    const bool GetForecastFlag() const;
    const bool GetRoundFlag() const;
    const int GetStyle() const;
    const int GetPressure() const;
    const int GetSpeed() const;
    const int GetTemperature() const;
    const POINTS& GetWindowPosition() const;
    void SetPlace(const tstring& placename);
    void SetWoeid(const tstring& placewoeid);
    void SetLanguage(const int language);
    void SetTransparancy(const long transparancy);
    void SetColorColorref(const COLORREF colorref);
    void SetColorPosition(const POINT position);
    void SetAutorunFlag(const int autorun);
    void SetSoundFlag(const int sound);
    void SetPinFlag(const int pin);
    void SetForecastFlag(const int forecast);
    void SetRoundFlag(const int round);
    void SetStyle(const int style);
    void SetPressure(const int pressure);
    void SetSpeed(const int speed);
    void SetTemperature(const int temperature);
    void SetWindowPosition(const HWND hWnd);

    Settings() = default;
    ~Settings() = default;
    Settings(const Settings& rhs);
    Settings& operator=(const Settings& rhs);
    void Swap(Settings& s1, Settings& s2);
    Settings(Settings&&) = default;
    Settings& operator=(Settings&&) = default;
    bool operator!=(const Settings& rhs);
    void LoadSettings(const TCHAR* filename = nullptr);
    void SaveSettings(const TCHAR* filename = nullptr);

private:
    void setAutorunRegkey(const int autorun);

private:
    tstring m_place{};
    tstring m_woeid{};
    int m_language{};
    long m_transparency{};
    struct {
        COLORREF colorref{};
        POINT position{};
    } m_color;
    int m_autorun{};
    int m_sound{};
    int m_pin{};
    int m_forecast{};
    int m_round{};
    int m_style{};
    int m_pressure{};
    int m_speed{};
    int m_temperature{};
    POINTS m_windowPosition{};

    static std::mutex ms_mutex;
};
