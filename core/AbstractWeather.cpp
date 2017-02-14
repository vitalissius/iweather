#include "AbstractWeather.h"

LanguagePack& AbstractWeather::m_langPack = LanguagePack::Instance();

int temperatureTo(const int temp, units::TemperatureUnits tempUnit)
{
    if (tempUnit == units::TemperatureUnits::TU_CELSIUS) { return temp; }
    else { return int(temp * 1.8f + 32); }
}

/* Methods of the Astronomy additional private structure */

AbstractWeather::Astronomy::Astronomy(const char* sunrise, const char* sunset)
    : m_sunrise(sunrise)
    , m_sunset(sunset)
{
    if (std::regex_search(sunrise, std::regex("[pPaA].??m|M.??")))
        m_clockSystemUnit = ClockSystemUnits::THE12HOURCLOCK;
    else
        m_clockSystemUnit = ClockSystemUnits::THE24HOURCLOCK;
}

AbstractWeather::Astronomy::Astronomy()
    : Astronomy("00:00", "00:00")
{}

void AbstractWeather::Astronomy::changeClockSystemTo(ClockSystemUnits clockSystem)
{
    if (m_clockSystemUnit == clockSystem)
        return;

    std::regex expr("([0-9]+):([0-9]+)");
    std::smatch match;

    for (auto* s : { &m_sunrise, &m_sunset })
    {
        if (std::regex_search(*s, match, expr))
        {
            int hours = std::stoi(match[1]);
            int minuts = std::stoi(match[2]);

            if (m_clockSystemUnit == ClockSystemUnits::THE12HOURCLOCK)
            {
                if (std::regex_search(*s, match, std::regex("(p.??m.??)")))
                    *s = from12to24hourClock(hours, minuts, true);
                else
                    *s = from12to24hourClock(hours, minuts, false);
            }
            else
            {
                *s = from24to12hourClock(hours, minuts);
            }
        }
    }

    m_clockSystemUnit = clockSystem;
}

std::string AbstractWeather::Astronomy::from24to12hourClock(int hours, int minuts)
{
    std::string time = "";

    if (hours <= 12)
    {
        if (hours == 0)
            time += "12";
        else
            time += std::to_string(hours);
    }
    else
    {
        time += std::to_string(hours - 12);
    }

    time += ":";

    if (minuts <= 9)
        time += "0";
    time += std::to_string(minuts);

    if (hours <= 11)
        time += " am";
    else
        time += " pm";
    return time;
}

std::string AbstractWeather::Astronomy::from12to24hourClock(int hours, int minuts, bool isPm)
{
    std::string time = "";

    if (isPm)
    {
        if (hours != 12)
            hours += 12;
    }
    else if (hours == 12)
        hours = 0;

    if (hours <= 9)
        time += "0";
    time += std::to_string(hours);

    time += ":";

    if (minuts <= 9)
        time += "0";
    time += std::to_string(minuts);

    return time;
}



/* Methods of the Forecast additional private structure */

AbstractWeather::Forecast::Forecast(int code, std::string date, std::string day, int high, int low, std::string text)
    : m_code(code)
    , m_date(date)
    , m_day(day)
    , m_high(high)
    , m_low(low)
    , m_text(text)
{}

int AbstractWeather::Forecast::GetCode() const 
{ 
    return m_code; 
}

const std::string& AbstractWeather::Forecast::GetDate() const 
{ 
    return m_date; 
}

const std::string& AbstractWeather::Forecast::GetDayName() const 
{ 
    return m_day; 
}

int AbstractWeather::Forecast::GetHighTemp() const 
{ 
    return temperatureTo(m_high, m_langPack.GetTemperatureUnit());
}

int AbstractWeather::Forecast::GetLowTemp() const
{
    return temperatureTo(m_low, m_langPack.GetTemperatureUnit());
}

const std::string AbstractWeather::Forecast::GetDescriptionText() const
{
    return m_text;
}



/* AbstractWeather's manager methods */

AbstractWeather::AbstractWeather()
{
    m_forecast.reserve(msc_forecastSize);
}



/* AbstractWeather's mutator methods */

void AbstractWeather::AddForecast(int code, std::string date, std::string day, int high, int low, std::string text)
{
    static int index = 0;

    if (index >= msc_forecastSize)
    {
        index = 0;
        m_forecast.clear();
    }
    Forecast forecast(code, date, day, high, low, text);
    m_forecast.emplace_back(std::move(forecast));
    ++index;
}

void AbstractWeather::SetLocation(const char* city, const char* country, const char* region)
{
    m_location.m_city = city;
    m_location.m_country = country;
    m_location.m_region = region;
}

void AbstractWeather::SetCondition(int code, int temp, const char* text)
{
    m_condition.m_code = code;
    m_condition.m_temp = temp;
    m_condition.m_text = text;
}

void AbstractWeather::SetAstronomy(const char* sunrise, const char* sunset)
{
    Astronomy astronomy(sunrise, sunset);
    if (astronomy.m_clockSystemUnit != m_astronomy.m_clockSystemUnit)
    {
        astronomy.changeClockSystemTo(m_astronomy.m_clockSystemUnit);
    }
    m_astronomy = astronomy;
}

void AbstractWeather::SetAtmosphere(int humidity, float pressure, int pressureState)
{
    m_atmosphere.m_humidity = humidity;
    m_atmosphere.m_pressure = pressure;
    m_atmosphere.m_pressureState = units::PressureStateUnits(pressureState);
}

void AbstractWeather::SetWind(int direction, float speed)
{
    m_wind.m_direction = direction;
    m_wind.m_speed = speed;
}

void AbstractWeather::SetPublicationDate(const char* pubDate)
{
    m_pubDate = pubDate;
}



