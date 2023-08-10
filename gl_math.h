#ifndef gl_math_hpp
#define gl_math_hpp

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/io.hpp"

using namespace boost::numeric::ublas;

boost::numeric::ublas::matrix<float, boost::numeric::ublas::row_major> make_ortho(
    const float left, const float right, const float bottom, const float top);

#endif