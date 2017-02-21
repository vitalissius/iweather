#include "GuiLang.h"

GuiLang::Dict::Dict(tstring eng, tstring rus)
    : eng(std::move(eng))
    , rus(std::move(rus))
{}

const tstring& GuiLang::Dict::GetWord(LanguagePack::LanguageUnits langUnit) const
{
    if (langUnit == LanguagePack::LanguageUnits::ENG)
    {
        return eng;
    }
    else
    {
        return rus;
    }
}



LanguagePack::LanguageUnits GuiLang::GetLanguageUnit() const
{
    return m_langPack.GetLanguageUnit();
}

units::PressureUnits GuiLang::GetPressureUnit() const
{
    return m_langPack.GetPressureUnit();
}

units::SpeedUnits GuiLang::GetSpeedUnit() const
{
    return m_langPack.GetSpeedUnit();
}

units::TemperatureUnits GuiLang::GetTemperatureUnit() const
{
    return m_langPack.GetTemperatureUnit();
}

void GuiLang::SetLanguageUnit(LanguagePack::LanguageUnits languageUnit)
{
    m_langPack.SetLanguageUnit(languageUnit);
}

void GuiLang::SetPressureUnit(units::PressureUnits pressureUnit)
{
    m_langPack.SetPressureUnit(pressureUnit);
}

void GuiLang::SetSpeedUnit(units::SpeedUnits speedUnit)
{
    m_langPack.SetSpeedUnit(speedUnit);
}

void GuiLang::SetTemperatureUnit(units::TemperatureUnits temperatureUnit)
{
    m_langPack.SetTemperatureUnit(temperatureUnit);
}

const tstring& GuiLang::GetWord(const UINT id) const
{
    return ms_dict.at(id).GetWord(m_langPack.GetLanguageUnit());
}



const std::map<UINT, const GuiLang::Dict> GuiLang::ms_dict{
    { IDS_REFRESH, { TEXT("Refresh"), TEXT("��������") } },
    { IDS_PIN, { TEXT("Pin"), TEXT("���������") } },
    { IDS_FORECAST, { TEXT("Show forecast"), TEXT("���������� �������") } },
    { IDS_SETTINGS, { TEXT("Settings"), TEXT("���������") } },
    { IDS_CLOSE, { TEXT("Close"), TEXT("�������") } },
    { IDS_PLACE, { TEXT("Current place: "), TEXT("������� �����: ") } },
    { IDS_SEARCH, { TEXT("Search"), TEXT("������") } },
    { IDS_TRANSPARANCY, { TEXT("Transparancy: "), TEXT("������������: ") } },
    { IDS_LESS, { TEXT("Less"), TEXT("������") } },
    { IDS_MORE, { TEXT("More"), TEXT("������") } },
    { IDS_COLOR, { TEXT("Color: "), TEXT("����: ") } },
    { IDS_AUTORUN, { TEXT("Autorun"), TEXT("����������") } },
    { IDS_SOUND, { TEXT("Sound"), TEXT("����") } },
    { IDS_ROUND, { TEXT("Rounded window"), TEXT("����������� ����") } },
    { IDS_LANGUAGE, { TEXT("Language:"), TEXT("����:") } },
    { IDS_ENGLISH, { TEXT("English"), TEXT("���������") } },
    { IDS_RUSSIAN, { TEXT("Russian"), TEXT("�������") } },
    { IDS_SAVE, { TEXT("Save"), TEXT("���������") } },
    { IDS_CANCEL, { TEXT("Cancel"), TEXT("��������") } },
    { IDS_APPLY, { TEXT("Apply"), TEXT("���������") } },
    { IDS_INPUTPLACE, { TEXT("Input place..."), TEXT("������� �����...") } },
    { IDS_STYLE, { TEXT("Style:"), TEXT("�����:") } },
    { IDS_STYLE1, { TEXT("Style #1"), TEXT("����� �1") } },
    { IDS_STYLE2, { TEXT("Style #2"), TEXT("����� �2") } },
    { IDS_STYLE3, { TEXT("Style #3"), TEXT("����� �3") } },
    { IDS_STYLE4, { TEXT("Style #4"), TEXT("����� �4") } },
    { IDS_PRESSURE, { TEXT("Pressure:"), TEXT("��������:") } },
    { IDS_PRESSURE1, { TEXT("atm"), TEXT("���") } },
    { IDS_PRESSURE2, { TEXT("mmHg"), TEXT("�� ��.��.") } },
    { IDS_PRESSURE3, { TEXT("mb"), TEXT("����") } },
    { IDS_PRESSURE4, { TEXT("inHg"), TEXT("���� ��.��.") } },
    { IDS_SPEED, { TEXT("Speed:"), TEXT("��������:") } },
    { IDS_SPEED1, { TEXT("km/h"), TEXT("��/�") } },
    { IDS_SPEED2, { TEXT("m/s"), TEXT("�/�") } },
    { IDS_SPEED3, { TEXT("mph"), TEXT("����/�") } },
    { IDS_SPEED4, { TEXT("ft/s"), TEXT("��/�") } },
    { IDS_TEMPERATURE, { TEXT("Temperature:"), TEXT("�����������:") } },
    { IDS_TEMPERATURE1, { TEXT("Celsius"), TEXT("������ �������") } },
    { IDS_TEMPERATURE2, { TEXT("Fahrenheit"), TEXT("������ ����������") } },
    { IDS_MISCELLANEOUS, { TEXT("Miscellaneous:"), TEXT("������:") } },
    { IDS_OTHER, { TEXT("Other:"), TEXT("���������:") } },
    { IDS_DAY, { TEXT("Day: "), TEXT("����: ") } },
    { IDS_NIGHT, { TEXT("Night: "), TEXT("����: ") } },
};
