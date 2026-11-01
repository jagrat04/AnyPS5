#include "prx/libcairo/libcairo.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <source_location>
#include <stdexcept>

namespace {

constexpr double Epsilon = 1e-9;

bool ApproxEqual(double a, double b) {
    return std::fabs(a - b) < Epsilon;
}

void Require(bool condition, std::source_location location = std::source_location::current()) {
    if (condition) return;
    std::fprintf(stderr, "libcairo Matrix check failed at line %u\n", location.line());
    std::abort();
}

void TestInitAndIdentity() {
    cairo_matrix_t m{};
    cairo_matrix_init(&m, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
    Require(m.xx == 1.0 && m.yx == 2.0 && m.xy == 3.0 && m.yy == 4.0 && m.x0 == 5.0 && m.y0 == 6.0);

    cairo_matrix_init_identity(&m);
    Require(m.xx == 1.0 && m.yx == 0.0 && m.xy == 0.0 && m.yy == 1.0 && m.x0 == 0.0 && m.y0 == 0.0);

    cairo_matrix_init_translate(&m, 10.0, -20.0);
    Require(m.xx == 1.0 && m.yx == 0.0 && m.xy == 0.0 && m.yy == 1.0 && m.x0 == 10.0 && m.y0 == -20.0);
}

void TestMultiplyAndTransform() {
    cairo_matrix_t a{};
    cairo_matrix_t b{};
    cairo_matrix_t r{};

    cairo_matrix_init_translate(&a, 5.0, 10.0);
    cairo_matrix_init_identity(&b);
    b.xx = 2.0;
    b.yy = 3.0;

    cairo_matrix_multiply(&r, &a, &b);
    double x = 1.0;
    double y = 2.0;
    cairo_matrix_transform_point(&r, &x, &y);
    Require(ApproxEqual(x, (1.0 + 5.0) * 2.0));
    Require(ApproxEqual(y, (2.0 + 10.0) * 3.0));

    double dx = 1.0;
    double dy = 2.0;
    cairo_matrix_transform_distance(&r, &dx, &dy);
    Require(ApproxEqual(dx, 2.0));
    Require(ApproxEqual(dy, 6.0));

    cairo_matrix_multiply(&a, &a, &b);
    Require(ApproxEqual(a.xx, r.xx) && ApproxEqual(a.yy, r.yy) && ApproxEqual(a.x0, r.x0) && ApproxEqual(a.y0, r.y0));
}

void TestTranslateScaleRotate() {
    cairo_matrix_t m{};
    cairo_matrix_init_identity(&m);
    cairo_matrix_scale(&m, 2.0, 4.0);
    cairo_matrix_translate(&m, 3.0, 5.0);

    double x = 1.0;
    double y = 2.0;
    cairo_matrix_transform_point(&m, &x, &y);
    Require(ApproxEqual(x, (1.0 + 3.0) * 2.0));
    Require(ApproxEqual(y, (2.0 + 5.0) * 4.0));

    cairo_matrix_init_identity(&m);
    cairo_matrix_rotate(&m, 3.14159265358979323846 / 2.0);
    x = 1.0;
    y = 0.0;
    cairo_matrix_transform_point(&m, &x, &y);
    Require(ApproxEqual(x, 0.0));
    Require(ApproxEqual(y, 1.0));
}

void TestInvert() {
    cairo_matrix_t m{};
    cairo_matrix_init_identity(&m);
    Require(cairo_matrix_invert(&m) == CAIRO_STATUS_SUCCESS);
    Require(ApproxEqual(m.xx, 1.0) && ApproxEqual(m.yy, 1.0));

    cairo_matrix_init_translate(&m, 7.0, -11.0);
    Require(cairo_matrix_invert(&m) == CAIRO_STATUS_SUCCESS);
    Require(ApproxEqual(m.x0, -7.0) && ApproxEqual(m.y0, 11.0));

    cairo_matrix_init(&m, 1.0, 2.0, 2.0, 4.0, 0.0, 0.0);
    Require(cairo_matrix_invert(&m) == CAIRO_STATUS_INVALID_MATRIX);
}

void TestNullThrows() {
    try {
        cairo_matrix_init(nullptr, 0, 0, 0, 0, 0, 0);
        Require(false);
    } catch (const std::invalid_argument&) {}

    try {
        cairo_matrix_init_identity(nullptr);
        Require(false);
    } catch (const std::invalid_argument&) {}

    try {
        cairo_matrix_init_translate(nullptr, 0, 0);
        Require(false);
    } catch (const std::invalid_argument&) {}

    try {
        cairo_matrix_translate(nullptr, 0, 0);
        Require(false);
    } catch (const std::invalid_argument&) {}

    try {
        cairo_matrix_scale(nullptr, 1, 1);
        Require(false);
    } catch (const std::invalid_argument&) {}

    try {
        cairo_matrix_rotate(nullptr, 0);
        Require(false);
    } catch (const std::invalid_argument&) {}

    try {
        cairo_matrix_invert(nullptr);
        Require(false);
    } catch (const std::invalid_argument&) {}

    cairo_matrix_t m{};
    try {
        cairo_matrix_multiply(nullptr, &m, &m);
        Require(false);
    } catch (const std::invalid_argument&) {}

    try {
        cairo_matrix_transform_distance(nullptr, nullptr, nullptr);
        Require(false);
    } catch (const std::invalid_argument&) {}

    try {
        cairo_matrix_transform_point(nullptr, nullptr, nullptr);
        Require(false);
    } catch (const std::invalid_argument&) {}
}

}

int main() {
    TestInitAndIdentity();
    TestMultiplyAndTransform();
    TestTranslateScaleRotate();
    TestInvert();
    TestNullThrows();
    std::puts("libcairo Matrix tests passed");
}
