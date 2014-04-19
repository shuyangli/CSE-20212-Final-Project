//
//  Motorcycle.cpp
//  GroupProjectCSE20212
//
//  Created by Yuxuan Chen on 3/28/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include <iostream>
#include <ctime>
#include <unistd.h>
#include "Motorcycle.h"
#include "Sample.h"
#include "Obstacle.h"
#include "Skybox.h"
#include "Track.h"

Motorcycle::Motorcycle(GLuint givenVertexBuffer,
                       GLint        givenVertexBufferLoc,
                       unsigned int givenVertexCount,
                       GLuint       givenNormalBuffer,
                       GLint        givenNormalBufferLoc,
                       GLuint       givenIndexBuffer,
                       glm::vec3 objPosition,
                       glm::vec3 objDirection,
                       double objAcceleration,
                       double objRotation)
{
    // setup initial model matrix as identity matrix
    scaleMatrix = glm::mat4(1.0f);
    setModelMatrix(scaleMatrix);
    
    // wrap states using vao
    glGenVertexArraysAPPLE(1, &vertexArrayObjectHandle);
    glBindVertexArrayAPPLE(vertexArrayObjectHandle);
    
    // bind vertex array
    glEnableVertexAttribArray(givenVertexBufferLoc);
    glBindBuffer(GL_ARRAY_BUFFER, givenVertexBuffer);
    glVertexAttribPointer(givenVertexBufferLoc,
                          3,            // to simplify program, we always use triangles
                          GL_FLOAT,     // type of elements in vertex buffer is GLfloat
                          GL_FALSE,     // not normalized
                          0,            // to simplify program, we keep each object in a homogeneous buffer
                          0);
    
    glEnableVertexAttribArray(givenNormalBufferLoc);
    glBindBuffer(GL_ARRAY_BUFFER, givenNormalBuffer);
    glVertexAttribPointer(givenNormalBufferLoc,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          0);
    
    // bind index array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, givenIndexBuffer);
    
    // finished: unbind vao to clear state
    glBindVertexArrayAPPLE(0);
    
    // Reset everything
    setTime();
    setPosition(objPosition);
    setDirection(objDirection);
    setAcceleration(objAcceleration);
    setRotation(objRotation);
    setSpeed(0);
}

Motorcycle::~Motorcycle()
{
    // clean up vertex array, which is generated in the constructor
    glDeleteVertexArraysAPPLE(1, &vertexArrayObjectHandle);
}
    
void Motorcycle::draw() // Draws the motorcycle
{
    glBindVertexArrayAPPLE(vertexArrayObjectHandle);
    glDrawElements(GL_TRIANGLES,
                   vertexCount,
                   GL_UNSIGNED_INT,
                   0);
    glBindVertexArrayAPPLE(0);
}

void Motorcycle::move() // Moves the motorcycle
{
    // Add speed to the position
    setPosition(getPosition() + getDirection() * getSpeed());
    usleep(100);
}

void Motorcycle::turnLeft()
{
    glm::vec3 newDir = getDirection();
    float angle = 0;
    if (newDir.x == 0) angle = (newDir.y > 0) ? M_PI / 2 : -M_PI / 2;
    angle += rotation;
    newDir.x = cosf(angle);
    newDir.y = sinf(angle);
    setDirection(newDir);
    usleep(100);
}

void Motorcycle::turnRight()
{
    glm::vec3 newDir = getDirection();
    float angle = 0;
    if (newDir.x == 0) angle = (newDir.y > 0) ? M_PI / 2 : -M_PI / 2;
    angle -= rotation;
    newDir.x = cosf(angle);
    newDir.y = sinf(angle);
    setDirection(newDir);
    usleep(100);
}

void Motorcycle::incSpeed()
{
    setSpeed(getSpeed() + getAcceleration());
    usleep(100);
}

void Motorcycle::decSpeed()
{
    setSpeed(getSpeed() - getAcceleration());
    usleep(100);
}

int Motorcycle::isInBounds()           // Determines if the motorcycle is in on the road
{
    return 1;
}

int Motorcycle::isInCollision()        // Determines if the motorcycle collides with something
{
    return 0;
}

int Motorcycle::isFinished()           // Determines if the motorcycle gets to the finish line
{
    return 0;
}
 
void Motorcycle::setTime()
{
    time = clock();
}

float Motorcycle::getTime() const
{
    return float(clock() - time) / CLOCKS_PER_SEC;
}

void Motorcycle::setPosition(glm::vec3 pos)
{
    position = pos;
}

glm::vec3 Motorcycle::getPosition() const
{
    return position;
}

void Motorcycle::setDirection(glm::vec3 dir)
{
    direction = dir / float(sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z));
}

glm::vec3 Motorcycle::getDirection() const
{
    return direction;
}

void Motorcycle::setSpeed(double spd)
{
    speed = spd;
}

float Motorcycle::getSpeed() const
{
    return speed;
}

void Motorcycle::setAcceleration(double acc)
{
    acceleration = acc;
}

float Motorcycle::getAcceleration() const
{
    return acceleration;
}

void Motorcycle::setRotation(double rot)
{
    rotation = rot;
}

float Motorcycle::getRotation() const
{
    return rotation;
}




