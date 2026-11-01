#pragma once

#include "SceTypes.hpp"

typedef struct _cairo_matrix {
    double xx;
    double yx;
    double xy;
    double yy;
    double x0;
    double y0;
} cairo_matrix_t;

using cairo_status_t = int;
constexpr cairo_status_t CAIRO_STATUS_SUCCESS = 0;
constexpr cairo_status_t CAIRO_STATUS_INVALID_MATRIX = 5;

extern "C" {

void APS5_VABI cairo_matrix_init(cairo_matrix_t* matrix, double xx, double yx, double xy, double yy, double x0, double y0);
void APS5_VABI cairo_matrix_init_identity(cairo_matrix_t* matrix);
void APS5_VABI cairo_matrix_init_translate(cairo_matrix_t* matrix, double tx, double ty);
void APS5_VABI cairo_matrix_translate(cairo_matrix_t* matrix, double tx, double ty);
void APS5_VABI cairo_matrix_scale(cairo_matrix_t* matrix, double sx, double sy);
void APS5_VABI cairo_matrix_rotate(cairo_matrix_t* matrix, double radians);
cairo_status_t APS5_VABI cairo_matrix_invert(cairo_matrix_t* matrix);
void APS5_VABI cairo_matrix_multiply(cairo_matrix_t* result, const cairo_matrix_t* a, const cairo_matrix_t* b);
void APS5_VABI cairo_matrix_transform_distance(const cairo_matrix_t* matrix, double* dx, double* dy);
void APS5_VABI cairo_matrix_transform_point(const cairo_matrix_t* matrix, double* x, double* y);

}
