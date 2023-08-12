#include "flock.h"
#include <random>
#include <iostream>

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

vec2 Flock::alignment_contribution(unsigned int i, const std::vector<unsigned int>& neighbors) const
{
    vec2 total_heading{0, 0};
    if(!neighbors.size())
        return total_heading;

    for (auto n : neighbors)
    {
        total_heading += velocities_[n];
    }

    // vec2 steer_alignment = avg_heading;
    // steer_alignment.normalize();
    // steer_alignment *= max_speed;
    // steer_alignment -= velocities_[i];
    // steer_alignment.limit(max_force);

    auto avg_heading = total_heading / neighbors.size();
    //auto nudge = avg_heading - velocities_[i];
    //return nudge.normalize();
    return avg_heading;
}

vec2 Flock::cohesion_contribution(unsigned int i, const std::vector<unsigned int> &neighbors) const
{
    vec2 total_position{0, 0};
    if (!neighbors.size())
        return total_position;

    for (auto n : neighbors)
    {
        total_position += velocities_[n];
    }

    auto avg_position = total_position / neighbors.size();
    auto nudge = avg_position - positions_[i];
    nudge.normalize();
    //auto nudge = avg_position - positions_[i];
    //nudge -= velocities_[i];
    //return nudge.normalize();
    return nudge;
}

vec2 Flock::separation_contribution(unsigned int i, const std::vector<unsigned int>& neighbors, GLfloat separation_distance) const
{
    vec2 avoidance{0, 0};
    unsigned int avoid_count = 0;
    for(auto n : neighbors)
    {
        vec2 distance_vec = positions_[i] - positions_[n];
        if (distance_vec.squared_mag() < separation_distance * separation_distance)
        {
            auto distance = distance_vec.mag();
            distance_vec.normalize();
            avoidance += distance_vec / distance;
            ++avoid_count;
        }
    }

    if (avoid_count)
    {
        avoidance /= avoid_count;
    }

    return avoidance;
}

// void Flock::apply_rules_to_boid(unsigned int i, GLfloat max_speed, GLfloat max_force)
// {
//     std::vector<unsigned int> neighbors{};
//     GLfloat sight_distance = 60.f;
//     GLfloat sight_angle = 45.f;
//     GLfloat separation_distance = 60.f;

//     // find all neighbors of the given boid within the sight distance and angle
//     for (unsigned int j = 0; j < count_; ++j)
//     {
//         if (j == i)
//             continue;

//         if (within_sight(i, j, sight_distance, sight_angle))
//         {
//             neighbors.push_back(j);
//         }
//     }

//     if (neighbors.size())
//     {
//         // implement alignment
//         vec2 avg_heading{0, 0};
//         vec2 avg_pos{0, 0};
//         vec2 avoidance{0, 0};
//         unsigned int avoid_count = 0;
//         for (auto n : neighbors)
//         {
//             avg_heading += velocities_[n];
//             avg_pos += positions_[n];

//             vec2 distance_vec = positions_[i] - positions_[n];
//             if (distance_vec.squared_mag() < separation_distance * separation_distance)
//             {
//                 auto distance = distance_vec.mag();
//                 distance_vec.normalize();
//                 avoidance += distance_vec / distance;
//                 ++avoid_count;
//             }
//         }

        

//         avg_heading /= neighbors.size();
//         avg_pos /= neighbors.size();

//         GLfloat alignment_factor = 0.8f;
//         GLfloat cohesion_factor = 0.8f;
//         GLfloat avoidance_factor = 0.8f;

//         vec2 steer_alignment = avg_heading;
//         steer_alignment.normalize();
//         steer_alignment *= max_speed;
//         steer_alignment -= velocities_[i];
//         steer_alignment.limit(max_force);

//         vec2 steer_cohesion = avg_pos;
//         steer_cohesion -= positions_[i];
//         steer_cohesion.normalize();
//         steer_cohesion *= max_speed;
//         steer_cohesion -= velocities_[i];
//         steer_cohesion.limit(max_force);

//         accelerations_[i] += steer_alignment * alignment_factor;
//         accelerations_[i] += steer_cohesion * cohesion_factor;
//         accelerations_[i] += avoidance * avoidance_factor;
//     }

//     // if the boid is out of the screen, have it want to come to the center
//     // implement a margin from the outside of the screen
//     // boids outside margin are nudged back inside
//     nudge_inside_margin(i, 6.f);
//     //wrap(i);
// }

// void Flock::apply_rules_to_boid(unsigned int i, GLfloat max_speed, GLfloat max_force)
// {
//     std::vector<unsigned int> neighbors{};
//     GLfloat sight_distance = 60.f;
//     GLfloat sight_angle = 45.f;
//     GLfloat separation_distance = 60.f;

