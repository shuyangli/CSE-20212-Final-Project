//
//  Motorcycle.cpp
//  GroupProjectCSE20212
//
//  Created by Yuxuan Chen on 3/28/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include "Constants.h"
#include "Motorcycle.h"
#include "Sample.h"
#include "Obstacle.h"
#include "Skybox.h"
#include "Track.h"

Motorcycle::Motorcycle(GLint        givenVertexBufferLoc,
                       GLint        givenNormalBufferLoc,
                       glm::vec3    objPosition,
                       glm::vec3    objDirection,
                       double       objAcceleration,
                       double       objRotation)
{
    
    // load objects
    ObjLoader loader;
    loader.loadObj(MOTORCYCLE_PATH, MTL_BASEPATH);
    
    // four parts!
    for (int i = 0; i < 4; ++i) {
    
        glGenBuffers(1, &vertexBuffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER,
                     loader.getVertices(i).size() * sizeof(GLfloat),
                     loader.getVertices(i).data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &normalBuffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER,
                     loader.getNormals(i).size() * sizeof(GLfloat),
                     loader.getNormals(i).data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &indexBuffer[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     loader.getIndices(i).size() * sizeof(GLuint),
                     loader.getIndices(i).data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        
        vertexCount[i] = (unsigned int) loader.getIndices(i).size();
        
        
        // setup initial model matrix as identity matrix
        scaleMatrix = glm::scale(glm::mat4(1.0f),
                                 glm::vec3(0.2f, 0.2f, 0.2f));
        
        setModelMatrix(scaleMatrix);
        
        // wrap states using vao
        glGenVertexArraysAPPLE(1, &vertexArrayObjectHandle[i]);
        glBindVertexArrayAPPLE(vertexArrayObjectHandle[i]);
        
        // bind vertex array
        glEnableVertexAttribArray(givenVertexBufferLoc);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[i]);
        glVertexAttribPointer(givenVertexBufferLoc,
                              3,            // to simplify program, we always use triangles
                              GL_FLOAT,     // type of elements in vertex buffer is GLfloat
                              GL_FALSE,     // not normalized
                              0,            // to simplify program, we keep each object in a homogeneous buffer
                              0);
        
        glEnableVertexAttribArray(givenNormalBufferLoc);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer[i]);
        glVertexAttribPointer(givenNormalBufferLoc,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              0);
        
        // bind index array
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer[i]);
        
        // finished: unbind vao to clear state
        glBindVertexArrayAPPLE(0);
    }
    
    // Reset everything
    setPosition(objPosition);
    setDirection(objDirection);
    initialDirection = objDirection;
    setAcceleration(objAcceleration);
    setRotation(objRotation);
    setSpeed(0);
    angleToFront = 0.0f;
}

Motorcycle::~Motorcycle()
{
    // clean up vertex array, which is generated in the constructor
    for (int i = 0; i < 4; ++i) {
        glDeleteVertexArraysAPPLE(1, &vertexArrayObjectHandle[i]);
        glDeleteBuffers(1, &vertexBuffer[i]);
        glDeleteBuffers(1, &normalBuffer[i]);
        glDeleteBuffers(1, &indexBuffer[i]);
    }
}

drawableObjectType_t Motorcycle::type() {
    return kDrawableObjectTypeMotorcycle;
}

void Motorcycle::calculateModelMatrix() {
    
    glm::mat4 rotatedModel = glm::rotate(scaleMatrix,
                                        angleToFront,
                                        glm::vec3(0, 1, 0));      // rotate around y-axis
    
    glm::mat4 translatedModel = glm::translate(glm::mat4(1.0f),
                                               getPosition());
    
    setModelMatrix(translatedModel * rotatedModel);
}

void Motorcycle::draw() // Draws the motorcycle
{
    for (int i = 0; i < 4; ++i) {
        glBindVertexArrayAPPLE(vertexArrayObjectHandle[i]);
        glDrawElements(GL_TRIANGLES,
                       vertexCount[i],
                       GL_UNSIGNED_INT,
                       0);
    }
    glBindVertexArrayAPPLE(0);
}

void Motorcycle::move(unsigned int deltaTime) // Moves the motorcycle
{
    std::cout << "moved" << std::endl;
    // Add speed to the position
    setPosition(getPosition() + getDirection() * (getSpeed() * deltaTime));
}

void Motorcycle::turnLeft(unsigned int deltaTime)
{
    glm::vec3 newDir = getDirection();
    float currentAngle = 0;
    std::cout << newDir.x << " " << newDir.y << std::endl;
    if (newDir.x == 0) currentAngle = (newDir.z > 0) ? M_PI / 2 : -M_PI / 2;
    else {
        currentAngle = atanf(newDir.z / newDir.x);
        if (newDir.x < 0) currentAngle = M_PI + currentAngle;
    }
    currentAngle += rotation;
    newDir.x = cosf(currentAngle);
    newDir.z = sinf(currentAngle);
    setDirection(newDir);
    
    angleToFront = currentAngle;
}

void Motorcycle::turnRight(unsigned int deltaTime)
{
    glm::vec3 newDir = getDirection();
    float currentAngle = 0;
    if (newDir.x == 0) currentAngle = (newDir.z > 0) ? M_PI / 2 : -M_PI / 2;
    else {
        currentAngle = atanf(newDir.z / newDir.x);
        if (newDir.x < 0) currentAngle = M_PI + currentAngle;
    }
    currentAngle -= rotation;
    newDir.x = cosf(currentAngle);
    newDir.z = sinf(currentAngle);
    setDirection(newDir);
    
    angleToFront = currentAngle;
}

void Motorcycle::incSpeed(unsigned int deltaTime)
{
    setSpeed(getSpeed() + getAcceleration() * deltaTime);
}

void Motorcycle::decSpeed(unsigned int deltaTime)
{
    setSpeed(getSpeed() - getAcceleration() * deltaTime);
}

glm::vec3 Motorcycle::getCameraFocus() {
    // 20 units in front of motorcycle
#warning should be tweaked
    return getPosition() + glm::vec3(4.0f, 1.0f, 0.0f);
//    return getPosition();
}

glm::vec3 Motorcycle::getCameraLocation() {
    // position of motorcycle, move up 2 units
#warning these should be tweaked
    return getPosition() + glm::vec3(-2.0f, 2.0f, 0);
}

int Motorcycle::isInBounds()           // Determines if the motorcycle is in on the road
{
#warning TODO
    return 1;
}

int Motorcycle::isInCollision()        // Determines if the motorcycle collides with something
{
#warning TODO
    return 0;
}

int Motorcycle::isFinished()           // Determines if the motorcycle gets to the finish line
{
#warning TODO
    return 0;
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




