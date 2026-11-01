#include "prx/libcairo/include/Api.hpp"
#include <cmath>
#include <stdexcept>

namespace {

void MatrixInitScale(cairo_matrix_t* matrix, double sx, double sy) {
    cairo_matrix_init(matrix, sx, 0.0, 0.0, sy, 0.0, 0.0);
}

void MatrixInitRotate(cairo_matrix_t* matrix, double radians) {
    double s = std::sin(radians);
    double c = std::cos(radians);
    cairo_matrix_init(matrix, c, s, -s, c, 0.0, 0.0);
}

}

extern "C" {

void APS5_VABI cairo_matrix_init(cairo_matrix_t* matrix, double xx, double yx, double xy, double yy, double x0, double y0) {
    if (!matrix) throw std::invalid_argument("cairo_matrix_init: null matrix");
    matrix->xx = xx;
    matrix->yx = yx;
    matrix->xy = xy;
    matrix->yy = yy;
    matrix->x0 = x0;
    matrix->y0 = y0;
}

void APS5_VABI cairo_matrix_init_identity(cairo_matrix_t* matrix) {
    cairo_matrix_init(matrix, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
}

void APS5_VABI cairo_matrix_init_translate(cairo_matrix_t* matrix, double tx, double ty) {
    cairo_matrix_init(matrix, 1.0, 0.0, 0.0, 1.0, tx, ty);
}

void APS5_VABI cairo_matrix_translate(cairo_matrix_t* matrix, double tx, double ty) {
    if (!matrix) throw std::invalid_argument("cairo_matrix_translate: null matrix");
    cairo_matrix_t translation;
    cairo_matrix_init_translate(&translation, tx, ty);
    cairo_matrix_multiply(matrix, &translation, matrix);
}

void APS5_VABI cairo_matrix_scale(cairo_matrix_t* matrix, double sx, double sy) {
    if (!matrix) throw std::invalid_argument("cairo_matrix_scale: null matrix");
    cairo_matrix_t scale;
    MatrixInitScale(&scale, sx, sy);
    cairo_matrix_multiply(matrix, &scale, matrix);
}

void APS5_VABI cairo_matrix_rotate(cairo_matrix_t* matrix, double radians) {
    if (!matrix) throw std::invalid_argument("cairo_matrix_rotate: null matrix");
    cairo_matrix_t rotation;
    MatrixInitRotate(&rotation, radians);
    cairo_matrix_multiply(matrix, &rotation, matrix);
}

cairo_status_t APS5_VABI cairo_matrix_invert(cairo_matrix_t* matrix) {
    if (!matrix) throw std::invalid_argument("cairo_matrix_invert: null matrix");
    double a = matrix->xx;
    double b = matrix->yx;
    double c = matrix->xy;
    double d = matrix->yy;
    double det = a * d - b * c;
    if (!std::isfinite(det) || det == 0.0) {
        return CAIRO_STATUS_INVALID_MATRIX;
    }
    double tx = matrix->x0;
    double ty = matrix->y0;
    double inv_det = 1.0 / det;
    matrix->xx = d * inv_det;
    matrix->yx = -b * inv_det;
    matrix->xy = -c * inv_det;
    matrix->yy = a * inv_det;
    matrix->x0 = (c * ty - d * tx) * inv_det;
    matrix->y0 = (b * tx - a * ty) * inv_det;
    return CAIRO_STATUS_SUCCESS;
}

void APS5_VABI cairo_matrix_multiply(cairo_matrix_t* result, const cairo_matrix_t* a, const cairo_matrix_t* b) {
    if (!result || !a || !b) throw std::invalid_argument("cairo_matrix_multiply: null argument");
    cairo_matrix_t r;
    r.xx = a->xx * b->xx + a->yx * b->xy;
    r.yx = a->xx * b->yx + a->yx * b->yy;
    r.xy = a->xy * b->xx + a->yy * b->xy;
    r.yy = a->xy * b->yx + a->yy * b->yy;
    r.x0 = a->x0 * b->xx + a->y0 * b->xy + b->x0;
    r.y0 = a->x0 * b->yx + a->y0 * b->yy + b->y0;
    *result = r;
}

void APS5_VABI cairo_matrix_transform_distance(const cairo_matrix_t* matrix, double* dx, double* dy) {
    if (!matrix || !dx || !dy) throw std::invalid_argument("cairo_matrix_transform_distance: null argument");
    double new_x = matrix->xx * (*dx) + matrix->xy * (*dy);
    double new_y = matrix->yx * (*dx) + matrix->yy * (*dy);
    *dx = new_x;
    *dy = new_y;
}

void APS5_VABI cairo_matrix_transform_point(const cairo_matrix_t* matrix, double* x, double* y) {
    if (!matrix || !x || !y) throw std::invalid_argument("cairo_matrix_transform_point: null argument");
    cairo_matrix_transform_distance(matrix, x, y);
    *x += matrix->x0;
    *y += matrix->y0;
}

}
