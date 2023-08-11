#ifndef gl_math_hpp
#define gl_math_hpp

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/io.hpp"
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/storage.hpp>
#include <array>
#include <GL/glew.h>

using bvec2 = boost::numeric::ublas::bounded_vector<GLfloat, 2>;
// matrix is stored in column major order as openGL expects
using bmat2 = boost::numeric::ublas::bounded_matrix<GLfloat, 2, 2, boost::numeric::ublas::column_major>;
using bmat4 = boost::numeric::ublas::bounded_matrix<GLfloat, 4, 4, boost::numeric::ublas::column_major>;

using vec2 = std::array<GLfloat, 2>;
using mat2 = std::array<std::array<GLfloat, 2>, 2>;
using mat4 = std::array<std::array<GLfloat, 4>, 4>;

mat4 make_ortho(const float left, const float right, const float bottom, const float top);

mat2 rotation_matrix(const float theta);

#endif