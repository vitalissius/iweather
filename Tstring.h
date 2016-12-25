#pragma once

#include <tchar.h>

#include <string>

using tstring = std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>>;

template<typename T>
tstring to_tstring(T t)
{
#if defined UNICODE || _UNICODE
    return std::to_wstring(t);
#else
    return std::to_string(t);
#endif
}