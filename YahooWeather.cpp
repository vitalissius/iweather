#include "YahooWeather.h"

const std::locale Places::currLocale("");                   // Current locale. Or "rus_rus.1251"

std::string trimFrontBackSpaces(std::string str)
{
    while (str.front() == ' ')
        str = str.substr(1, str.size());
    while (str.back() == ' ')
        str = str.substr(0, str.size() - 1);
    return str;
}

std::string makeQuery(const std::string& value, const Table table)
{
    std::string query;
    switch (table) 
    {
        case Table::GEO_PLACES:
            query = "select * from geo.places where text='_'";
            break;
        case Table::WEATHER_FORECAST:
            query = "select * from weather.forecast where woeid=_ and u='c'&format=xml";
            break;
        default:
            throw IncorrectTableName();
    }

    auto pos = query.rfind('_');
    query.replace(pos, 1, value);
    
    while ((pos = query.find(' ')) != std::string::npos)
    {
        query.replace(pos, 1, "%20");
    }
    
    return query;
}

std::string makeRequest(std::string query)
{
    std::string request = "GET /v1/public/yql?q=_ HTTP/1.0\r\n";
    const auto pos = request.find('_');
    request.replace(pos, 1, query);
    return request;
}

std::string downloadData(const std::string& value, const Table table)
{
    boost::asio::ip::tcp::iostream ios;
    ios.expires_from_now(boost::posix_time::seconds(60));
    ios.connect("query.yahooapis.com", "http");
    if (!ios)
    {
        throw SocketError(ios.error().message().data());
    }

    ios << makeRequest(std::move(makeQuery(value, table)));
    ios << "Host: query.yahooapis.com\r\n";
    ios << "Accept: */*\r\n";
    ios << "Connection: close\r\n\r\n";

    std::string header;
    while (std::getline(ios, header) && header != "\r") 
    { 
        /* Must be empty */ 
    }

    std::string xmlPage((std::istreambuf_iterator<char>(ios)), std::istreambuf_iterator<char>());
    return xmlPage;
}



Places& Places::CreateInstance()
{
    static Places places;
    return places;
}

void Places::Update(const char* place)
{
    if (m_places.size() != 0)
    {
        m_places.clear();
    }
    auto page = downloadData(place, ::Table::GEO_PLACES);
    parseXmlData(std::move(page));
}

std::string Places::GetWoeid(const std::string& place) const
{
    for (const auto& e : m_places)
    {
        if (e.place == place)
            return e.woeid;
    }
    return "";
}

std::vector<Places::Place>::const_iterator Places::begin() const
{
    return m_places.begin();
}

std::vector<Places::Place>::const_iterator Places::end() const
{
    return m_places.end();
}

std::size_t Places::size() const
{
    return m_places.size();
}

void Places::parseXmlData(std::string xmlPage)
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.Parse(xmlPage.data(), xmlPage.size());
    if (error != tinyxml2::XML_SUCCESS) 
    {
        throw IncorrectFile("XML file corrupted: function Parse()");
    }

    tinyxml2::XMLElement* peQuery = document.FirstChildElement("query");
    if (!peQuery) 
    {
        throw IncorrectFile("Incorrect XML file: function FirstChildElement()");
    }

    int count = peQuery->ToElement()->IntAttribute("yahoo:count");
    if (count != 0)
    {
        tinyxml2::XMLElement* pePlace = peQuery->FirstChild()->FirstChildElement("place");
        for (int i = 0; i < count; ++i)
        {
            tinyxml2::XMLElement* peData = pePlace->FirstChildElement("woeid");

            const char* pszWoeid = peData->GetText();
            const char* pszCountry = peData->NextSiblingElement("country")->GetText();
            const char* pszRegion = peData->NextSiblingElement("admin1")->GetText();
            const char* pszTown = peData->NextSiblingElement("locality1")->GetText();

            auto place = makePlace(pszTown, pszCountry, pszRegion);
            m_places.push_back(Places::Place{ pszWoeid, correctCharacterEncoding(std::move(place)) });

            pePlace = pePlace->NextSiblingElement("place");
        }
    }
    else
    {
        throw IncorrectFile("XML file is empty: Does not contain elements");
    }
}

std::string Places::correctCharacterEncoding(std::string from)
{
    std::wstring wide = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(from.data());
    std::string narrow(wide.size(), '\0');
    std::use_facet<std::ctype<wchar_t>>(currLocale).narrow(wide.data(), wide.data() + wide.size(), 
        '?', &narrow[0]);
    return narrow;
}

std::string Places::makePlace(const char* pszTown, const char* pszCountry, const char* pszRegion)
{
    std::string place = "";

    if (pszTown && pszCountry && pszRegion)
    {
        place += pszTown;
        place += ", ";
        place += pszCountry;
        place += " (";
        place += pszRegion;
        place += ")";
    }
    else if (pszCountry && pszRegion)
    {
        place += pszRegion;
        place += ", ";
        place += pszCountry;
    }
    else if (pszTown && pszCountry)
    {
        place += pszTown;
        place += ", ";
        place += pszCountry;
    }
    else
    {
        place += pszCountry;
    }

    return place;
}



void YahooWeather::Update(const std::string& woeid)
{
    const std::string page = downloadData(woeid, ::Table::WEATHER_FORECAST);
    if (page.size() < 3000u)
    {
        // If XML page does not contains complete data - do not update status
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

    SetLocation(trimFrontBackSpaces(city).data(), 
        trimFrontBackSpaces(country).data(),
        trimFrontBackSpaces(region).data());
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

    //ChangeClockSystemTo(CurrentClockSystem());
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
