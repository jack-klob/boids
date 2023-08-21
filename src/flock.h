#ifndef flock_hpp
#define flock_hpp

#include "gl_math.h"
#include <vector>
#include <GL/glew.h>

class Flock
{
public:
    /// @brief struct for parameters of simulation
    struct parameters
    {
        float cohesion_factor;
        float alignment_factor;
        float separation_factor;
        int n;
        bool wrap;
        unsigned int seed = 0;
        float sight_dist;
        float sight_angle;
        float separation_dist;
        int height = 800;
        int width = 800;
    };

    /// @brief flock constructor
    /// @param params parameters to be used for the simulation
    Flock(const parameters& params);

    /// @brief update the positions of the flock based on velocities
    /// @param dt time since last update (seconds)
    void update(float dt);

    /// @brief draw the boids on the current window at their current positions
    void draw() const;

private:
    /// @brief create the draw data to be used in the shaders
    void create_draw_data();

    /// @brief update the draw data with current state of the flock
    void update_draw_data() const;

    /// @brief wrap the boids across the screen if they are outside
    /// @param i index of the boid to wrap
    void wrap(unsigned int i);
    
    /// @brief apply a force to nudge boid back inside screen
    /// @param i index of boid to nudge
    void nudge_inside_margin(unsigned int i);

    /// @brief update velocity of boid based on flocking rules
    /// @param i index of boid to apply rules to
    void apply_rules_to_boid(unsigned int i);
    
    /// @brief check if another boid can be seen by the current boid
    /// @param source boid which is looking
    /// @param other boid which is tested
    /// @return true if other boid can be seen
    bool within_sight(unsigned int source, unsigned int other) const;

private:
    const parameters params_;
    unsigned int count_;
    std::vector<vec2> positions_;
    std::vector<vec2> velocities_;
    std::vector<mat2> rotations_;
    GLuint pos_buffer_, rotation_buffer_;
};

#endif