#pragma once
#include <vector>
#include <string>

namespace StrUtils {
template<typename ResultList, typename CharT>
ResultList Split(const std::basic_string<CharT> &s, const std::basic_string<CharT> &delimiter, bool keepEmptyParts = true)
{
    ResultList slist;
    // If delimiter.empty():
    //   pos = s.find(delimiter, start);
    // pos will be 0.
    if (delimiter.empty()) {
        if (s.size() > 0 || keepEmptyParts)
            slist.insert(slist.end(), s);
        return slist;
    }

    typename std::basic_string<CharT>::size_type start = 0;
    typename std::basic_string<CharT>::size_type pos;

    std::basic_string<CharT> part;

    if (delimiter.length() == 1) {
        CharT ch = delimiter[0];

        // Hope that:
        //   find(CharT ch, size_type pos)
        // will be faster than:
        //   find(std::basic_string<CharT>, size_type pos)
        while ((pos = s.find(ch, start)) != s.npos) {  // Use strstr/wcsstr instead?
            part = s.substr(start, pos - start);
            if (part.size() > 0 || keepEmptyParts)
                slist.insert(slist.end(), part);
            start = pos + delimiter.length();
        }
    } else {
        while ((pos = s.find(delimiter, start)) != s.npos) {
            part = s.substr(start, pos - start);
            if (part.size() > 0 || keepEmptyParts)
                slist.insert(slist.end(), part);
            start = pos + delimiter.length();
        }
    }

    if (start != s.length() || keepEmptyParts)
        slist.insert(slist.end(), s.substr(start));
    return slist;
}

typedef std::vector<std::string> StringVector;
template<typename CharT>
inline StringVector Split(const std::basic_string<CharT> &s, const CharT *delimiter, bool keepEmptyParts = true)
{
    assert(delimiter != nullptr);
    return Split<StringVector>(s, std::basic_string<CharT>(delimiter), keepEmptyParts);
}

}
