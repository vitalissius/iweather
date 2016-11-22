#pragma once

#include <exception>
#include <string>

class WeatherException : public std::exception {
public:
    WeatherException(const char* message)
        : std::exception(message)
    {}
};

class IncorrectForecastSize : public WeatherException {
public:
    IncorrectForecastSize(const char* message)
        : WeatherException(message)
    {}
};
