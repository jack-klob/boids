#ifndef flock_hpp
#define flock_hpp

#include "gl_math.h"
#include <vector>
#include <GL/glew.h>

class Flock
{
public:

    Flock(unsigned int count, unsigned int seed = 0);

    bool is_outside_screen(unsigned int i) const;

    void update();

    void draw() const;

private:
    void create_data();

    void update_draw_data() const;

private:
    unsigned int count_;
    std::vector<vec2> positions_;
    std::vector<vec2> velocities_;
    std::vector<mat2> rotations_;
    GLuint trans_buffer_, rotation_buffer_;
};

#endif