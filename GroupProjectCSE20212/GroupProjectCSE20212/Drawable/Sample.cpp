//
//  Sample.cpp
//  GroupProjectCSE20212
//
//  Created by Shuyang Li on 03/29/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include "Sample.h"

Sample::Sample(GLuint       givenProgram,
               GLuint       givenVertexBuffer,
               GLint        givenVertexBufferLoc,
               unsigned int givenVertexCount,
               GLuint       givenIndexBuffer) : Drawable(givenProgram), vertexCount(givenVertexCount) {
    
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
    
    // bind index array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, givenIndexBuffer);
    
    // finished: unbind vao to clear state
    glBindVertexArrayAPPLE(0);
}

Sample::~Sample() { }

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