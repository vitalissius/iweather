#include "Settings.h"

std::mutex Settings::ms_mutex;

const tstring& Settings::GetPlace() const
{
    return m_place;
}

const tstring& Settings::GetKey() const
{
    return m_woeid;
}

const int Settings::GetLanguage() const
{
    return m_language;
}

const long Settings::GetTransparancy() const
{
    return m_transparency;
}

const COLORREF Settings::GetColorColorref() const
{
    return m_color.colorref;
}

const POINT& Settings::GetColorPosition() const
{
    return m_color.position;
}

const bool Settings::GetAutorunFlag() const
{
    return m_autorun ? true : false;
}

const bool Settings::GetSoundFlag() const
{
    return m_sound ? true : false;
}

const bool Settings::GetPinFlag() const
{
    return m_pin ? true : false;
}

const bool Settings::GetForecastFlag() const
{
    return m_forecast ? true : false;
}

const bool Settings::GetRoundFlag() const
{
    return m_round ? true : false;
}

const int Settings::GetStyle() const
{
    return m_style;
}

const int Settings::GetPressure() const
{
    return m_pressure;
}

const int Settings::GetSpeed() const
{
    return m_speed;
}

const int Settings::GetTemperature() const
{
    return m_temperature;
}

const POINTS& Settings::GetWindowPosition() const
{
    return m_windowPosition;
}

void Settings::SetPlace(const tstring& placename)
{
    m_place = placename;
}

void Settings::SetWoeid(const tstring& placewoeid)
{
    m_woeid = placewoeid;
}

void Settings::SetLanguage(const int language)
{
    m_language = language;
}

void Settings::SetTransparancy(const long transparancy)
{
    m_transparency = transparancy;
}

void Settings::SetColorColorref(const COLORREF colorref)
{
    m_color.colorref = colorref;
}

void Settings::SetColorPosition(const POINT position)
{
    m_color.position = position;
}

void Settings::SetAutorunFlag(const int autorun)
{
    m_autorun = autorun;
}

void Settings::SetSoundFlag(const int sound)
{
    m_sound = sound;
}

void Settings::SetPinFlag(const int pin)
{
    m_pin = pin;
}

void Settings::SetForecastFlag(const int forecast)
{
    m_forecast = forecast;
}

void Settings::SetRoundFlag(const int round)
{
    m_round = round;
}

void Settings::SetStyle(const int style)
{
    m_style = style;
}

void Settings::SetPressure(const int pressure)
{
    m_pressure = pressure;
}

void Settings::SetSpeed(const int speed)
{
    m_speed = speed;
}

void Settings::SetTemperature(const int temperature)
{
    m_temperature = temperature;
}

void Settings::SetWindowPosition(const HWND hWnd)
{
    RECT rc;
    GetWindowRect(hWnd, &rc);
    POINTS position;
    position.x = short(rc.left);
    position.y = short(rc.top);
    m_windowPosition = position;
}

Settings::Settings(const Settings& rhs)
{
    m_place = rhs.m_place;
    m_woeid = rhs.m_woeid;
    m_language = rhs.m_language;
    m_transparency = rhs.m_transparency;
    m_color.colorref = rhs.m_color.colorref;
    m_color.position.x = rhs.m_color.position.x;
    m_color.position.y = rhs.m_color.position.y;
    m_autorun = rhs.m_autorun;
    m_sound = rhs.m_sound;
    m_pin = rhs.m_pin;
    m_forecast = rhs.m_forecast;
    m_round = rhs.m_round;
    m_style = rhs.m_style;
    m_pressure = rhs.m_pressure;
    m_speed = rhs.m_speed;
    m_temperature = rhs.m_temperature;
    m_windowPosition = rhs.m_windowPosition;
}

Settings& Settings::operator=(const Settings& rhs)
{
    Settings tmp(rhs);
    Swap(*this, tmp);
    return *this;
}

void Settings::Swap(Settings& s1, Settings& s2)
{
    std::swap(s1.m_place, s2.m_place);
    std::swap(s1.m_woeid, s2.m_woeid);
    std::swap(s1.m_language, s2.m_language);
    std::swap(s1.m_transparency, s2.m_transparency);
    std::swap(s1.m_color.colorref, s2.m_color.colorref);
    std::swap(s1.m_color.position.x, s2.m_color.position.x);
    std::swap(s1.m_color.position.y, s2.m_color.position.y);
    std::swap(s1.m_autorun, s2.m_autorun);
    std::swap(s1.m_sound, s2.m_sound);
    std::swap(s1.m_pin, s2.m_pin);
    std::swap(s1.m_forecast, s2.m_forecast);
    std::swap(s1.m_round, s2.m_round);
    std::swap(s1.m_style, s2.m_style);
    std::swap(s1.m_pressure, s2.m_pressure);
    std::swap(s1.m_speed, s2.m_speed);
    std::swap(s1.m_temperature, s2.m_temperature);
    std::swap(s1.m_windowPosition, s2.m_windowPosition);
}

bool Settings::operator!=(const Settings& rhs)
{
    return m_place != rhs.m_place
        || m_woeid != rhs.m_woeid
        || m_language != rhs.m_language
        || m_transparency != rhs.m_transparency
        || m_color.colorref != rhs.m_color.colorref
        || m_color.position.x != rhs.m_color.position.x
        || m_color.position.y != rhs.m_color.position.y
        || m_autorun != rhs.m_autorun
        || m_sound != rhs.m_sound
        || m_pin != rhs.m_pin
        || m_forecast != rhs.m_forecast
        || m_round != rhs.m_round
        || m_style != rhs.m_style
        || m_pressure != rhs.m_pressure
        || m_speed != rhs.m_speed
        || m_temperature != rhs.m_temperature
        || m_windowPosition.x != rhs.m_windowPosition.x
        || m_windowPosition.y != rhs.m_windowPosition.y;
}

