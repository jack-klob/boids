#ifndef flock_hpp
#define flock_hpp

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/storage.hpp>
#include <GL/glew.h>

class Flock
{
    using vec2 = boost::numeric::ublas::bounded_vector<float, 2>;

public:

    Flock(unsigned int count, unsigned int seed = 0);

    void update();

    void draw() const;

private:
    void createData();

    void updateDrawData() const;

private:
    unsigned int count_;
    std::vector<vec2> positions_;
    std::vector<vec2> velocities_;
    std::vector<vec2> rotations_;
    GLuint trans_buffer_;
};

#endif