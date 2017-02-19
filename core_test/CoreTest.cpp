#include "../core/AccuPlaces.h"
#include "../core/AccuWeather.h"
#include "../core/YahooWeather.h"
#include "../core/YahooPlaces.h"

#include <iostream>
#include <locale>
#include <memory>
#include <string>

auto deleter = [](AbstractPlaces* places) {};

std::unique_ptr<AbstractPlaces, decltype(deleter)> makePlaces(units::WeatherProvider provider=
    units::WeatherProvider::Accuweather);

std::unique_ptr<AbstractWeather> makeWeather(units::WeatherProvider provider=
    units::WeatherProvider::Accuweather);

std::string inputPlace();

units::WeatherProvider chooseProvider();

std::string choosePlace(const AbstractPlaces* places);

LanguagePack::LanguageUnits chooseLanguage();

void printWeather(const std::unique_ptr<AbstractWeather>& weather);

void updatePlaces(const std::unique_ptr<AbstractPlaces, decltype(deleter)>& places);



int main(int argc, char* argv[])
{
    try {
        std::locale::global(std::locale("ukr_ukr.1251"));

        auto provider = chooseProvider();

        auto places = makePlaces(provider);
        updatePlaces(places);

        auto placekey = choosePlace(places.get());

        auto weather = makeWeather(provider);

        auto language = chooseLanguage();
        weather->ChangeLanguageTo(language);
        weather->ChangeClockSystemTo24h();
        weather->ChangePressureUnitTo(units::PressureUnits::PU_MILLIBARS);
        weather->ChangeSpeedUnitTo(units::SpeedUnits::SU_METERSPERSECOND);
        weather->ChangeTemperatureUnitTo(units::TemperatureUnits::TU_CELSIUS);
        weather->Update(placekey);

        printWeather(weather);
    }
    catch (NetworkError& ne)
    {
        std::cout << "Exception: " << ne.what() << ". Check your network connection!\n";
    }
    catch (std::exception& e)
    {
        std::cout << "Some exception. Message: " << e.what() << "\n";
    }
    return 0;
}



std::unique_ptr<AbstractPlaces, decltype(deleter)> makePlaces(units::WeatherProvider provider)
{
    std::unique_ptr<AbstractPlaces, decltype(deleter)> places(nullptr, deleter);
    switch (provider)
    {
        case units::WeatherProvider::Yahoo:
            places.reset(YahooPlaces::Instance());
            break;
        default:
            places.reset(AccuPlaces::Instance());
            break;
    }
    return places;
}

std::unique_ptr<AbstractWeather> makeWeather(units::WeatherProvider provider)
{
    switch (provider)
    {
        case units::WeatherProvider::Yahoo:
            return std::make_unique<YahooWeather>();
        default:
            return std::make_unique<AccuWeather>();
    }
}

std::string inputPlace()
{
    std::cout << "\nInput place to be searching for: ";
    std::string place;
    std::getline(std::cin, place);
    return place;
}

units::WeatherProvider chooseProvider()
{
    std::cout << "\n0) Yahoo\n1) Accuweathern\nChoose provider (index): ";

    std::string choose;
    do {
        std::getline(std::cin, choose);
    } while (!std::regex_match(choose, std::regex("[0-1]")) && (std::cout << "Choose correct provider (index): "));

    return units::WeatherProvider(std::stoi(choose));
}

std::string choosePlace(const AbstractPlaces* places)
{
    for (size_t i = 0; i < places->size(); ++i)
    {
        std::cout << i << ") " << places->at(i).place << "\n";
    }

    std::cout << "\nChoose place (index): ";

    std::string choose;
    do {
        std::getline(std::cin, choose);
        if (std::regex_match(choose, std::regex("[0-9]+")) && (std::stoul(choose) < places->size()))
        {
            break;
        }
    } while (std::cout << "Choose correct place (index): ");

    return places->GetKey((places->begin() + std::stoi(choose))->place);
}

LanguagePack::LanguageUnits chooseLanguage()
{
    std::cout << "\n0) English\n1) Russian\nChoose language (index): ";

    std::string choose;

    do {
        std::getline(std::cin, choose);
    } while (!std::regex_match(choose, std::regex("[0-1]")) && (std::cout << "Choose correct language (index): "));

    return LanguagePack::LanguageUnits(std::stoi(choose));
}

void printWeather(const std::unique_ptr<AbstractWeather>& weather)
{
    static auto prints = [](const char* descr, const std::string& msg) {
        std::cout << std::setw(24) << descr << msg << "\n";
    };
    static auto printss = [](const char* descr, const std::string& msg1, const std::string& msg2) {
        std::cout << std::setw(24) << descr << msg1 << " (" << msg2 << ")" << "\n";
    };
    static auto printi = [](const char* descr, int msg) {
        std::cout << std::setw(24) << descr << msg << "\n";
    };

    prints("\nPublication date: ", weather->GetPublicationDateLine());
    prints("City: ", weather->GetLocationCLine());
    prints("City, country: ", weather->GetLocationCcLine());
    prints("City, country (region): ", weather->GetLocationCcrLine());
    printi("Temp: ", weather->GetConditionTemt());
    prints("Description: ", weather->GetConditionText());
    prints("Humidity: ", weather->GetHumidityLine());
    printss("Pressure: ", weather->GetPressureLine(), weather->GetPressureStateLine());
    printss("Pressure: ", weather->GetWindSpeedLine(), weather->GetWindDirectionLine());
    prints("Sunrise: ", weather->GetSunriseLine());
    prints("Sunset: ", weather->GetSunsetLine());

    std::cout << "Forecast:\nCode:\tDate:\t\tDay:\tHigh:\tLow:\tText:\n";
    for (const auto& forecast : weather->GetForecastVector())
    {
        std::cout << forecast.GetCode()
            << "\t" << forecast.GetDate()
            << "\t" << forecast.GetDayName()
            << "\t" << forecast.GetHighTemp()
            << "\t" << forecast.GetLowTemp()
            << "\t" << forecast.GetDescriptionText() << "\n";
    }
}

void updatePlaces(const std::unique_ptr<AbstractPlaces, decltype(deleter)>& places)
{
    while (true)
    {
        std::string place = inputPlace();
        try {
            places->Update(place);
            break;
        }
        catch (IncorrectFile&)
        {
            std::cout << "\nNot found. Try input another place! ";
            continue;
        }
    }
}
