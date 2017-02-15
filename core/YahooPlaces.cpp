#include "YahooPlaces.h"

YahooPlaces* YahooPlaces::Instance()
{
    static YahooPlaces places;
    return &places;
}

void YahooPlaces::Update(const std::string& place)
{
    if (!m_places.empty())
    {
        m_places.clear();
    }

    std::string page = downloadData("query.yahooapis.com", place, DataResource::YAHOO_GEO_PLACES);
    parse(std::move(page));
}

void YahooPlaces::parse(std::string page)
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.Parse(page.data(), page.size());
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

            std::string place(utf8ToCp1251(makePlace(pszTown, pszCountry, pszRegion)));
            m_places.emplace_back(std::move(pszWoeid), std::move(place));

            pePlace = pePlace->NextSiblingElement("place");
        }
    }
    else
    {
        throw IncorrectFile("XML file is empty: Does not contain elements");
    }
}

std::string YahooPlaces::makePlace(const char* pszTown, const char* pszCountry, const char* pszRegion)
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
