#ifndef flock_hpp
#define flock_hpp

#include "gl_math.h"
#include <vector>
#include <GL/glew.h>

struct parameters
{
    float cohesion_factor;
    float alignment_factor;
    float separation_factor;
    int n;
    unsigned int seed = 0;
    float sight_dist;
    float sight_angle;
    int height = 800;
    int width = 800;
};

class Flock
{
public:

    Flock(const parameters& params);

    void update(float dt);

    void draw() const;

private:
    void create_data();

    void wrap(unsigned int i);
    
    void nudge_inside_margin(unsigned int i, GLfloat nudge_factor, GLfloat max_force);

    void apply_rules_to_boid(unsigned int i, GLfloat max_speed, GLfloat max_force);

    void update_draw_data() const;

    bool within_sight(unsigned int source, unsigned int other, GLfloat sight_distance, GLfloat sight_angle) const;

private:
    const parameters params_;
    unsigned int count_;
    std::vector<vec2> positions_;
    std::vector<vec2> velocities_;
    std::vector<mat2> rotations_;
    GLuint pos_buffer_, rotation_buffer_;
};

#endif