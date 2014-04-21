//
//  Skybox.cpp
//  GroupProjectCSE20212
//
//  Created by Shuyang Li on 03/31/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include "Skybox.h"

Skybox::Skybox(GLuint       givenVertexBuffer,
               GLint        givenVertexBufferLoc,
               unsigned int givenVertexCount,
               GLuint       givenNormalBuffer,
               GLint        givenNormalBufferLoc,
               GLuint       givenIndexBuffer,
               GLuint       givenTextureHandle) : vertexCount(givenVertexCount) {
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
    
    // bind texture
    glBindTexture(GL_TEXTURE_2D, givenTextureHandle);
    
    // finish
    glBindVertexArrayAPPLE(0);
}

Skybox::~Skybox() {
    glDeleteVertexArraysAPPLE(1, &vertexArrayObjectHandle);
}

void Skybox::draw() {
    glBindVertexArrayAPPLE(vertexArrayObjectHandle);
    glDrawElements(GL_TRIANGLES,
                   vertexCount,
                   GL_UNSIGNED_INT,
                   0);
    glBindVertexArrayAPPLE(0);
}
    
drawableObjectType_t Skybox::type() {
    return kDrawableObjectTypeSkybox;
}