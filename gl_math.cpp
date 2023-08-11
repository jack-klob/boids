#include "gl_math.h"
#include <cmath>

using namespace boost::numeric::ublas;

mat4 make_ortho(const float left, const float right, const float bottom, const float top)
{
    const float near = -1.0f;
    const float far = 1.0f;
    mat4 m(4, 4);
    m(0, 0) = 2.0f / (right - left);
    m(0, 1) = 0.f;
    m(0, 2) = 0.f;
    m(0, 3) = -(right + left) / (right - left);

    m(1, 0) = 0.f;
    m(1, 1) = 2.0f / (top - bottom);
    m(1, 2) = 0.f;
    m(1, 3) = -(top + bottom) / (top - bottom);

    m(2, 0) = 0.f;
    m(2, 1) = 0.f;
    m(2, 2) = -2.f / (far - near);
    m(2, 3) = -(far + near) / (far - near);

    m(3, 0) = 0.f;
    m(3, 0) = 0.f;
    m(3, 0) = 0.f;
    m(3, 3) = 1.f;

    return m;
}

mat2 rotation_matrix(const float theta)
{
    mat2 m(2, 2);
    float cos_theta = cos(theta);
    float sin_theta = sin(theta);

    m(0, 0) = cos_theta;
    m(0, 1) = -sin_theta;
    m(1, 0) = sin_theta;
    m(1, 1) = cos_theta;

    return m;
}
