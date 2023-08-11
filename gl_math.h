#ifndef gl_math_hpp
#define gl_math_hpp

#include <array>
#include <GL/glew.h>

using mat2 = std::array<std::array<GLfloat, 2>, 2>;
using mat4 = std::array<std::array<GLfloat, 4>, 4>;

mat4 make_ortho(const float left, const float right, const float bottom, const float top);

mat2 rotation_matrix(const float theta);

class vec2
{
    public:
        using vec2_data = std::array<GLfloat, 2>;

        vec2();
        vec2(const vec2 &other);
        vec2(GLfloat x, GLfloat y);

        GLfloat operator[](int i) const;
        GLfloat& operator[](int i);

        vec2 &operator+=(const vec2 rhs);
        vec2 &operator-=(const vec2 rhs);
        vec2 &operator*=(GLfloat rhs);
        vec2 &operator/=(GLfloat rhs);

        vec2 operator+(const vec2 &rhs) const;
        vec2 operator-(const vec2 rhs) const;
        vec2 operator*(GLfloat rhs) const;
        vec2 operator/(GLfloat rhs) const;

        GLfloat squared_mag() const;
        GLfloat mag() const;

    private:
        vec2_data data_;
};

#endif