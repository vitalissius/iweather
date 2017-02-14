#pragma once

#include "AbstractWeather.h"
#include "WeatherUtility.h"

#include <boost/asio.hpp>
#include <tinyxml2.h>

#include <codecvt>
#include <locale>
#include <vector>
#include <iterator>
#include <iostream>

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
