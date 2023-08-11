#include "flock.h"
#include "boost/numeric/ublas/io.hpp"
#include <random>
#include <iostream>
#include <cassert>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// create buffer for drawing the boids
void Flock::createData()
{
    // defining the shape for a boid
    // facing to the right so rotation angles do not need adjusting
     GLfloat base_tri[] = 
    {
        -15.f,  10.f,
        -15.f, -10.f,
         10.f,   0.f,
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

void Flock::updateDrawData() const
{
    glNamedBufferSubData(trans_buffer_, 0, count_ * sizeof(vec2), positions_.data());
    glNamedBufferSubData(rotation_buffer_, 0, count_ * sizeof(mat2), rotations_.data());
}

void Flock::update()
{
    for (unsigned int i = 0; i < count_; ++i)
    {
        // update position of boid with veloctity
        positions_[i][0] += velocities_[i][0];
        positions_[i][1] += velocities_[i][1];

        // update rotation of boid
        rotations_[i] = rotation_matrix(std::atan2(velocities_[i][1], velocities_[i][0]));
    }

    updateDrawData();
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

        // random starting velocities in range [0, .5]
        velocities_[i][0] = 1.f * (rng(generator) - 0.5f);
        velocities_[i][1] = 1.f * (rng(generator) - 0.5f);
    }

    createData();
}