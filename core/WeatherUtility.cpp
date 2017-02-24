#include "WeatherUtility.h"

std::string makePath(const DataResource resource, const std::string& value,
    const LanguagePack::LanguageUnits langUnit)
{
    static const std::string accuApikey = "s0LAbqAqtCqL7Of2KcrcnXDfgG3ika6G";

    static const std::unordered_map<LanguagePack::LanguageUnits, const char*> langs{
        { LanguagePack::LanguageUnits::ENG, "en-US" },
        { LanguagePack::LanguageUnits::RUS, "ru-RU" }
    };

    static const std::unordered_map<DataResource, const std::string> paths{
        { DataResource::ACCU_CURRENTCONDITIONS, "/currentconditions/v1/_?details=true&metric=true&apikey=_&language=" },
        { DataResource::ACCU_FORECASTS,         "/forecasts/v1/daily/5day/_?details=true&metric=true&apikey=_&language=" },
        { DataResource::ACCU_LOCATIONS,         "/locations/v1/search?q=_&apikey=_" },
        { DataResource::ACCU_LOCATIONS_INFO,    "/locations/v1/_?apikey=_&language=" },
        { DataResource::YAHOO_GEO_PLACES,       "/v1/public/yql?q=select%20*%20from%20geo.places%20where%20text='_'" },
        { DataResource::YAHOO_WEATHER_FORECAST, "/v1/public/yql?q=select%20*%20from%20weather.forecast%20where%20woeid=_%20and%20u='c'" }
    };

    std::string path = paths.at(resource);

    path.replace(path.find('_'), 1, value);

    if (resource == DataResource::ACCU_CURRENTCONDITIONS ||
        resource == DataResource::ACCU_FORECASTS ||
        resource == DataResource::ACCU_LOCATIONS ||
        resource == DataResource::ACCU_LOCATIONS_INFO)
    {
        path.replace(path.rfind('_'), 1, accuApikey);
        if (resource != DataResource::ACCU_LOCATIONS)
        {
            path += langs.at(langUnit);
        }
    }

    size_t pos = path.find(' ');
    while (pos != std::string::npos)
    {
        path.replace(pos, 1, "%20");
        pos = path.find(' ');
    }

    return path;
}

std::string downloadData(const char* host, const std::string& value, const DataResource resource,
    const LanguagePack::LanguageUnits langUnit)
{
    boost::asio::ip::tcp::iostream ios;
    ios.expires_from_now(boost::posix_time::seconds(60));
    ios.connect(host, "http");
    if (!ios)
    {
        throw NetworkError(ios.error().message().data());
    }

    ios << "GET " << makePath(resource, value, langUnit) << " HTTP/1.0\r\n";
    ios << "Host: " << host << "\r\n";
    ios << "Accept: */*\r\n";
    ios << "Connection: close\r\n\r\n";

    std::string header;
    while (std::getline(ios, header) && header != "\r") { /* Must be empty */ }

    return{ std::istreambuf_iterator<char>{ios}, std::istreambuf_iterator<char>{} };
}
