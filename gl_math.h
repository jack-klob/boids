#ifndef gl_math_hpp
#define gl_math_hpp

#include <array>
#include <GL/glew.h>

using mat2 = std::array<std::array<GLfloat, 2>, 2>;
using mat4 = std::array<std::array<GLfloat, 4>, 4>;

mat4 make_ortho(const float left, const float right, const float bottom, const float top);

mat2 rotation_matrix(const float theta);

/// @brief lightweight 2 element vector class
class vec2
{
    public:
       
        /// @brief vec2 storage
        using vec2_data = std::array<GLfloat, 2>;


        /// @brief default constructor
        vec2();


        /// @brief value constructor
        /// @param x value for x
        /// @param y value for y
        vec2(GLfloat x, GLfloat y);


        /// @brief copy constructor
        /// @param other vec2 to copy
        vec2(const vec2 &other);



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


        /// @brief clamps the magnitude of the vector
        /// @param lim value to clamp to
        /// @return the clamped vector
        vec2 &limit(GLfloat lim);

        /// @brief normalize the vector
        /// @return the normalized vector
        vec2 &normalize();

        /// @brief compute the dot product between two vectors
        /// @param other vector to dot product with
        /// @return the value of the dot product
        GLfloat dot(const vec2 &other) const;
        
        /// @brief compute the angle between two vectors
        /// @param other vector to find to angle with
        /// @return the angle between the two vectors (degrees)
        GLfloat angle_between(const vec2 &other) const;

        /// @brief compute the squared magnitude of a vector
        /// @return the squared magnitude
        GLfloat squared_mag() const;

        /// @brief compute the magnitude of a vector
        /// @return the magnitude
        GLfloat mag() const;

    private:
        vec2_data data_;
};

#endif