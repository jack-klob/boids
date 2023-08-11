#ifndef gl_math_hpp
#define gl_math_hpp

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/io.hpp"
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/storage.hpp>

using vec2 = boost::numeric::ublas::bounded_vector<float, 2>;
// matrix is stored in column major order as openGL expects
using mat2 = boost::numeric::ublas::bounded_matrix<float, 2, 2, boost::numeric::ublas::column_major>;
using mat4 = boost::numeric::ublas::bounded_matrix<float, 4, 4, boost::numeric::ublas::column_major>;

mat4 make_ortho(const float left, const float right, const float bottom, const float top);

mat2 rotation_matrix(const float theta);

#endif