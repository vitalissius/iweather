#pragma once

#include <Windows.h>

#include <vector>
#include <string>
#include <locale>
#include <codecvt>

tstring widen(const std::string& str, const std::locale& loc=std::locale(""))
{
    std::wstring wstr(str.length(), 0);
    std::use_facet<std::ctype<wchar_t>>(loc).widen(&str[0], &str[0] + str.length(), &wstr[0]);
    return wstr;
}

std::wstring conv(const std::string& str)
{
    if (str.empty())
        return std::wstring();

    size_t needed = ::MultiByteToWideChar(CP_ACP, 0, str.data(), str.size(), NULL, 0);
    if (needed == 0)
        throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

    std::vector<wchar_t> buffer(needed);
    int converted = ::MultiByteToWideChar(CP_ACP, 0, str.data(), str.size(), &buffer[0], buffer.size());
    if (converted == 0)
        throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

    return std::wstring(&buffer[0], converted);
};

std::string conv(const std::wstring& wstr)
{
    if (wstr.empty())
        return std::string();

    size_t needed = WideCharToMultiByte(CP_ACP, 0, wstr.data(), wstr.size(), NULL, 0, NULL, NULL);
    if (needed == 0)
        throw std::runtime_error("Failed converting UTF-16 string to UTF-8");

    std::vector<char> buffer(needed);
    int converted = WideCharToMultiByte(CP_ACP, 0, wstr.data(), wstr.size(), &buffer[0], buffer.size(), "?", NULL);
    if (converted == 0)
        throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

    return std::string(&buffer[0], converted);
};
