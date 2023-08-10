#include "flock.h"
#include "boost/numeric/ublas/io.hpp"
#include <random>
#include <iostream>

// create buffer for drawing the boids
void Flock::createData() const
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
    GLuint trans_buffer;
    glGenBuffers(1, &trans_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, trans_buffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * count_ * sizeof(GLfloat), positions_.data(), GL_STATIC_DRAW);

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

Flock::Flock(unsigned int count, unsigned int seed) : count_(count), positions_(count)
{
    // generate random starting positions for agents in flock
    std::random_device rd;
    std::mt19937 generator(rd());
    if (seed)
    {
        generator.seed(seed);
    }
    std::uniform_real_distribution<float> rng(0.f, 1.f);

    for (auto &pos : positions_)
    {
        pos(0) = 800.f * rng(generator);
        pos(1) = 800.f * rng(generator);
    }
}