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
        case LanguageUnits::UKR:
            std::locale::global(std::locale("ukr_ukr.1251"));
            m_pack = &csm_ukrPack;
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

const std::string& LanguagePack::GetWindDescription(units::BeaufortNumbers beaufortNumber) const
{
    return m_pack->at(Keys::KEY_WIND_DESCRIPTION).at(beaufortNumber);
}

const std::string& LanguagePack::GetWindLandCondition(units::BeaufortNumbers beaufortNumber) const
{
    return m_pack->at(Keys::KEY_WIND_LANDCONDITION).at(beaufortNumber);
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
    }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_WIND_DESCRIPTION,{
        "Calm", "Light air", "Light breeze", "Gentle breeze",
        "Moderate breeze", "Fresh breeze", "Strong breeze", "High wind",
        "Gale", "Strong gale", "Storm", "Violent storm", "Hurricane"
    }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_WIND_LANDCONDITION,{
        "Calm. Smoke rises vertically",
        "Smoke drift indicates wind direction. Leaves and wind vanes are stationary",
        "Wind felt on exposed skin. Leaves rustle. Wind vanes begin to move",
        "Leaves and small twigs constantly moving, light flags extended",
        "Dust and loose paper raised. Small branches begin to move",
        "Branches of a moderate size move. Small trees in leaf begin to sway",
        "Large branches in motion. Whistling heard in overhead wires. Umbrella use becomes difficult. Empty plastic bins tip over",
        "Whole trees in motion. Effort needed to walk against the wind",
        "Some twigs broken from trees. Cars veer on road. Progress on foot is seriously impeded",
        "Some branches break off trees, and some small trees blow over. Construction/temporary signs and barricades blow over",
        "Trees are broken off or uprooted, structural damage likely",
        "Widespread vegetation and structural damage likely",
        "Severe widespread damage to vegetation and structures. Debris and unsecured objects are hurled about"
    }),
};

const std::unordered_map<LanguagePack::Keys, std::vector<std::string>> LanguagePack::csm_rusPack
{
    std::make_pair<Keys, std::vector<std::string>>(KEY_PRESSURE,{ "���", "�� ��.��.", "����", "���� ��.��." }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_SPEED,{ "��/�", "�/�", "����/�", "��/�" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_TEMPERATURE,{ "C", "F" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_PRESSURESTATE,{ "����������", "����������", "����������" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_COMPASSROSE,{
        "��������",     "������-������-���������",      "������-���������",     "�������-������-���������",
        "���������",    "�������-���-���������",       "���-���������",        "���-���-���������",
        "�����",        "���-���-��������",             "���-��������",         "������-���-��������",
        "��������",     "������-������-��������",      "������-��������",      "������-������-��������"
    }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_WIND_DESCRIPTION,{
        "�����", "�����", "˸����", "������",
        "���������", "������", "�������", "�������",
        "����� �������", "�����", "������� �����", "�������� �����", "������",
    }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_WIND_LANDCONDITION,{
        "���������. ��� ����������� �����������, ������ �������� ����������",
        "����������� ����� ������� �� ������ ����, �� �� �� �������",
        "�������� ����� ��������� �����, �������� ������, ���������� � �������� ������",
        "������ � ������ ����� �������� �� ����� ���������, ����� ��������� ����� �����",
        "����� ��������� ���� � �����, �������� � �������� ������ ����� ��������",
        "�������� ������ ������ ��������, �������� ����� ��������� �����",
        "�������� ������� ����� ��������, ����� ����������� �������",
        "�������� ������ ��������",
        "����� ������ ����� ��������, ���� ������ ����� ����� ������",
        "��������� �����������, ����� �������� ��������� ����� ������",
        "������������ ���������� ��������, ����� �������� ������� � ������",
        "������� ���������� �� ������������ ������������. ����������� ����� �����",
        "�������� ����������, �������� ���������� ������, �������� � ����, ������� ������� � �������, �������������� ����������. ������ ����� ������"
    }),
};

const std::unordered_map<LanguagePack::Keys, std::vector<std::string>> LanguagePack::csm_ukrPack
{
    std::make_pair<Keys, std::vector<std::string>>(KEY_PRESSURE,{ "���", "�� ��.��.", "����", "���� ��.��." }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_SPEED,{ "��/���", "�/�", "����/���", "��/�" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_TEMPERATURE,{ "C", "F" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_PRESSURESTATE,{ "������������", "����������", "�������" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_COMPASSROSE,{
        "ϳ������",    "ϳ�����-������-�������",   "ϳ�����-�������",     "������-������-�������",
        "�������",      "������-������-�������",     "ϳ������-�������",     "ϳ������-�������-�������",
        "ϳ�������",    "ϳ������-�������-��������",  "ϳ������-��������",    "�������-�������-��������",
        "��������",     "�������-������-��������",   "ϳ�����-��������",    "ϳ�����-������-��������"
    }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_WIND_DESCRIPTION,{
        "�����", "�����", "������", "�������",
        "�������", "�����", "�������", "̳����",
        "���� �����", "�����", "������� �����", "��������� �����", "������",
    }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_WIND_LANDCONDITION,{
        "����� ��������� ����. ��� ��������� ����������. ����� ����� �������",
        "��� ������. ������ �� ����������",
        "��� ������ ���������� ��������. ��������� �����. ������ ����������",
        "������ �����, ��������� ���� ����. �������� �������",
        "��������� ���� ���� �����. ³��� ����� ��� �� ������ ������",
        "��������� ����� ����. �� ��� �'��������� ����",
        "��������� ����� ����",
        "��������� ������� �������� ����� �� ��� ���������� ����, �� �������",
        "��������� ���� �����. � ����� ��� ����� ����",
        "������� ����������. ����� ��������, ����� �����",
        "����� ����������. ������ ����������� � �������",
        "����� ����������",
        "���������� �� ����������"
    }),
};
