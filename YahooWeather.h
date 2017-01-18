#pragma once

#include "AbstractWeather.h"

#include <boost/asio.hpp>
#include <tinyxml2.h>

#include <codecvt>
#include <locale>
#include <vector>
#include <iterator>
#include <iostream>

std::string trimFrontBackSpaces(std::string str);

enum class Table { GEO_PLACES, WEATHER_FORECAST };

std::string makeQuery(const std::string& value, const Table table);

std::string makeRequest(std::string query);

//@param: value - woeid or place
//@param: table - must be Table::GEO_PLACES if 'value' is a woeid or Table::WEATHER_FORECAST if it's a place
std::string downloadData(const std::string& value, const Table table);



class Places {
    struct Place {
        std::string woeid;
        std::string place;
    };

private:
    Places() = default;

public:
    Places(const Places&) = delete;
    Places& operator=(const Places&) = delete;

    Places(Places&&) = default;
    Places& operator=(Places&&) = default;

    static Places& CreateInstance();
    void Update(const char* place);
    std::string GetWoeid(const std::string& place) const;

    std::vector<Place>::const_iterator begin() const;
    std::vector<Place>::const_iterator end() const;
    std::size_t size() const;

private:
    void parseXmlData(std::string xmlPage);
    std::string correctCharacterEncoding(std::string from);
    std::string makePlace(const char* pszTown, const char* pszCountry, const char* pszRegion);

private:
    std::vector<Place> m_places;

private:
    static const std::locale currLocale;
};



class YahooWeather : public AbstractWeather {
public:
    void Update(const std::string& woeid);

private:
    void updateLocation(tinyxml2::XMLNode* node);
    void updateWind(tinyxml2::XMLNode* node);
    void updateAtmosphere(tinyxml2::XMLNode* node);
    void updateAstronomy(tinyxml2::XMLNode* node);
    void updateCondition(tinyxml2::XMLNode* node);
    void updatePublicationDate(tinyxml2::XMLNode* node);
    void updateForecast(tinyxml2::XMLNode* node);
};
