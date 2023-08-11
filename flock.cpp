#include "flock.h"
#include <random>

// create buffer for drawing the boids
void Flock::create_data()
{
    // defining the shape for a boid
    // facing to the right so rotation angles do not need adjusting
     GLfloat base_tri[] = 
    {
        -7.5f,  5.f,
        -7.5f, -5.f,
         5.f,   0.f,
    };
    
    // buffer for base triangle
    // data in this buffer will never be updated
    GLuint tri_buffer;
    glGenBuffers(1, &tri_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, tri_buffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), base_tri, GL_STATIC_DRAW);
    // store layout at location 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    // transform buffer
    // positions will be loaded into this buffer for every frame
    glGenBuffers(1, &trans_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, trans_buffer_);
    glBufferData(GL_ARRAY_BUFFER, count_ * sizeof(vec2), positions_.data(), GL_DYNAMIC_DRAW);

    // this says that the layout for "transform" is located at location 1
    // layout = 1 can then be accessed from the shader
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 0);
    glVertexAttribDivisor(1, 1);

    // rotation matrices at location 2 and 3
    glGenBuffers(1, &rotation_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, rotation_buffer_);
    glBufferData(GL_ARRAY_BUFFER, count_ * sizeof(mat2), rotations_.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mat2), 0);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(mat2), (void*)(sizeof(GLfloat) * 2));
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
}

void Flock::draw() const
{
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count_);
}

void Flock::update_draw_data() const
{
    glNamedBufferSubData(trans_buffer_, 0, count_ * sizeof(vec2), positions_.data());
    glNamedBufferSubData(rotation_buffer_, 0, count_ * sizeof(mat2), rotations_.data());
}

// determine if the other boid can be seen by the source boid based on its sight parameters
bool Flock::within_sight(unsigned int source, unsigned int other, GLfloat sight_distance, GLfloat sight_angle) const
{
    auto diff = positions_[source] - positions_[other];
    if(abs(diff[0]) < sight_distance && abs(diff[1]) < sight_distance)
    {
        if (diff.squared_mag() < sight_distance * sight_distance)
        {
            return true;
        }
    }

    return false;
}

void Flock::update(float dt)
{
    for (unsigned int i = 0; i < count_; ++i)
    {

        std::vector<unsigned int> neighbors{};
        GLfloat sight_distance = 40.f;
        GLfloat sight_angle = 360.f;

        // find all neighbors of the given boid within the sight distance and angle
        for (unsigned int j = 0; j < count_; ++j)
        {   
            if(j == i)
                continue;

            if(within_sight(i, j, sight_distance, sight_angle))
            {
                neighbors.push_back(j);
            }
        }

        if(neighbors.size())
        {
            // implement alignment
            vec2 avg_vel{0,0};
            vec2 avg_pos{0, 0};
            vec2 avoidance{0, 0};
            for(auto n : neighbors)
            {
                avg_vel += velocities_[n];
                avg_pos += positions_[n];

                avoidance += positions_[i] - positions_[n];
            }
            avg_vel /= neighbors.size();
            avg_pos /= neighbors.size();

            GLfloat alignment_factor = 0.005f;
            GLfloat cohesion_factor = 0.005f;
            GLfloat avoidance_factor = 0.005f;

            velocities_[i] += (avg_vel - velocities_[i]) * alignment_factor;
            velocities_[i] += (avg_pos - velocities_[i]) * cohesion_factor;
            velocities_[i] += avoidance * avoidance_factor;
        }

        // if the boid is out of the screen, have it want to come to the center
        // implement a margin from the outside of the screen
        // boids outside margin are nudged back inside
        nudge_inside_margin(i, 1);

        // update position of boid with veloctity
        positions_[i] += velocities_[i] * dt;
        // update rotation of boid
        rotations_[i] = rotation_matrix(std::atan2(velocities_[i][1], velocities_[i][0]));
    }

    update_draw_data();
}

Flock::Flock(unsigned int count, unsigned int seed) : count_(count), positions_(count), velocities_(count), rotations_(count)
{
    // generate random starting positions for agents in flock
    std::random_device rd;
    std::mt19937 generator(rd());
    if (seed)
    {
        generator.seed(seed);
    }
    std::uniform_real_distribution<float> rng(0.f, 1.f);

    // initialize random positions and velocities
    for (unsigned int i = 0; i < count_; ++i)
    {
        // random starting position within window
        positions_[i][0] = 800.f * rng(generator);
        positions_[i][1] = 800.f * rng(generator);

        // random starting value for velocity
        // velocity in pixels/sec
        velocities_[i][0] = 400.f * (2.f * (rng(generator) - 0.5f));
        velocities_[i][1] = 400.f * (2.f * (rng(generator) - 0.5f));
    }

    create_data();
}

void Flock::nudge_inside_margin(unsigned int i, GLfloat nudge_factor)
{
    auto px = positions_[i][0];
    auto py = positions_[i][1];

    vec2 nudge{0, 0};
    if(px < 100)
        nudge[0] = 1;
    else if(px > 700)
        nudge[0] = -1;

    if(py < 100)
        nudge[1] = 1;
    else if(py > 700)
        nudge[1] = -1;

    velocities_[i] += nudge * nudge_factor;
}
