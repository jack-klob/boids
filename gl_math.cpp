#include "gl_math.h"

using namespace boost::numeric::ublas;

matrix<float, row_major> make_ortho(const float left, const float right, const float bottom, const float top)
{
    const float near = -1.0f;
    const float far = 1.0f;
    matrix<float, row_major> m(4, 4, 0);
    m(0, 0) = 2.0f / (right - left);
    m(1, 1) = 2.0f / (top - bottom);
    m(2, 2) = -2.f / (far - near);
    m(3, 3) = 1;

    m(3, 0) = -(right + left) / (right - left);
    m(3, 1) = -(top + bottom) / (top - bottom);
    m(3, 2) = -(far + near) / (far - near);
    return m;
}