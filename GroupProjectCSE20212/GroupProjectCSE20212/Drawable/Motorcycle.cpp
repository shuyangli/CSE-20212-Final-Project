//
//  Motorcycle.cpp
//  GroupProjectCSE20212
//
//  Created by Yuxuan Chen on 3/28/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include "Motorcycle.h"
#include "Sample.h"
#include <iostream>
#include "Drawable.h"
#include "Obstacle.h"
#include "Skybox.h"
#include "Track.h"

Motorcycle::Motorcycle(GLuint       givenVertexBuffer,
                       GLint        givenVertexBufferLoc,
                       unsigned int givenVertexCount,
                       GLuint       givenNormalBuffer,
                       GLint        givenNormalBufferLoc,
                       GLuint       givenIndexBuffer)
{
    // setup initial model matrix as identity matrix
    scaleMatrix = glm::mat4(1.0f);
    setModelMatrix(scaleMatrix);
    
    rotateSpeed = 0.0;
    rotateAngle = 0.0;
    
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
    
    // Set timer to 0
    setTime(0);

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

 
void Motorcycle::setTime(double t)
{
    time = t;
}

double Motorcycle::getTime() const
{
    return time;
}


 
