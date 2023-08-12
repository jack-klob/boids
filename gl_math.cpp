#define _USE_MATH_DEFINES
#include "gl_math.h"
#include <cmath>

mat4 make_ortho(const float left, const float right, const float bottom, const float top)
{
    const float near = -1.0f;
    const float far = 1.0f;
    mat4 m{};
    m[0][0] = 2.0f / (right - left);
    m[0][1] = 0.f;
    m[0][2] = 0.f;
    m[0][3] = 0.f;

    m[1][0] = 0.f;
    m[1][1] = 2.0f / (top - bottom);
    m[1][2] = 0.f;
    m[1][3] = 0.f;

    m[2][0] = 0.f;
    m[2][1] = 0.f;
    m[2][2] = -2.f / (far - near);
    m[2][3] = 0.f;

    m[3][0] = -(right + left) / (right - left);
    m[3][1] = -(top + bottom) / (top - bottom);
    m[3][2] = -(far + near) / (far - near);
    m[3][3] = 1.f;

    return m;
}

mat2 rotation_matrix(const float theta)
{
    mat2 m{};
    float cos_theta = cos(theta);
    float sin_theta = sin(theta);

    m[0][0] = cos_theta;
    m[0][1] = sin_theta;
    m[1][0] = -sin_theta;
    m[1][1] = cos_theta;

    return m;
}

vec2::vec2() : data_()
{
    data_[0] = 0.f;
    data_[1] = 0.f;
}

vec2::vec2(const vec2 &other) : data_(other.data_) {}

vec2::vec2(GLfloat x, GLfloat y) : data_()
{
    data_[0] = x;
    data_[1] = y;
}

GLfloat vec2::operator[](int i) const
{
    return data_[i];
}

GLfloat &vec2::operator[](int i)
{
    return data_[i];
}

vec2 &vec2::operator+=(const vec2 rhs)
{
    data_[0] += rhs.data_[0];
    data_[1] += rhs.data_[1];
    return *this;
}

vec2 &vec2::operator-=(const vec2 rhs)
{
    data_[0] -= rhs.data_[0];
    data_[1] -= rhs.data_[1];
    return *this;
}

vec2 &vec2::operator*=(GLfloat rhs)
{
    data_[0] *= rhs;
    data_[1] *= rhs;
    return *this;
}

vec2 &vec2::operator/=(GLfloat rhs)
{
    data_[0] /= rhs;
    data_[1] /= rhs;
    return *this;
}

vec2 vec2::operator+(const vec2 &rhs) const
{
    return vec2(*this) += rhs;
}

vec2 vec2::operator-(const vec2 rhs) const
{
    return vec2(*this) -= rhs;
}

vec2 vec2::operator*(GLfloat rhs) const
{
    return vec2(*this) *= rhs;
}

vec2 vec2::operator/(GLfloat rhs) const
{
    return vec2(*this) /= rhs;
}

vec2 &vec2::limit(GLfloat lim)
{
    if (squared_mag() > lim * lim)
    {
        normalize();
        (*this) *= lim;
    }

    return *this;
}

vec2 &vec2::normalize()
{
    auto length = mag();
    if (length)
        (*this) /= length;

    return *this;
}

GLfloat vec2::dot(const vec2 &other) const
{
    return data_[0] * other.data_[0] + data_[1] * other.data_[1];
}

GLfloat vec2::angle_between(const vec2 &other) const
{
    GLfloat angle = std::acos(dot(other) / mag() / other.mag());
    // convert to degrees
    return angle * 180.f / M_PI;
}

GLfloat vec2::squared_mag() const
{
    return data_[0] * data_[0] + data_[1] * data_[1];
}

GLfloat vec2::mag() const
{
    return std::sqrt(squared_mag());
}