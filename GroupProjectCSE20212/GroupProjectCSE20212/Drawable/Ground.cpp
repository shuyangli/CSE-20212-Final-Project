//
//  Ground.cpp
//  GroupProjectCSE20212
//
//  Created by Shuyang Li on 04/23/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include "Ground.h"
#include "Constants.h"
#include "Drawable.h"

Ground::Ground(GLint        givenVertexBufferLoc,
               GLint        givenNormalBufferLoc) {
    
    GLfloat arrayBufferData[12] = {
        -1500.0f, -1.0f, -1500.0f,
        -1500.0f, -1.0f, +1500.0f,
        +1500.0f, -1.0f, -1500.0f,
        +1500.0f, -1.0f, +1500.0f
    }
    
    GLfloat normalBufferData[12] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    }
    
    GLuint indexBufferData[6] = {
        0, 2, 3,
        3, 1, 0
    }
    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(arrayBufferData),
                 arrayBufferData,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(normalBufferData),
                 normalBufferData,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(indexBufferData),
                 indexBufferData,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    vertexCount = 6;
    
    // setup initial model matrix
    scaleMatrix = glm::mat4(1.0f);
    setModelMatrix(scaleMatrix);
    
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

Ground::~Ground() {
    
    // clean up vertex array, which is generated in the constructor
    glDeleteVertexArraysAPPLE(1, &vertexArrayObjectHandle);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &indexBuffer);
}

void Ground::draw() {
    glBindVertexArrayAPPLE(vertexArrayObjectHandle);
    glDrawElements(GL_TRIANGLES,
                   vertexCount,
                   GL_UNSIGNED_INT,
                   0);
    glBindVertexArrayAPPLE(0);
}

drawableObjectType_t Ground::type() {
    return kDrawableObjectTypeSample;
}
