#pragma once

#include "LanguagePack.h"
#include "Exceptions.h"

#include <boost/asio.hpp>

#include <unordered_map>
#include <string>

enum class DataProvider { YAHOO, ACCUWEATHER };

enum class DataResource {
    YAHOO_GEO_PLACES, YAHOO_WEATHER_FORECAST,
    ACCU_LOCATIONS, ACCU_LOCATIONS_INFO, ACCU_CURRENTCONDITIONS, ACCU_FORECASTS
};

// If resource == YAHOO_GEO_PLACES than value must be city (place)
// If resource == YAHOO_WEATHER_FORECAST than value must be woeid
// If resource == ACCU_LOCATIONS than value must be city (place)
// If resource == ACCU_LOCATIONS_INFO than value must be key
// If resource == ACCU_CURRENTCONDITIONS than value must be key
// If resource == ACCU_FORECASTS than value must be key
std::string makePath(const DataResource resource, const std::string& value,
    const LanguagePack::LanguageUnits langUnit={});

std::string downloadData(const char* host, const std::string& value, const DataResource resource,
    const LanguagePack::LanguageUnits langUnit={});
