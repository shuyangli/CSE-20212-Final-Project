//
//  Motorcycle.cpp
//  GroupProjectCSE20212
//
//  Created by Yuxuan Chen on 3/28/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include <iostream>

#define GLM_FORCE_RADIANS
#include "../Helper/glm/gtx/rotate_vector.hpp"

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
                                 glm::vec3(0.2f, 0.2f, 0.2f)) * glm::mat4(1.0f);
        
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
    position = objPosition;
    setDirection(objDirection);
    
    initialDirection = objDirection;
    acceleration = objAcceleration;
    rotation = objRotation;
    speed = 0.0f;
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
    
    glm::mat4 translatedModel = glm::translate(glm::mat4(1.0f), position);  // move model to position in model space
    
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
    
    std::cout << angleToFront << std::endl;
    
    glBindVertexArrayAPPLE(0);
}

void Motorcycle::move(unsigned int deltaTime) { // Moves the motorcycle
    // Add speed to the position
    position += direction * (speed * deltaTime);
}

void Motorcycle::turnLeft(unsigned int deltaTime) {
    
    angleToFront += rotation;
    direction = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f),
                             angleToFront);
}

void Motorcycle::turnRight(unsigned int deltaTime) {
    angleToFront -= rotation;
    direction = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f),
                             angleToFront);
}

void Motorcycle::incSpeed(unsigned int deltaTime) {
    speed += acceleration * deltaTime;
}

void Motorcycle::decSpeed(unsigned int deltaTime) {
    speed -= acceleration * deltaTime;
}

glm::vec3 Motorcycle::getCameraFocus() {
    // focus is 2 units in front of the camera, and 0.5 units above
    return position + direction * 2.0f + glm::vec3(0.0f, 0.5f, 0.0f);
}

glm::vec3 Motorcycle::getCameraLocation() {
    // camera location is 1 unit back from the position, so that part of the motorcycle can be seen
    return position - direction * 1.0f + glm::vec3(0.0f, 1.0f, 0.0f);
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

void Motorcycle::setDirection(glm::vec3 dir) {
    direction = dir / float(sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z));
}