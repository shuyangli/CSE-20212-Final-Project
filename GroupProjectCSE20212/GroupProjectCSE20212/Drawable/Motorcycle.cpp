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
    
    // Load the motorcycle object
    setLoader(new ObjLoader());
    ObjLoader * myLoaderRef = getLoader();
    myLoaderRef -> loadObj(MOTORCYCLE_PATH, MTL_BASEPATH);
    
    // The Blender motorcycle model has four parts to be rendered.
    for (int i = 0; i < 4; ++i) {
    
        glGenBuffers(1, &vertexBuffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER,
                     myLoaderRef -> getVertices(i).size() * sizeof(GLfloat),
                     myLoaderRef -> getVertices(i).data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &normalBuffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER,
                     myLoaderRef -> getNormals(i).size() * sizeof(GLfloat),
                     myLoaderRef -> getNormals(i).data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &indexBuffer[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     myLoaderRef -> getIndices(i).size() * sizeof(GLuint),
                     myLoaderRef -> getIndices(i).data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        
        vertexCount[i] = (unsigned int) myLoaderRef -> getIndices(i).size();
        
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
    
    // setup initial model matrix as identity matrix
    setScaleVector(glm::vec3(1.0f, 1.0f, 1.0f));
    scaleMatrix = glm::scale(glm::mat4(1.0f), getScaleVector()) * glm::mat4(1.0f);
    
    setModelMatrix(scaleMatrix);
    
    // Reset everything
    position = objPosition;
    direction = glm::normalize(objDirection);
    initialDirection = objDirection;
    acceleration = objAcceleration;
    rotation = objRotation;
    speed = 0.0f;
    angleToFront = 0.0f;
    wrappingBoxVertices.push_back(glm::vec2(0.850f, 0.180f));
    wrappingBoxVertices.push_back(glm::vec2(0.850f, -0.180f));
    wrappingBoxVertices.push_back(glm::vec2(-0.155f, -0.180f));
    wrappingBoxVertices.push_back(glm::vec2(-0.155f, 0.180f));
}

Motorcycle::~Motorcycle()
{
    setLoader(nullptr);
    
    // clean up vertex array, which is generated in the constructor
    for (int i = 0; i < 4; ++i) {
        glDeleteVertexArraysAPPLE(1, &vertexArrayObjectHandle[i]);
        glDeleteBuffers(1, &vertexBuffer[i]);
        glDeleteBuffers(1, &normalBuffer[i]);
        glDeleteBuffers(1, &indexBuffer[i]);
    }
}

std::vector<glm::vec2> Motorcycle::getWrappingBox() const {
    return wrappingBoxVertices;
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
    
    glBindVertexArrayAPPLE(0);
}

void Motorcycle::draw(GLint lightIntensityLoc, GLint ambientLightIntensityLoc) {
    
    ObjLoader * myLoaderRef = getLoader();
    
    for (int i = 0; i < 4; ++i) {
        tinyobj::material_t material = myLoaderRef -> getMaterial(i);
        
        glUniform4f(lightIntensityLoc, material.diffuse[0], material.diffuse[1], material.diffuse[2], 1.0f);
        glUniform4f(ambientLightIntensityLoc, material.ambient[0], material.ambient[1], material.ambient[2], 1.0f);
        
        glBindVertexArrayAPPLE(vertexArrayObjectHandle[i]);
        glDrawElements(GL_TRIANGLES,
                       vertexCount[i],
                       GL_UNSIGNED_INT,
                       0);
    }
}

void Motorcycle::move(unsigned int deltaTime) { // Moves the motorcycle
    position += direction * (speed * deltaTime);
    
    // Normalizing the direction vector
    direction = direction / sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    
    // Update the wrapping box.
    // Remember we only consider the horizontal x-z plane.
    glm::vec2 XZdir = glm::vec2(direction.x, direction.z);          // Only consider the direction on x-z plane
    XZdir = XZdir / sqrtf(XZdir.x * XZdir.x + XZdir.y * XZdir.y);   // Normalizing the XZdir
    //std::cout << "XZdir: " << XZdir.x << ", " << XZdir.y << "\n";
    glm::vec2 XZleft = glm::vec2(-XZdir.y, XZdir.x);        // Left direction with respect to the direction vector
    XZleft = XZleft / sqrtf(XZleft.x * XZleft.x + XZleft.y * XZleft.y); // Normalizing the XZleft
    glm::vec2 XZpos = glm::vec2(position.x, position.z);
    //wrappingBoxVertices[0] = XZpos + XZdir * 0.850f + XZleft * 0.180f;
    wrappingBoxVertices[0] = XZpos + XZdir * 1.0f + XZleft * 0.30f;
    wrappingBoxVertices[1] = XZpos + XZdir * 1.0f + XZleft * -0.30f;
    wrappingBoxVertices[2] = XZpos + XZdir * -0.3f + XZleft * -0.30f;
    wrappingBoxVertices[3] = XZpos + XZdir * -0.3f + XZleft * 0.30f;
    
    /*
    std::cout << "ListPlot[{" << std::endl;
    std::cout << "{" << wrappingBoxVertices[0].x << "," << wrappingBoxVertices[0].y << "},"
              << "{" << wrappingBoxVertices[1].x << "," << wrappingBoxVertices[1].y << "},"
              << "{" << wrappingBoxVertices[2].x << "," << wrappingBoxVertices[2].y << "},"
              << "{" << wrappingBoxVertices[3].x << "," << wrappingBoxVertices[3].y << "}}]\n";
     */
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
    // camera location is 0.5 unit back from the position, so that part of the motorcycle can be seen
    return position - direction * 0.5f + glm::vec3(0.0f, 1.0f, 0.0f);
}
