#pragma once

#include "AbstractWeather.h"
#include "Exceptions.h"
#include "WeatherUtility.h"

#include <json/json.h>

class AccuWeather : public AbstractWeather {
public:
    void Update(const std::string& woeid) override;

private:
    void updateLocation(const Json::Value& root);
    void updatePublicationDate(const Json::Value& root);
    void updateCondition(const Json::Value& root);
    void updateAtmosphere(const Json::Value& root);
    void updateWind(const Json::Value& root);
    void updateAstronomy(const Json::Value& root);
    void updateForecast(const Json::Value& root);
    int getCorrespondingCode(const int code);
};
