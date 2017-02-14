#include "Utility.h"

std::string utf8ToCp1251(const std::string& from, const std::locale& loc)
{
    std::wstring w = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(from);
    std::string n(w.size(), '\0');
    std::use_facet<std::ctype<wchar_t>>(loc).narrow(w.data(), w.data() + w.size(), '?', &n[0]);
    return n;
}

std::string trim(std::string str)
{
    std::string::size_type s = 0;
    std::string::size_type e = 0;

    if (!str.empty())
    {
        for (std::string::size_type i = 0; i < str.size(); ++i)
        {
            if (str[i] == ' ') ++s;
            else break;
        }

        for (std::string::size_type ri = str.size() - 1; ri > 0; --ri)
        {
            if (str[ri] == ' ') ++e;
            else break;
        }
    }
    return str.substr(s, str.size() - s - e);
}