void Settings::LoadSettings(const TCHAR* filename)
{
    if (!filename) { filename = TEXT("weather.ini"); }

    std::lock_guard<std::mutex> guard(ms_mutex);

    TCHAR path[MAX_PATH + 1];

    SHGetSpecialFolderPath(NULL, path, CSIDL_MYDOCUMENTS, TRUE);

    _stprintf_s(path + _tcslen(path), MAX_PATH - _tcslen(path), TEXT("\\"));
    _stprintf_s(path + _tcslen(path), MAX_PATH - _tcslen(path), filename);


    TCHAR buffer[MAX_PATH];

    GetPrivateProfileString(TEXT("Options"), TEXT("Place name"), TEXT(""), buffer, MAX_PATH, path);
    m_place = buffer;
    GetPrivateProfileString(TEXT("Options"), TEXT("Place woeid"), TEXT(""), buffer, MAX_PATH, path);
    m_woeid = buffer;
    m_language = GetPrivateProfileInt(TEXT("Options"), TEXT("Language"), 0, path);
    m_transparency = GetPrivateProfileInt(TEXT("Options"), TEXT("Transparency"), 0, path);
    m_color.colorref = GetPrivateProfileInt(TEXT("Options"), TEXT("Colorref"), 0, path);
    GetPrivateProfileString(TEXT("Options"), TEXT("Color position"), TEXT(""), buffer, MAX_PATH, path);
    tstringstream tss;
    tss << buffer;
    tss >> m_color.position.x;
    tss >> m_color.position.y;
    m_autorun = GetPrivateProfileInt(TEXT("Options"), TEXT("Autorun"), 0, path);// == 0 ? false : true;
    m_sound = GetPrivateProfileInt(TEXT("Options"), TEXT("Sound"), 0, path);// == 0 ? false : true;
    m_pin = GetPrivateProfileInt(TEXT("Options"), TEXT("Pin"), 0, path);// == 0 ? false : true;
    m_forecast = GetPrivateProfileInt(TEXT("Options"), TEXT("Forecast"), 1, path);// == 0 ? false : true;
    m_round = GetPrivateProfileInt(TEXT("Options"), TEXT("Round"), 0, path);// == 0 ? false : true;
    m_style = GetPrivateProfileInt(TEXT("Options"), TEXT("Style"), 500, path);// == 0 ? false : true;
    m_pressure = GetPrivateProfileInt(TEXT("Options"), TEXT("Pressure"), 0, path);// == 0 ? false : true;
    m_speed = GetPrivateProfileInt(TEXT("Options"), TEXT("Speed"), 0, path);// == 0 ? false : true;
    m_temperature = GetPrivateProfileInt(TEXT("Options"), TEXT("Temperature"), 0, path);// == 0 ? false : true;
    GetPrivateProfileString(TEXT("Options"), TEXT("Window position"), TEXT(""), buffer, MAX_PATH, path);
    tss.clear();
    tss << buffer;
    tss >> m_windowPosition.x;
    tss >> m_windowPosition.y;
}

void Settings::SaveSettings(const TCHAR* filename)
{
    if (!filename) { filename = TEXT("weather.ini"); }

    std::lock_guard<std::mutex> guard(ms_mutex);

    TCHAR path[MAX_PATH + 1];
    SHGetSpecialFolderPath(NULL, path, CSIDL_MYDOCUMENTS, TRUE);

    _stprintf_s(path + _tcslen(path), MAX_PATH - _tcslen(path), TEXT("\\"));
    _stprintf_s(path + _tcslen(path), MAX_PATH - _tcslen(path), filename);

    tstring tmpcp = to_tstring(m_color.position.x) + TEXT(" ") + to_tstring(m_color.position.y);
    tstring tmpwp = to_tstring(m_windowPosition.x) + TEXT(" ") + to_tstring(m_windowPosition.y);

    WritePrivateProfileString(TEXT("Options"), TEXT("Place name"), m_place.data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Place woeid"), m_woeid.data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Language"), to_tstring(m_language).data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Transparency"), to_tstring(m_transparency).data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Colorref"), to_tstring(m_color.colorref).data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Color position"), tmpcp.data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Autorun"), to_tstring(m_autorun).data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Sound"), to_tstring(m_sound).data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Pin"), to_tstring(m_pin).data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Forecast"), to_tstring(m_forecast).data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Round"), to_tstring(m_round).data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Style"), to_tstring(m_style).data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Pressure"), to_tstring(m_pressure).data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Speed"), to_tstring(m_speed).data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Temperature"), to_tstring(m_temperature).data(), path);
    WritePrivateProfileString(TEXT("Options"), TEXT("Window position"), tmpwp.data(), path);

    setAutorunRegkey(m_autorun);
}

void Settings::setAutorunRegkey(const int autorun)
{
    HKEY hkey;
    static const TCHAR subKey[]{ TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run") };
    static const DWORD opt = REG_OPTION_NON_VOLATILE;
    RegCreateKeyEx(HKEY_CURRENT_USER, subKey, NULL, TEXT(""), opt, KEY_SET_VALUE, NULL, &hkey, NULL);

    if (hkey)
    {
        if (autorun)
        {
            TCHAR szPath[MAX_PATH + 1];
            GetModuleFileName(NULL, szPath, _countof(szPath));
            RegSetValueEx(hkey, TEXT("aWeather"), NULL, REG_SZ, (LPBYTE)szPath, _countof(szPath));
            RegCloseKey(hkey);
        }
        else
        {
            RegDeleteValue(hkey, TEXT("aWeather"));
            RegCloseKey(hkey);
        }
    }
}
