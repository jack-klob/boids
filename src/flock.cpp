#include "flock.h"
#include <random>
#include <iostream>

void Flock::create_draw_data()
{
    // create vertex array object to store state
    // this will be the only vertex array object
    // so only need to bind once
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // defining the shape for a boid
    // facing to the right so rotation angles do not need adjusting
     GLfloat base_tri[] = 
    {
        -7.5f,  5.f,
        -7.5f, -5.f,
         5.f,   0.f,
    };
    
    // base shape buffer
    GLuint tri_buffer;
    glGenBuffers(1, &tri_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, tri_buffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), base_tri, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // store layout at location 0 for shader
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    // position buffer
    glGenBuffers(1, &pos_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, pos_buffer_);
    glBufferData(GL_ARRAY_BUFFER, count_ * sizeof(vec2), positions_.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(1); // store layout at location 1 for shader
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 0);
    glVertexAttribDivisor(1, 1); // every position will be used for a single base shape

    // rotation buffer
    glGenBuffers(1, &rotation_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, rotation_buffer_);
    glBufferData(GL_ARRAY_BUFFER, count_ * sizeof(mat2), rotations_.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(2); // store layout at location 2 in shader
    glEnableVertexAttribArray(3); // must use two separate layouts for matrix
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mat2), 0);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(mat2), (void*)(sizeof(GLfloat) * 2));
    glVertexAttribDivisor(2, 1); // each rotation used for for a single base shape
    glVertexAttribDivisor(3, 1);
}

// long function but more performant than separate functions for each rule
// where 3 separate loops would be required
void Flock::apply_rules_to_boid(unsigned int i, GLfloat max_speed, GLfloat max_force)
{
    std::vector<unsigned int> neighbors{};
    GLfloat separation_distance = 25.f;

    vec2 avg_pos, avg_heading, repel;

    for (unsigned int j = 0; j < count_; ++j)
    {
        if (j == i)
            continue;

        if (within_sight(i, j, params_.sight_dist, params_.sight_angle))
        {
            neighbors.push_back(j);
        }
    }

    for(auto n : neighbors)
    {
        vec2 dist_vec = positions_[i] - positions_[n];
        auto dist = dist_vec.mag();
        if (dist <= separation_distance * separation_distance)
        {   
            // the repelling force is inversely proportional to the distance
            // closer boids should repel more than ones further away
            repel += dist_vec / dist;
        }

        avg_pos += positions_[n];
        avg_heading += velocities_[n];
    }

    if(neighbors.size())
    {
        avg_pos /= neighbors.size();
        avg_heading /= neighbors.size();

        // apply cohesion
        velocities_[i] += ((avg_pos - positions_[i]).normalize() * 100.f * params_.cohesion_factor).limit(max_force);
        // apply alignment
        velocities_[i] += ((avg_heading - velocities_[i]).normalize() * 100.f * params_.alignment_factor).limit(max_force);
        // apply separation
        velocities_[i] += (repel.normalize() * 100.f * params_.separation_factor).limit(max_force);
    }

    nudge_inside_margin(i, 7.f, max_force);

    auto speed = velocities_[i].mag();
    // enforce minimum speed
    if(speed < 200.f)
    {
        velocities_[i].normalize();
        velocities_[i] *= 200.f;
    }
    // enforce maximum speed
    if(speed > 600.f)
    {
        velocities_[i].normalize();
        velocities_[i] *= 600.f;
    }
}

void Flock::draw() const
{
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count_);
}

void Flock::update_draw_data() const
{
    // update position buffer with positions
    glNamedBufferSubData(pos_buffer_, 0, count_ * sizeof(vec2), positions_.data());
    // update rotation buffer with rotations
    glNamedBufferSubData(rotation_buffer_, 0, count_ * sizeof(mat2), rotations_.data());
}

bool Flock::within_sight(unsigned int source, unsigned int other, GLfloat sight_distance, GLfloat sight_angle) const
{
    auto diff = positions_[source] - positions_[other];
    if(std::abs(diff[0]) <= sight_distance && std::abs(diff[1]) <= sight_distance && positions_[source].angle_between(diff) <= sight_angle)
    {
        if (diff.squared_mag() <= sight_distance * sight_distance)
        {
            return true;
        }
    }

    return false;
}

void Flock::update(float dt)
{
    GLfloat max_speed = 400.f;
    GLfloat max_force = 15.f;

    // update all forces acting on each boid
    for (unsigned int i = 0; i < count_; ++i)
    {
        apply_rules_to_boid(i, max_speed, max_force);
    }

    // apply forces to each boid
    for (unsigned int i = 0; i < count_; ++i)
    {
        rotations_[i] = rotation_matrix(std::atan2(velocities_[i][1], velocities_[i][0]));
        velocities_[i].limit(max_speed);
        positions_[i] += velocities_[i] * dt;
    }

    update_draw_data();
}

Flock::Flock(const parameters &params) : 
    params_(params),count_(params.n), positions_(params.n), velocities_(params.n), rotations_(params.n)
{
    // generate random starting positions for agents in flock
    std::random_device rd;
    std::mt19937 generator(rd());
    if (params_.seed)
        generator.seed(params_.seed);
    
    std::uniform_real_distribution<float> rng(0.f, 1.f);

    // initialize random positions and velocities
    for (unsigned int i = 0; i < count_; ++i)
    {
        // random starting position within window
        positions_[i][0] = params_.width * rng(generator);
        positions_[i][1] = params_.height * rng(generator);

        // random starting value for velocity
        // velocity in pixels/sec
        velocities_[i][0] = 400.f * (2.f * (rng(generator) - 0.5f));
        velocities_[i][1] = 400.f * (2.f * (rng(generator) - 0.5f));
    }

    create_data();
}

void Flock::wrap(unsigned int i)
{
    auto &px = positions_[i][0];
    auto &py = positions_[i][1];

    if(px < 0)
        px = params_.width;
    else if(px > params_.width)
        px = 0.f;
    else if(py < 0)
        py = params_.height;
    else if(py > params_.height)
        py = 0.f;
}

void Flock::nudge_inside_margin(unsigned int i, GLfloat nudge_factor, GLfloat max_force)
{
    auto px = positions_[i][0];
    auto py = positions_[i][1];
    GLfloat margin = 250;

    vec2 nudge{0, 0};
    if(px < margin)
        nudge[0] = 1;
    else if(px > params_.width - margin)
        nudge[0] = -1;

    if(py < margin)
        nudge[1] = 1;
    else if(py > params_.height - margin)
        nudge[1] = -1;

    velocities_[i] += (nudge.normalize() * nudge_factor).limit(max_force);
}
