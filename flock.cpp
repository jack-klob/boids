#include "flock.h"
#include "boost/numeric/ublas/io.hpp"
#include <random>
#include <iostream>

// create buffer for drawing the boids
void Flock::createData()
{
    // defining the shape for a boid
    GLfloat base_tri[] = 
    {
        -10.f, -15.f,
         10.f, -15.f,
         0.f,   10.f,
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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    glVertexAttribDivisor(1, 1);
}

void Flock::draw() const
{
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count_);
}

void Flock::updateDrawData() const
{
    glNamedBufferSubData(trans_buffer_, 0, count_ * sizeof(vec2), positions_.data());
}

void Flock::update()
{
    // for every boid in flock, update position with velocity
    for (unsigned int i = 0; i < count_; ++i)
    {
        positions_[i](0) += velocities_[i](0);
        positions_[i](1) += velocities_[i](1);
    }

    updateDrawData();
}

Flock::Flock(unsigned int count, unsigned int seed) : count_(count), positions_(count), velocities_(count)
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
        positions_[i](0) = 800.f * rng(generator);
        positions_[i](1) = 800.f * rng(generator);

        // random starting velocities in range [0, .5]
        velocities_[i](0) = 0.5f * rng(generator);
        velocities_[i](1) = 0.5f * rng(generator);
    }

    createData();
}