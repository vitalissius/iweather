#include "GuiLang.h"

GuiLang::Dict::Dict(tstring eng, tstring rus, tstring ukr)
    : eng(std::move(eng))
    , rus(std::move(rus))
    , ukr(std::move(ukr))
{}

const tstring& GuiLang::Dict::GetWord(LanguagePack::LanguageUnits langUnit) const
{
    if (langUnit == LanguagePack::LanguageUnits::ENG)
    {
        return eng;
    }
    else if (langUnit == LanguagePack::LanguageUnits::RUS)
    {
        return rus;
    }
    else
    {
        return ukr;
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



const std::unordered_map<UINT, const GuiLang::Dict> GuiLang::ms_dict{
    { IDS_REFRESH, { TEXT("Refresh"), TEXT("Обновить"), TEXT("Оновити") } },
    { IDS_PIN, { TEXT("Pin"), TEXT("Закрепить"), TEXT("Закріпити") } },
    { IDS_FORECAST, { TEXT("Show forecast"), TEXT("Показывать прогноз"), TEXT("Показувати прогноз") } },
    { IDS_SETTINGS, { TEXT("Settings"), TEXT("Настройки"), TEXT("Налаштування") } },
    { IDS_CLOSE, { TEXT("Close"), TEXT("Закрыть"), TEXT("Закрити") } },
    { IDS_PLACE, { TEXT("Current place: "), TEXT("Текущее место: "), TEXT("Поточне місце: ") } },
    { IDS_SEARCH, { TEXT("Search"), TEXT("Искать"), TEXT("Шукати") } },
    { IDS_TRANSPARANCY, { TEXT("Transparancy: "), TEXT("Прозрачность: "), TEXT("Прозорість: ") } },
    { IDS_LESS, { TEXT("Less"), TEXT("Меньше"), TEXT("Менше") } },
    { IDS_MORE, { TEXT("More"), TEXT("Больше"), TEXT("Більше") } },
    { IDS_COLOR, { TEXT("Color: "), TEXT("Цвет: "), TEXT("Колір: ") } },
    { IDS_AUTORUN, { TEXT("Autorun"), TEXT("Автозапуск"), TEXT("Автозапуск") } },
    { IDS_SOUND, { TEXT("Sound"), TEXT("Звук"), TEXT("Звук") } },
    { IDS_ROUND, { TEXT("Rounded window"), TEXT("Закруглённое окно"), TEXT("Закруглене вікно") } },
    { IDS_LANGUAGE, { TEXT("Language:"), TEXT("Язык:"), TEXT("Мова:") } },
    { IDS_ENGLISH, { TEXT("English"), TEXT("Английкий"), TEXT("Англійська") } },
    { IDS_RUSSIAN,{ TEXT("Russian"), TEXT("Русский"), TEXT("Російська") } },
    { IDS_UKRAINIAN, { TEXT("Ukrainian"), TEXT("Украинский"), TEXT("Українська") } },
    { IDS_SAVE, { TEXT("Save"), TEXT("Сохранить"), TEXT("Зберегти") } },
    { IDS_CANCEL, { TEXT("Cancel"), TEXT("Отменить"), TEXT("Скасувати") } },
    { IDS_APPLY, { TEXT("Apply"), TEXT("Применить"), TEXT("Застосувати") } },
    { IDS_INPUTPLACE, { TEXT("Input place..."), TEXT("Введите место..."), TEXT("Введіть місце...") } },
    { IDS_STYLE, { TEXT("Style:"), TEXT("Стиль:"), TEXT("Стиль:") } },
    { IDS_STYLE1, { TEXT("Style #1"), TEXT("Стиль №1"), TEXT("Стиль №1") } },
    { IDS_STYLE2, { TEXT("Style #2"), TEXT("Стиль №2"), TEXT("Стиль №2") } },
    { IDS_STYLE3, { TEXT("Style #3"), TEXT("Стиль №3"), TEXT("Стиль №3") } },
    { IDS_STYLE4, { TEXT("Style #4"), TEXT("Стиль №4"), TEXT("Стиль №4") } },
    { IDS_PRESSURE, { TEXT("Pressure:"), TEXT("Давление:"), TEXT("Тиск:") } },
    { IDS_PRESSURE1, { TEXT("atm"), TEXT("атм"), TEXT("атм") } },
    { IDS_PRESSURE2, { TEXT("mmHg"), TEXT("мм рт.ст."), TEXT("мм рт.ст.") } },
    { IDS_PRESSURE3, { TEXT("mb"), TEXT("мбар"), TEXT("мбар") } },
    { IDS_PRESSURE4, { TEXT("inHg"), TEXT("дюйм рт.ст."), TEXT("дюйм рт.ст.") } },
    { IDS_SPEED, { TEXT("Speed:"), TEXT("Скорость:"), TEXT("Швидкість:") } },
    { IDS_SPEED1, { TEXT("km/h"), TEXT("км/ч"), TEXT("км/год") } },
    { IDS_SPEED2, { TEXT("m/s"), TEXT("м/с"), TEXT("м/с") } },
    { IDS_SPEED3, { TEXT("mph"), TEXT("миль/ч"), TEXT("миль/год") } },
    { IDS_SPEED4, { TEXT("ft/s"), TEXT("фт/с"), TEXT("фт/с") } },
    { IDS_TEMPERATURE, { TEXT("Temperature:"), TEXT("Температура:"), TEXT("Температура:") } },
    { IDS_TEMPERATURE1, { TEXT("Celsius"), TEXT("Градус Цельсия"), TEXT("Градус Цельсія") } },
    { IDS_TEMPERATURE2, { TEXT("Fahrenheit"), TEXT("Градус Фаренгейта"), TEXT("Градус Фаренгейта") } },
    { IDS_MISCELLANEOUS, { TEXT("Miscellaneous:"), TEXT("Разное:"), TEXT("Різне:") } },
    { IDS_OTHER, { TEXT("Other:"), TEXT("Остальное:"), TEXT("Інше:") } },
    { IDS_NIGHT, { TEXT("Night: "), TEXT("Ночь: "), TEXT("Ніч: ") } },
};
