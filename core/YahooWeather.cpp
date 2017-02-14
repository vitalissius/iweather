#include "YahooWeather.h"

void YahooWeather::Update(const std::string& woeid)
{
    std::string page = downloadData("query.yahooapis.com", woeid, DataResource::YAHOO_WEATHER_FORECAST);
    if (page.size() < 3000u)
    {
        return;
    }
    tinyxml2::XMLDocument document;
    document.Parse(page.data(), page.size());

    tinyxml2::XMLNode* node;
    node = document.FirstChildElement()->FirstChildElement()->FirstChildElement()->FirstChildElement();
    node = node->ToElement()->NextSiblingElement("yweather:location");

    updateAstronomy(node);
    updateAtmosphere(node);
    updateCondition(node);
    updateForecast(node);
    updateLocation(node);
    updatePublicationDate(node);
    updateWind(node);
}

void YahooWeather::updateLocation(tinyxml2::XMLNode* node)
{
    const char* city = node->ToElement()->Attribute("city");
    const char* country = node->ToElement()->Attribute("country");
    const char* region = node->ToElement()->Attribute("region");

    SetLocation(trim(city).data(), trim(country).data(), trim(region).data());
}

void YahooWeather::updateWind(tinyxml2::XMLNode* node)
{
    node = node->ToElement()->NextSiblingElement("yweather:wind");

    int direction = 0;
    float speed = 0.0f;

    node->ToElement()->QueryIntAttribute("direction", &direction);
    node->ToElement()->QueryFloatAttribute("speed", &speed);

    SetWind(direction, speed);
}

void YahooWeather::updateAtmosphere(tinyxml2::XMLNode* node)
{
    node = node->ToElement()->NextSiblingElement("yweather:atmosphere");

    int humidity = 0;
    float pressure = 0.0f;
    int rising = 0;
    node->ToElement()->QueryIntAttribute("humidity", &humidity);
    node->ToElement()->QueryFloatAttribute("pressure", &pressure);
    node->ToElement()->QueryIntAttribute("rising", &rising);

    constexpr float magicNumber = 33.8639f;
    const float bar = pressure / magicNumber / 1000.0f;
    const float atm = bar * 0.987f;

    SetAtmosphere(humidity, atm, rising);
}

void YahooWeather::updateAstronomy(tinyxml2::XMLNode* node)
{
    node = node->ToElement()->NextSiblingElement("yweather:astronomy");

    const char* sunrise = node->ToElement()->Attribute("sunrise");
    const char* sunset = node->ToElement()->Attribute("sunset");


    SetAstronomy(sunrise, sunset);
}

void YahooWeather::updateCondition(tinyxml2::XMLNode* node)
{
    node = node->ToElement()->NextSiblingElement("item");
    node = node->FirstChildElement();
    node = node->ToElement()->NextSiblingElement("yweather:condition");

    const char* text = node->ToElement()->Attribute("text");
    int code = 0;
    int temp = 0;
    node->ToElement()->QueryIntAttribute("code", &code);
    node->ToElement()->QueryIntAttribute("temp", &temp);

    SetCondition(code, temp, text);
}

void YahooWeather::updatePublicationDate(tinyxml2::XMLNode* node)
{
    node = node->ToElement()->NextSiblingElement("item");
    node = node->FirstChildElement();
    const char* date = node->ToElement()->NextSiblingElement("pubDate")->GetText();

    SetPublicationDate(date);
}

void YahooWeather::updateForecast(tinyxml2::XMLNode* node)
{
    node = node->ToElement()->NextSiblingElement("item");
    node = node->FirstChildElement();
    node = node->ToElement()->NextSiblingElement("yweather:forecast");

    for (int i = 0; i < msc_forecastSize; ++i)
    {
        const char* day = node->ToElement()->Attribute("day");
        const char* date = node->ToElement()->Attribute("date");
        int low = 0;
        node->ToElement()->QueryIntAttribute("low", &low);
        int high = 0;
        node->ToElement()->QueryIntAttribute("high", &high);
        const char* text = node->ToElement()->Attribute("text");
        int code = 0;
        node->ToElement()->QueryIntAttribute("code", &code);

        AddForecast(code, date, day, high, low, text);

        node = node->NextSibling();
    }
}
