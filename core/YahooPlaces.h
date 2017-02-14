#pragma once

#include "AbstractPlaces.h"
#include "Exceptions.h"
#include "WeatherUtility.h"
#include "Utility.h"

#include <tinyxml2.h>

class YahooPlaces : public AbstractPlaces {
private:
    YahooPlaces() = default;

public:
    static YahooPlaces* Instance();

public:
    void Update(const std::string& place, const LanguagePack::LanguageUnits lang) override;

private:
    void parse(std::string page) override;

private:
    static std::string makePlace(const char* pszTown, const char* pszCountry, const char* pszRegion);
};
