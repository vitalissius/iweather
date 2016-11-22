#pragma once

#include "Exceptions.h"
#include "LanguagePack.h"

#include <algorithm>
#include <iomanip>
#include <string>
#include <vector>
#include <regex>
#include <sstream>

class AbstractWeather {
private:
    /* Additional private structures */

    /* Additional private structure Location */
    struct Location {
        std::string m_city;
        std::string m_country;
        std::string m_region;
    };

    /* Additional private structure Wind */
    struct Wind {
        int m_direction;
        float m_speed;
    };

    /* Additional private structure Atmosphere */
    struct Atmosphere {
        int m_humidity;
        float m_pressure;
        units::PressureStateUnits m_pressureState;
    };

    /* Additional private structure Astronomy */
    struct Astronomy {
		enum class ClockSystemUnits { THE12HOURCLOCK, THE24HOURCLOCK };

        std::string m_sunrise;
        std::string m_sunset;
        ClockSystemUnits m_clockSystemUnit;

    private:
        std::string from24to12hourClock(int hours, int minuts);
        std::string from12to24hourClock(int hours, int minuts, bool isPm);

    public:
        Astronomy(const char* sunrise, const char* sunset);
        Astronomy();
        void changeClockSystemTo(ClockSystemUnits clockSystem);
    };

    /* Additional private structure Condition */
    struct Condition {
        int m_code;
        int m_temp;
        std::string m_text;
    };

    /* Additional private structure Forecast */
    struct Forecast {
    private:
        int m_code;
        std::string m_date;
        std::string m_day;
        int m_high;
        int m_low;
        std::string m_text;

    public:
        Forecast(int code, std::string date, std::string day, int high, int low, std::string text);
        int GetCode() const;
        const std::string& GetDate() const;
        const std::string& GetDayName() const;
        int GetHighTemp() const;
        int GetLowTemp() const;
        const std::string GetDescriptionText() const;
    };

private:
    /* AbstractWeather's private members */
    std::vector<Forecast> m_forecast;
    Location m_location;
    Condition m_condition;
    Astronomy m_astronomy;
    Atmosphere m_atmosphere;
    Wind m_wind;
    std::string m_pubDate;
    LanguagePack m_langPack;

private:
    /* AbstractWeather's private constant static member */
    static const int msc_forecastSize = 5;

public:
    /* AbstractWeather's manager methods */
    AbstractWeather();
    virtual ~AbstractWeather() = 0;

    /* AbstractWeather's mutator methods */
    //@description: The weather forecast for a specific day
    //@param: code - This code used to choose a text description or image for the forecast
    //@param: date - The date to which this forecast applies
    //@param: day - Day of the week to which this forecast applies (Mon, Tue, Wed, Thu, Fri, Sat, Sun)
    //@param: high - The forecasted high temperature for this day (set this value by the Celsius)
    //@param: low - The forecasted low temperature for this day (set this value by the Celsius)
    //@param: text - A textual description of conditions (for example, "Partly Cloudy", "Sunny")
    void AddForecast(int code, std::string date, std::string day, int high, int low, std::string text);
    
    //@description: The location of this forecast
    //@param: city - City name
    //@param: region - State, territory, or region, if given
    //@param: country - Country name
    void SetLocation(const char* city, const char* country, const char* region);

    //@description: The current weather conditions
    //@param: text - A textual description of conditions (for example, "Partly Cloudy", "Sunny")
    //@param: code - This code used to choose a text description or image for the forecast
    //@param: temp - The current temperature (set this value in the Celsius)
    void SetCondition(int code, int temp, const char* text);
    
    //@description: Forecast information about current astronomical conditions
    //@param: sunrise - Today's sunrise time
    //@param: sunset - Today's sunset time
    void SetAstronomy(const char* sunrise, const char* sunset);
    
    //@description: Forecast information about current atmospheric pressure and humidity
    //@param: humidity - Humidity, in percent
    //@param: pressure - Barometric pressure (set this value in the 'atm' units)
    //@param: pressureState - State of the barometric pressure (steady=='0', rising=='1', falling=='2')
    void SetAtmosphere(int humidity, float pressure, int pressureState);

    //@description: Forecast information about wind
    //@param: direction - Wind direction (set this value in the degrees from '0' to '359')
    //@param: speed - Wind speed (set this value in the 'kph')
    void SetWind(int direction, float speed);

    //@description: Information about date and time when the forecast was posted
    //@param: pubDate - The date and time (for example 'Tue, 22 Nov 2016 12:00 AM EET')
    void SetPublicationDate(const char* pubDate);

public:
    /* AbstractWeather's accessor methods */
    
    //int GetConditionCode() const { return m_condition.m_code; }
    //int GetConditionTemt() const { return m_condition.m_temp; }
    //const std::string& GetConditionText() const { return m_condition.m_text; }

    //@return: Ref to the vector of the Forecast-s (size of the vector must be equal msc_forecastSize)
    const std::vector<Forecast>& GetForecastVector() const;

    //@return: 'City'
    const std::string& GetLocationCLine() const;

    //@return: 'City, Country'
    std::string GetLocationCcLine() const;

    //@return: 'City, Country (Region)'
    std::string GetLocationCcrLine() const;

    //@return: Compass rose name
    const std::string& GetWindDirectionLine() const;

    //@return: String describing wind speed
    std::string GetWindSpeedLine() const;

    //@return: String describing humidity of the atmosphere
    std::string GetHumidityLine() const;

    //@return: String describing atmospheric pressure
    std::string GetPressureLine() const;

    //@return: String describing atmospheric pressure state
    const std::string& GetPressureStateLine() const;

    //@return: String describing time of the sunrise
    const std::string& GetSunriseLine() const;

    //@return: string describing time of the sunset
    const std::string& GetSunsetLine() const;

    //@return: String describing of the publication date (time)
    const std::string& GetPublicationDateLine() const;

public:
    /* AbstractWeather's "changeror" methods */
    void ChangeSpeedUnitTo(units::SpeedUnits speedUnit);
    void ChangePressureUnitTo(units::PressureUnits pressureUnit);
    void ChangeLanguageTo(LanguagePack::LanguageUnits languageUnit);

    void ChangeClockSystemTo24h();
    void ChangeClockSystemTo12h();

private:
    /* Private additional AbstractWeather's "changeror" methods */
    //@return: Velocity in specific units
    float to_KilometersPerHour() const;
    float to_MetersPerSecond() const;
    float to_MilesPerHour() const;
    float to_FeetPerSecond() const;

    //@return: Atmospheric pressure in specific units
    float to_Atm() const;
    float to_MilliBar() const;
    float to_MillimeterOfMercury() const;
    float to_InchesOfMercury() const;
};
