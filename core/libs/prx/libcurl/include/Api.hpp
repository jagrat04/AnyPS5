#pragma once

#include <cstddef>
#include "SceTypes.hpp"

extern "C" {

int APS5_VABI curl_strequal(const char* s1, const char* s2);
int APS5_VABI curl_strnequal(const char* s1, const char* s2, std::size_t n);

}
