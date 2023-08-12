#ifndef flock_hpp
#define flock_hpp

#include "gl_math.h"
#include <vector>
#include <GL/glew.h>

class Flock
{
public:

    Flock(unsigned int count, unsigned int seed = 0);

    void update(float dt);

    void draw() const;

private:
    void create_data();

    void wrap(unsigned int i);
    
    void nudge_inside_margin(unsigned int i, GLfloat nudge_factor);

    void apply_rules_to_boid(unsigned int i, GLfloat max_speed, GLfloat max_force);

    void update_draw_data() const;

    bool within_sight(unsigned int source, unsigned int other, GLfloat sight_distance, GLfloat sight_angle) const;

private:
    unsigned int count_;
    std::vector<vec2> positions_;
    std::vector<vec2> velocities_;
    std::vector<vec2> accelerations_;
    std::vector<mat2> rotations_;
    GLuint trans_buffer_, rotation_buffer_;
};

#endif