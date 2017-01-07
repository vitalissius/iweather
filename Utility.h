#pragma once

#include <tchar.h>

#include <string>
#include <sstream>

using tstring = std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>>;

using tstringstream = std::basic_stringstream<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>>;

template<typename T>
tstring to_tstring(T t)
{
#if defined UNICODE || _UNICODE
    return std::to_wstring(t);
#else
    return std::to_string(t);
#endif
}



class Noncopyable {
protected:
    Noncopyable() = default;

    ~Noncopyable() = default;

    Noncopyable(const Noncopyable&) = delete;

    Noncopyable& operator=(const Noncopyable&) = delete;
};
