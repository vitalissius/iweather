#pragma once

#include <exception>
#include <string>

class WeatherException : public std::exception {
public:
    WeatherException() = default;
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

class IncorrectTableName : public WeatherException {
public:
    IncorrectTableName() = default;
    IncorrectTableName(const char* message)
        : WeatherException(message)
    {}
};

class SocketError : public WeatherException {
public:
    SocketError(const char* message)
        : WeatherException(message)
    {}
};

class IncorrectFile : public WeatherException {
public:
    IncorrectFile(const char* message)
        : WeatherException(message)
    {}
};