//     // find all neighbors of the given boid within the sight distance and angle
//     for (unsigned int j = 0; j < count_; ++j)
//     {
//         if (j == i)
//             continue;

//         if (within_sight(i, j, sight_distance, sight_angle))
//         {
//             neighbors.push_back(j);
//         }
//     }
    
//     GLfloat alignment_factor = .5f;
//     GLfloat cohesion_factor = .5f;
//     GLfloat avoidance_factor = 50.f;

//     vec2 alignment = alignment_contribution(i, neighbors) * alignment_factor;
//     vec2 cohesion = cohesion_contribution(i, neighbors) * cohesion_factor;
//     vec2 separation = separation_contribution(i, neighbors, separation_distance) * avoidance_factor;

//     nudge_inside_margin(i, 50.f);

//     velocities_[i] += alignment * alignment_factor;
//     velocities_[i] += cohesion * cohesion_factor;
//     velocities_[i] += separation * avoidance_factor;
//     velocities_[i].limit(max_speed);

//     //wrap(i);
// }

void Flock::apply_rules_to_boid(unsigned int i, GLfloat max_speed, GLfloat max_force)
{
    std::vector<unsigned int> neighbors{};
    GLfloat sight_distance = 60.f;
    GLfloat sight_angle = 45.f;
    GLfloat separation_distance = 25.f;

    vec2 avg_pos{0, 0};
    vec2 avg_heading{0, 0};
    vec2 close{0, 0};

    // find all neighbors of the given boid within the sight distance and angle
    for (unsigned int j = 0; j < count_; ++j)
    {
        if (j == i)
            continue;

        if (within_sight(i, j, sight_distance, sight_angle))
        {
            neighbors.push_back(j);
        }
    }

    for(auto n : neighbors)
    {
        auto dist = positions_[i] - positions_[n];
        if (dist.squared_mag() <= separation_distance * separation_distance)
        {
            close += dist;
        }

        avg_pos += positions_[n];
        avg_heading += velocities_[n];
    }

    GLfloat alignment_factor = 100.f;
    GLfloat cohesion_factor = 100.f;
    GLfloat avoidance_factor = 100000.f;
    if(neighbors.size())
    {
        avg_pos /= neighbors.size();
        avg_heading /= neighbors.size();

        velocities_[i] += ((avg_pos - positions_[i]).normalize() * cohesion_factor).limit(max_force);
        velocities_[i] += ((avg_heading - velocities_[i]).normalize() * alignment_factor).limit(max_force);
        velocities_[i] += (close.normalize() * avoidance_factor).limit(max_force);
    }

    nudge_inside_margin(i, 7.f);
    //wrap(i);

    auto speed = velocities_[i].mag();
    if(speed < 200.f)
    {
        velocities_[i].normalize();
        velocities_[i] *= 200.f;
    }
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
    glNamedBufferSubData(trans_buffer_, 0, count_ * sizeof(vec2), positions_.data());
    glNamedBufferSubData(rotation_buffer_, 0, count_ * sizeof(mat2), rotations_.data());
}

// determine if the other boid can be seen by the source boid based on its sight parameters
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
    GLfloat max_speed = 200.f;
    GLfloat max_force = 10.f;

    // update all forces acting on each boid
    for (unsigned int i = 0; i < count_; ++i)
    {
        apply_rules_to_boid(i, max_speed, max_force);
    }

    // apply forces to each boid
    for (unsigned int i = 0; i < count_; ++i)
    {
        // velocities_[i] += accelerations_[i];
        // velocities_[i].limit(max_speed);
        
        rotations_[i] = rotation_matrix(std::atan2(velocities_[i][1], velocities_[i][0]));

        positions_[i] += velocities_[i] * dt;
        //accelerations_[i] *= 0.f;
    }

    update_draw_data();
}

Flock::Flock(unsigned int count, unsigned int seed) : 
    count_(count), positions_(count), velocities_(count), rotations_(count), accelerations_(count)
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

void Flock::wrap(unsigned int i)
{
    auto &px = positions_[i][0];
    auto &py = positions_[i][1];

    if(px < 0)
        px = 800.f;
    else if(px > 800)
        px = 0.f;
    else if(py < 0)
        py = 800.f;
    else if(py > 800)
        py = 0.f;
}

void Flock::nudge_inside_margin(unsigned int i, GLfloat nudge_factor)
{
    auto px = positions_[i][0];
    auto py = positions_[i][1];

    vec2 nudge{0, 0};
    if(px < 250)
        nudge[0] = 1;
    else if(px > 550)
        nudge[0] = -1;

    if(py < 250)
        nudge[1] = 1;
    else if(py > 550)
        nudge[1] = -1;

    velocities_[i] += nudge.normalize() * nudge_factor;
}
