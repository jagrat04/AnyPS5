#include "prx/libcurl/libcurl.hpp"

#include <cstdio>
#include <cstdlib>
#include <source_location>
#include <stdexcept>

namespace {

void Require(bool condition, std::source_location location = std::source_location::current()) {
    if (condition) return;
    std::fprintf(stderr, "libcurl Strequal check failed at line %u\n", location.line());
    std::abort();
}

void TestStrequal() {
    Require(curl_strequal("hello", "hello") == 1);
    Require(curl_strequal("Hello", "hElLo") == 1);
    Require(curl_strequal("HELLO", "hello") == 1);
    Require(curl_strequal("hello", "world") == 0);
    Require(curl_strequal("hello", "hell") == 0);
    Require(curl_strequal("hell", "hello") == 0);
    Require(curl_strequal("", "") == 1);
    Require(curl_strequal("", "a") == 0);
    Require(curl_strequal("a", "") == 0);
    Require(curl_strequal("123", "123") == 1);
    Require(curl_strequal("123", "124") == 0);
}

void TestStrnequal() {
    Require(curl_strnequal("hello", "HEL", 3) == 1);
    Require(curl_strnequal("hello", "HEL", 4) == 0);
    Require(curl_strnequal("hello", "world", 0) == 1);
    Require(curl_strnequal("", "", 0) == 1);
    Require(curl_strnequal("", "", 5) == 1);
    Require(curl_strnequal("abc", "ABC", 10) == 1);
    Require(curl_strnequal("abc", "ABD", 2) == 1);
    Require(curl_strnequal("abc", "ABD", 3) == 0);
    Require(curl_strnequal("", "a", 1) == 0);
    Require(curl_strnequal("a", "", 1) == 0);
}

void TestNullArguments() {
    try {
        curl_strequal(nullptr, "a");
        Require(false);
    } catch (const std::invalid_argument&) {}

    try {
        curl_strequal("a", nullptr);
        Require(false);
    } catch (const std::invalid_argument&) {}

    try {
        curl_strequal(nullptr, nullptr);
        Require(false);
    } catch (const std::invalid_argument&) {}

    try {
        curl_strnequal(nullptr, "a", 1);
        Require(false);
    } catch (const std::invalid_argument&) {}

    try {
        curl_strnequal("a", nullptr, 1);
        Require(false);
    } catch (const std::invalid_argument&) {}

    try {
        curl_strnequal(nullptr, nullptr, 0);
        Require(false);
    } catch (const std::invalid_argument&) {}
}

}

int main() {
    TestStrequal();
    TestStrnequal();
    TestNullArguments();
    std::puts("libcurl strequal tests passed");
}
