#include "prx/libcurl/include/Api.hpp"
#include <cstddef>
#include <stdexcept>

namespace {

unsigned char RawToUpper(char c) {
    auto u = static_cast<unsigned char>(c);
    return (u >= 'a' && u <= 'z') ? static_cast<unsigned char>(u - 'a' + 'A') : u;
}

}

extern "C" {

int APS5_VABI curl_strequal(const char* s1, const char* s2) {
    if (!s1 || !s2) throw std::invalid_argument("curl_strequal: null string");
    while (*s1 && *s2) {
        if (RawToUpper(*s1) != RawToUpper(*s2)) {
            return 0;
        }
        ++s1;
        ++s2;
    }
    return *s1 == *s2 ? 1 : 0;
}

int APS5_VABI curl_strnequal(const char* s1, const char* s2, std::size_t n) {
    if (!s1 || !s2) throw std::invalid_argument("curl_strnequal: null string");
    while (*s1 && *s2 && n) {
        if (RawToUpper(*s1) != RawToUpper(*s2)) {
            break;
        }
        --n;
        ++s1;
        ++s2;
    }
    if (n == 0) {
        return 1;
    }
    return RawToUpper(*s1) == RawToUpper(*s2) ? 1 : 0;
}

}