/* AbstractWeather's accessor methods */

int AbstractWeather::GetConditionCode() const
{
    return m_condition.m_code;
}

int AbstractWeather::GetConditionTemt() const
{
    return temperatureTo(m_condition.m_temp, m_langPack.GetTemperatureUnit());
}

const std::string& AbstractWeather::GetConditionText() const
{
    return m_condition.m_text;
}

const std::vector<AbstractWeather::Forecast>& AbstractWeather::GetForecastVector() const
{
    if (msc_forecastSize != m_forecast.size())
    {
        std::stringstream ss;
        ss << "Incorrect forecast size! Must be " << msc_forecastSize;
        throw IncorrectForecastSize(ss.str().data());
    }
    return m_forecast;
}

const std::string& AbstractWeather::GetLocationCLine() const
{
    return m_location.m_city;
}

std::string AbstractWeather::GetLocationCcLine() const
{
    return m_location.m_city + ", " + m_location.m_country;
}

std::string AbstractWeather::GetLocationCcrLine() const
{
    return m_location.m_city + ", " + m_location.m_country + " (" + m_location.m_region + ")";
}

const std::string& AbstractWeather::GetWindDirectionLine() const
{
    float angleWithCorrection = m_wind.m_direction - 11.25f;
    int index = static_cast<int>(std::ceil(angleWithCorrection / 22.5f)) % LanguagePack::csm_compassPoints;
    units::CompassRoseUnits direction = static_cast<units::CompassRoseUnits>(index);
    return m_langPack.GetCompassRoseName(direction);
}

std::string AbstractWeather::GetWindSpeedLine() const
{
    float speed = 0.0f;
    switch (m_langPack.GetSpeedUnit())
    {
    case units::SpeedUnits::SU_KILOMETERSPERHOUR:
        speed = to_KilometersPerHour();
        break;
    case units::SpeedUnits::SU_METERSPERSECOND:
        speed = to_MetersPerSecond();
        break;
    case units::SpeedUnits::SU_MILESPERHOUR:
        speed = to_MilesPerHour();
        break;
    case units::SpeedUnits::SU_FEETPERSECOND:
        speed = to_FeetPerSecond();
        break;
    }
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << speed;
    return ss.str() + " " + m_langPack.GetSpeedName();
}

std::string AbstractWeather::GetHumidityLine() const
{
    return std::to_string(m_atmosphere.m_humidity) + " %";
}

std::string AbstractWeather::GetPressureLine() const
{
    float value = 0;
    switch (m_langPack.GetPressureUnit())
    {
    case units::PressureUnits::PU_ATMOSPHERES:
        value = to_Atm();
        break;
    case units::PressureUnits::PU_MILLIBARS:
        value = to_MilliBar();
        break;
    case units::PressureUnits::PU_MILLIMETERSOFMERCURY:
        value = to_MillimeterOfMercury();
        break;
    case units::PressureUnits::PU_INCHESOFMERCURY:
        value = to_InchesOfMercury();
        break;
    }
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << value;
    return ss.str() + " " + m_langPack.GetPressureName();
}

const std::string& AbstractWeather::GetPressureStateLine() const
{
    auto pressureState = units::PressureStateUnits(m_atmosphere.m_pressureState);
    return m_langPack.GetPressureStateName(pressureState);
}

const std::string& AbstractWeather::GetSunriseLine() const
{
    return m_astronomy.m_sunrise;
}

const std::string& AbstractWeather::GetSunsetLine() const
{
    return m_astronomy.m_sunset;
}

const std::string& AbstractWeather::GetPublicationDateLine() const
{
    return m_pubDate;
}



/* AbstractWeather's "changeror" methods */

void AbstractWeather::ChangeSpeedUnitTo(units::SpeedUnits speedUnit)
{
    m_langPack.SetSpeedUnit(speedUnit);
}

void AbstractWeather::ChangePressureUnitTo(units::PressureUnits pressureUnit)
{
    m_langPack.SetPressureUnit(pressureUnit);
}

void AbstractWeather::ChangeTemperatureUnitTo(units::TemperatureUnits temperatureUnit)
{
    m_langPack.SetTemperatureUnit(temperatureUnit);
}

void AbstractWeather::ChangeLanguageTo(LanguagePack::LanguageUnits languageUnit)
{
    m_langPack.SetLanguageUnit(languageUnit);
}

void AbstractWeather::ChangeClockSystemTo24h()
{
    m_astronomy.changeClockSystemTo(Astronomy::ClockSystemUnits::THE24HOURCLOCK);
}

void AbstractWeather::ChangeClockSystemTo12h()
{
    m_astronomy.changeClockSystemTo(Astronomy::ClockSystemUnits::THE12HOURCLOCK);
}



/* Private additional AbstractWeather's "changeror" methods */

float AbstractWeather::to_KilometersPerHour() const { return m_wind.m_speed; }
float AbstractWeather::to_MetersPerSecond() const   { return m_wind.m_speed * 0.277778f; }
float AbstractWeather::to_MilesPerHour() const      { return m_wind.m_speed * 0.621427f; }
float AbstractWeather::to_FeetPerSecond() const     { return m_wind.m_speed * 0.911344f; }

float AbstractWeather::to_Atm() const                   { return m_atmosphere.m_pressure; }
float AbstractWeather::to_MilliBar() const              { return m_atmosphere.m_pressure * 1013.25f; }
float AbstractWeather::to_MillimeterOfMercury() const   { return m_atmosphere.m_pressure * 760.000f; }
float AbstractWeather::to_InchesOfMercury() const       { return m_atmosphere.m_pressure * 29.9213f; }
