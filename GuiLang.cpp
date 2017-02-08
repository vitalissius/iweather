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
    { IDS_REFRESH, { TEXT("Refresh"), TEXT("Обновить") } },
    { IDS_PIN, { TEXT("Pin"), TEXT("Закрепить") } },
    { IDS_FORECAST, { TEXT("Show forecast"), TEXT("Показывать прогноз") } },
    { IDS_SETTINGS, { TEXT("Settings"), TEXT("Настройки") } },
    { IDS_CLOSE, { TEXT("Close"), TEXT("Закрыть") } },
    { IDS_PLACE, { TEXT("Current place: "), TEXT("Текущее место: ") } },
    { IDS_SEARCH, { TEXT("Search"), TEXT("Искать") } },
    { IDS_TRANSPARANCY, { TEXT("Transparancy: "), TEXT("Прозрачность: ") } },
    { IDS_LESS, { TEXT("Less"), TEXT("Меньше") } },
    { IDS_MORE, { TEXT("More"), TEXT("Больше") } },
    { IDS_COLOR, { TEXT("Color: "), TEXT("Цвет: ") } },
    { IDS_AUTORUN, { TEXT("Autorun"), TEXT("Автозапуск") } },
    { IDS_SOUND, { TEXT("Sound"), TEXT("Звук") } },
    { IDS_ROUND, { TEXT("Rounded window"), TEXT("Закруглённое окно") } },
    { IDS_LANGUAGE, { TEXT("Language:"), TEXT("Язык:") } },
    { IDS_ENGLISH, { TEXT("English"), TEXT("Английкий") } },
    { IDS_RUSSIAN, { TEXT("Russian"), TEXT("Русский") } },
    { IDS_SAVE, { TEXT("Save"), TEXT("Сохранить") } },
    { IDS_CANCEL, { TEXT("Cancel"), TEXT("Отменить") } },
    { IDS_APPLY, { TEXT("Apply"), TEXT("Применить") } },
    { IDS_INPUTPLACE, { TEXT("Input place..."), TEXT("Введите место...") } },
    { IDS_STYLE, { TEXT("Style:"), TEXT("Стиль:") } },
    { IDS_STYLE1, { TEXT("Style #1"), TEXT("Стиль №1") } },
    { IDS_STYLE2, { TEXT("Style #2"), TEXT("Стиль №2") } },
    { IDS_STYLE3, { TEXT("Style #3"), TEXT("Стиль №3") } },
    { IDS_STYLE4, { TEXT("Style #4"), TEXT("Стиль №4") } },
    { IDS_PRESSURE, { TEXT("Pressure:"), TEXT("Давление:") } },
    { IDS_PRESSURE1, { TEXT("atm"), TEXT("атм") } },
    { IDS_PRESSURE2, { TEXT("mmHg"), TEXT("мм рт.ст.") } },
    { IDS_PRESSURE3, { TEXT("mb"), TEXT("мбар") } },
    { IDS_PRESSURE4, { TEXT("inHg"), TEXT("дюйм рт.ст.") } },
    { IDS_SPEED, { TEXT("Speed:"), TEXT("Скорость:") } },
    { IDS_SPEED1, { TEXT("km/h"), TEXT("км/ч") } },
    { IDS_SPEED2, { TEXT("m/s"), TEXT("м/с") } },
    { IDS_SPEED3, { TEXT("mph"), TEXT("миль/ч") } },
    { IDS_SPEED4, { TEXT("ft/s"), TEXT("фт/с") } },
    { IDS_TEMPERATURE, { TEXT("Temperature:"), TEXT("Температура:") } },
    { IDS_TEMPERATURE1, { TEXT("Celsius"), TEXT("Градус Цельсия") } },
    { IDS_TEMPERATURE2, { TEXT("Fahrenheit"), TEXT("Градус Фаренгейта") } },
    { IDS_MISCELLANEOUS, { TEXT("Miscellaneous:"), TEXT("Разное:") } },
    { IDS_OTHER, { TEXT("Other:"), TEXT("Остальное:") } },
    { IDS_DAY, { TEXT("Day: "), TEXT("День: ") } },
    { IDS_NIGHT, { TEXT("Night: "), TEXT("Ночь: ") } },
};
