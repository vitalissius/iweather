#include "AccuWeather.h"

void AccuWeather::Update(const std::string& woeid)
{
    std::string page = downloadData("dataservice.accuweather.com", woeid,
        DataResource::ACCU_LOCATIONS_INFO, m_langPack.GetLanguageUnit());
    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(page, root))
    {
        throw IncorrectFile("Json weather file: parsing error");
    }
    updateLocation(root);


    page = std::move(downloadData("dataservice.accuweather.com", woeid,
        DataResource::ACCU_CURRENTCONDITIONS, m_langPack.GetLanguageUnit()));
    if (!reader.parse(page, root))
    {
        throw IncorrectFile("Json weather file: parsing error");
    }
    updatePublicationDate(root);
    updateCondition(root[0]);
    updateAtmosphere(root[0]);
    updateWind(root[0]);

    page = std::move(downloadData("dataservice.accuweather.com", woeid,
        DataResource::ACCU_FORECASTS, m_langPack.GetLanguageUnit()));
    if (!reader.parse(page, root))
    {
        throw IncorrectFile("Json weather file: parsing error");
    }
    updateAstronomy(root["DailyForecasts"][0]);
    updateForecast(root["DailyForecasts"]);
}

void AccuWeather::updateLocation(const Json::Value& root)
{
    auto city = utf8ToCp1251(root["LocalizedName"].asString());
    if (city.empty())
    {
        city = root["EnglishName"].asString();
    }

    auto country = utf8ToCp1251(root["Country"]["LocalizedName"].asString());
    if (country.empty())
    {
        country = root["Country"]["EnglishName"].asString();
    }

    auto region = utf8ToCp1251(root["AdministrativeArea"]["LocalizedName"].asString());
    if (region.empty())
    {
        region = root["AdministrativeArea"]["EnglishName"].asString();
    }
    SetLocation(city.data(), country.data(), region.data());
}

void AccuWeather::updatePublicationDate(const Json::Value& root)
{
    const auto epochTime = root[0]["EpochTime"].asInt64();
    char buffer[40];
    tm time;
    localtime_s(&time, &epochTime);
    strftime(buffer, sizeof(buffer), "%e %b %Y (%A) %r", &time);

    SetPublicationDate(buffer);
}

void AccuWeather::updateCondition(const Json::Value& root)
{
    const auto code = getCorrespondingCode(root["WeatherIcon"].asInt());
    const auto text = utf8ToCp1251(root["WeatherText"].asString());
    const auto temp = root["Temperature"]["Metric"]["Value"].asFloat();

    SetCondition(code, int(std::round(temp)), text.data());
}

void AccuWeather::updateAtmosphere(const Json::Value& root)
{
    auto humidity = root["RelativeHumidity"].asInt();
    auto pressure = root["Pressure"]["Metric"]["Value"].asFloat();
    pressure = pressure / 1013.25f;
    // F, S, R - pressure tendency code regardless of language.
    // F=>Falling, S=>Steady, R=>Rising (s=0, r=1, f=2)
    auto pressureState = 0;
    switch (root["PressureTendency"]["Code"].asCString()[0])
    {
        case 'S':
            pressureState = 0;
            break;
        case 'R':
            pressureState = 1;
            break;
        case 'F':
            pressureState = 2;
            break;
    }
    SetAtmosphere(humidity, pressure, pressureState);
}

void AccuWeather::updateWind(const Json::Value& root)
{
    auto direction = root["Wind"]["Direction"]["Degrees"].asInt();
    auto speed = root["Wind"]["Speed"]["Metric"]["Value"].asFloat();
    SetWind(direction, speed);
}

void AccuWeather::updateAstronomy(const Json::Value& root)
{
    auto rise = root["Sun"]["Rise"].asString();
    auto set = root["Sun"]["Set"].asString();

    std::smatch res;
    std::regex rgx("T(..:..):");
    std::regex_search(rise, res, rgx);
    auto sunrise = res[1].str();

    std::regex_search(set, res, rgx);
    auto sunset = res[1].str();

    SetAstronomy(sunrise.data(), sunset.data());
}

void AccuWeather::updateForecast(const Json::Value& root)
{
    for (Json::ArrayIndex i = 0; i < root.size(); ++i)
    {
        auto iroot = root[i];
        auto code = getCorrespondingCode(iroot["Day"]["Icon"].asInt());

        auto epochTime = iroot["EpochDate"].asInt64();
        tm time;
        char buffer[16];
        localtime_s(&time, &epochTime);
        strftime(buffer, sizeof(buffer), "%a %b %d %Y", &time);

        std::cmatch res;
        std::regex rgx("(.{2,3}) (...) (..) (....)");           // "Fri Jan 27 2017"
        std::regex_search(buffer, res, rgx);

        std::string dayname = res[1];

        std::string month = res[2];
        std::string number = res[3];
        std::string year = res[4];
        auto date((((number += " ") += month) += " ") += year);

        auto high = int(std::round(iroot["Temperature"]["Maximum"]["Value"].asFloat()));

        auto low = int(std::round(iroot["Temperature"]["Minimum"]["Value"].asFloat()));

        auto dphrase = iroot["Day"]["IconPhrase"].asString();
        auto nphrase = iroot["Night"]["IconPhrase"].asString();
        int diff = nphrase.size() - dphrase.size();
        if (diff > 0)
        {
            dphrase.append(diff * 2, ' ');
        }
        auto text = utf8ToCp1251((dphrase += '\n') += nphrase);

        AddForecast(code, std::move(date), std::move(dayname), high, low, std::move(text));
    }
}

int AccuWeather::getCorrespondingCode(const int code)
{
    static int map[]{
        48, 32, 34, 30, 44, 34, 28, 26, 26, 48,
        48, 20, 40, 12, 39,  0,  0, 38, 40, 13,
        13, 13, 16, 14, 25, 18,  8, 48, 48,  7,
        36, 25, 23, 31, 33, 29, 27, 33, 27, 45,
        45, 47, 47, 13, 14
    };
    return map[code];
}
