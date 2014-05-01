//
//  Skybox.cpp
//  GroupProjectCSE20212
//
//  Created by Shuyang Li on 03/31/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include "Skybox.h"

Skybox::Skybox(GLint    givenVertexBufferLoc,
               GLint    givenNormalBufferLoc,
               GLint    givenUVBufferLoc) {
    
    setLoader(new ObjLoader());
    ObjLoader * myLoaderRef = getLoader();
    myLoaderRef -> loadObj(SKYBOX_PATH, MTL_BASEPATH);
    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 myLoaderRef -> getVertices().size() * sizeof(GLfloat),
                 myLoaderRef -> getVertices().data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 myLoaderRef -> getUVs().size() * sizeof(GLfloat),
                 myLoaderRef -> getUVs().data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 myLoaderRef -> getIndices().size() * sizeof(GLuint),
                 myLoaderRef -> getIndices().data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    vertexCount = (unsigned int) myLoaderRef -> getIndices().size();
    
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
    
    // bind UV array
    glEnableVertexAttribArray(givenUVBufferLoc);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(givenUVBufferLoc,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          0);
    
    // bind index array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    
    // finished: unbind vao to clear state
    glBindVertexArrayAPPLE(0);
}

Skybox::~Skybox() {
    glDeleteVertexArraysAPPLE(1, &vertexArrayObjectHandle);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);
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
