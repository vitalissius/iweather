#include "LanguagePack.h"

LanguagePack::LanguagePack(LanguageUnits language)
{
    SetLanguageUnit(language);
    m_pressureUnit = units::PressureUnits::PU_ATMOSPHERES;
    m_speedUnit = units::SpeedUnits::SU_KILOMETERSPERHOUR;
    m_temperatureUnit = units::TemperatureUnits::TU_CELSIUS;
}



LanguagePack& LanguagePack::Instance()
{
    static LanguagePack langPack;
    return langPack;
}

void LanguagePack::SetLanguageUnit(LanguageUnits languageUnit)
{
    m_languageUnit = languageUnit;
    switch (m_languageUnit)
    {
        case LanguageUnits::ENG:
            std::locale::global(std::locale("eng_us.1251"));
            m_pack = &csm_engPack;
            break;

        case LanguageUnits::RUS:
            std::locale::global(std::locale("rus_rus.1251"));
            m_pack = &csm_rusPack;
            break;
    }
}

void LanguagePack::SetPressureUnit(units::PressureUnits pressureUnit)
{ 
    m_pressureUnit = pressureUnit;
}

void LanguagePack::SetSpeedUnit(units::SpeedUnits speedUnit)
{ 
    m_speedUnit = speedUnit;
}

void LanguagePack::SetTemperatureUnit(units::TemperatureUnits temperatureUnit)
{ 
    m_temperatureUnit = temperatureUnit;
}



LanguagePack::LanguageUnits LanguagePack::GetLanguageUnit() const 
{ 
    return m_languageUnit;
}

units::PressureUnits LanguagePack::GetPressureUnit() const
{ 
    return m_pressureUnit;
}

units::SpeedUnits LanguagePack::GetSpeedUnit() const
{ 
    return m_speedUnit;
}

units::TemperatureUnits LanguagePack::GetTemperatureUnit() const
{ 
    return m_temperatureUnit;
}

const std::string& LanguagePack::GetPressureName() const
{
    return m_pack->at(Keys::KEY_PRESSURE).at(m_pressureUnit);
}

const std::string& LanguagePack::GetSpeedName() const
{
    return m_pack->at(Keys::KEY_SPEED).at(m_speedUnit);
}

const std::string& LanguagePack::GetTemperatureName(units::TemperatureUnits temperature) const
{
    return m_pack->at(Keys::KEY_TEMPERATURE).at(temperature);
}

const std::string& LanguagePack::GetPressureStateName(units::PressureStateUnits pressureState) const
{
    return m_pack->at(Keys::KEY_PRESSURESTATE).at(pressureState);
}

const std::string& LanguagePack::GetCompassRoseName(units::CompassRoseUnits compassRose) const
{
    return m_pack->at(Keys::KEY_COMPASSROSE).at(compassRose);
}



const std::unordered_map<LanguagePack::Keys, std::vector<std::string>> LanguagePack::csm_engPack
{
    std::make_pair<Keys, std::vector<std::string>>(KEY_PRESSURE,{ "atm", "mmHg", "mb", "inHg" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_SPEED,{ "km/h", "m/s", "mph", "ft/s" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_TEMPERATURE,{ "C", "F" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_PRESSURESTATE,{ "Steady", "Rising", "Falling" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_COMPASSROSE,{
        "North",    "North-northeast",  "Northeast",    "East-northeast",
        "East",     "East-southeast",   "Southeast",    "South-southeast",
        "South",    "South-southwest",  "Southwest",    "West-southwest",
        "West",     "West-northwest",   "Northwest",    "North-northwest"
    })
};

const std::unordered_map<LanguagePack::Keys, std::vector<std::string>> LanguagePack::csm_rusPack
{
    std::make_pair<Keys, std::vector<std::string>>(KEY_PRESSURE,{ "атм", "мм рт.ст.", "мбар", "дюйм рт.ст." }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_SPEED,{ "км/ч", "м/с", "миль/ч", "фт/с" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_TEMPERATURE,{ "C", "F" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_PRESSURESTATE,{ "Постоянное", "Повышается", "Понижается" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_COMPASSROSE,{
        "Северный",     "Северо-северо-восточный",      "Северо-восточный",     "Востоко-северо-восточный",
        "Восточный",    "Востоко-юго-восточный",       "Юго-восточный",        "Юго-юго-восточный",
        "Южный",        "Юго-юго-западный",             "Юго-западный",         "Западо-юго-западный",
        "Западный",     "Западо-северо-западный",      "Северо-западный",      "Северо-северо-западный"
    })
};
