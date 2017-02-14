#pragma once

#include "AbstractPlaces.h"
#include "Exceptions.h"
#include "WeatherUtility.h"

#include <json/json.h>

class AccuPlaces : public AbstractPlaces {
private:
    AccuPlaces() = default;

public:
    static AccuPlaces* Instance();

public:
    void Update(const std::string& place, const LanguagePack::LanguageUnits lang) override;

private:
    void parse(std::string page) override;

private:
    static std::string makePlace(std::string type, std::string name, std::string country, std::string area);
};
