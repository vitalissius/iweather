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
    std::make_pair<Keys, std::vector<std::string>>(KEY_PRESSURE,{ "атм", "мм рт.ст.", "мбар", "дюйм рт.ст." }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_SPEED,{ "км/ч", "м/с", "миль/ч", "фт/с" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_TEMPERATURE,{ "C", "F" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_PRESSURESTATE,{ "Постоянное", "Повышается", "Понижается" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_COMPASSROSE,{
        "Северный",     "Северо-северо-восточный",      "Северо-восточный",     "Востоко-северо-восточный",
        "Восточный",    "Востоко-юго-восточный",       "Юго-восточный",        "Юго-юго-восточный",
        "Южный",        "Юго-юго-западный",             "Юго-западный",         "Западо-юго-западный",
        "Западный",     "Западо-северо-западный",      "Северо-западный",      "Северо-северо-западный"
    }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_WIND_DESCRIPTION,{
        "Штиль", "Тихий", "Лёгкий", "Слабый",
        "Умеренный", "Свежий", "Сильный", "Крепкий",
        "Очень крепкий", "Шторм", "Сильный шторм", "Жестокий шторм", "Ураган",
    }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_WIND_LANDCONDITION,{
        "Безветрие. Дым поднимается вертикально, листья деревьев неподвижны",
        "Направление ветра заметно по относу дыма, но не по флюгеру",
        "Движение ветра ощущается лицом, шелестят листья, приводится в движение флюгер",
        "Листья и тонкие ветви деревьев всё время колышутся, ветер развевает лёгкие флаги",
        "Ветер поднимает пыль и мусор, приводит в движение тонкие ветви деревьев",
        "Качаются тонкие стволы деревьев, движение ветра ощущается рукой",
        "Качаются толстые сучья деревьев, гудят телеграфные провода",
        "Качаются стволы деревьев",
        "Ветер ломает сучья деревьев, идти против ветра очень трудно",
        "Небольшие повреждения, ветер начинает разрушать крыши зданий",
        "Значительные разрушения строений, ветер вырывает деревья с корнем",
        "Большие разрушения на значительном пространстве. Наблюдается очень редко",
        "Огромные разрушения, серьезно повреждены здания, строения и дома, деревья вырваны с корнями, растительность уничтожена. Случай очень редкий"
    }),
};

const std::unordered_map<LanguagePack::Keys, std::vector<std::string>> LanguagePack::csm_ukrPack
{
    std::make_pair<Keys, std::vector<std::string>>(KEY_PRESSURE,{ "атм", "мм рт.ст.", "мбар", "дюйм рт.ст." }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_SPEED,{ "км/год", "м/с", "миль/год", "фт/с" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_TEMPERATURE,{ "C", "F" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_PRESSURESTATE,{ "Стаціонарний", "Зростаючий", "Спадний" }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_COMPASSROSE,{
        "Північний",    "Північно-північно-східний",   "Північно-східний",     "Східно-північно-східний",
        "Східний",      "Східно-північно-східний",     "Південно-східний",     "Південно-південно-східний",
        "Південний",    "Південно-південно-західний",  "Південно-західний",    "Західно-південно-західний",
        "Західний",     "Західно-північно-західний",   "Північно-західний",    "Північно-північно-західний"
    }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_WIND_DESCRIPTION,{
        "Штиль", "Тихий", "Легкий", "Слабкий",
        "Помірний", "Свіжий", "Сильний", "Міцний",
        "Дуже міцний", "Шторм", "Сильний шторм", "Жорстокий шторм", "Ураган",
    }),
    std::make_pair<Keys, std::vector<std::string>>(KEY_WIND_LANDCONDITION,{
        "Повна відсутність вітру. Дим піднімається прямовисно. Листя дерев нерухомі",
        "Дим «пливе». Флюгер не обертається",
        "Рух повітря відчувається обличчям. Шелестить листя. Флюгер обертається",
        "Тріпоче листя, хитаються дрібні гілки. Майорять прапори",
        "Хитаються тонкі гілки дерев. Вітер піднімає пил та шматки паперу",
        "Хитаються великі гілки. На воді з'являються хвилі",
        "Хитаються великі гілки",
        "Хитаються невеликі стовбури дерев На морі здіймаються хвилі, що піняться",
        "Ламаються гілки дерев. і важко йти проти вітру",
        "Невеликі руйнування. Зриває черепицю, руйнує димарі",
        "Значні руйнування. Дерева вириваються з корінням",
        "Великі руйнування",
        "Призводить до спустошень"
    }),
};
