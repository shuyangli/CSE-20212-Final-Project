//
//  Sample.cpp
//  GroupProjectCSE20212
//
//  Created by Shuyang Li on 03/29/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include "Sample.h"
#include "Constants.h"

Sample::Sample(GLint        givenVertexBufferLoc,
               GLint        givenNormalBufferLoc){
    
    // load objects
    ObjLoader loader;
    loader.loadObj(CUBE_PATH, MTL_BASEPATH);
    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 loader.getVertices().size() * sizeof(GLfloat),
                 loader.getVertices().data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 loader.getNormals().size() * sizeof(GLfloat),
                 loader.getNormals().data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 loader.getIndices().size() * sizeof(GLuint),
                 loader.getIndices().data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    
    vertexCount = loader.getVertices().size();
    
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
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(givenVertexBufferLoc,
                          3,            // to simplify program, we always use triangles
                          GL_FLOAT,     // type of elements in vertex buffer is GLfloat
                          GL_FALSE,     // not normalized
                          0,            // to simplify program, we keep each object in a homogeneous buffer
                          0);
    
    glEnableVertexAttribArray(givenNormalBufferLoc);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(givenNormalBufferLoc,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          0);
    
    // bind index array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    
    // finished: unbind vao to clear state
    glBindVertexArrayAPPLE(0);
}

Sample::~Sample() {
    // clean up vertex array, which is generated in the constructor
    glDeleteVertexArraysAPPLE(1, &vertexArrayObjectHandle);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &indexBuffer);
}

void Sample::increaseTurn() {
    rotateSpeed += 0.005;
    
}

void Sample::decreaseTurn() {
    rotateSpeed -= 0.005;
}

void Sample::calculateModelMatrix() {
    std::cout << "Sample: calculate model mat" << std::endl;
    rotateAngle += rotateSpeed;
    glm::mat4 tempModel = glm::rotate(scaleMatrix,
                                      rotateAngle,
                                      glm::vec3(0, 1, 0));
    setModelMatrix(tempModel);
}

void Sample::draw() {
    glBindVertexArrayAPPLE(vertexArrayObjectHandle);
    glDrawElements(GL_TRIANGLES,
                   vertexCount,
                   GL_UNSIGNED_INT,
                   0);
    glBindVertexArrayAPPLE(0);
}

drawableObjectType_t Sample::type() {
    return kDrawableObjectTypeSample;
}