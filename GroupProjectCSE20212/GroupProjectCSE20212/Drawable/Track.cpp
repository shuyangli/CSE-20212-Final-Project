//
//  Track.cpp
//  GroupProjectCSE20212
//
//  Created by Yuxuan Chen on 3/28/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include "Track.h"
#include "Constants.h"
#include "Drawable.h"
#include <vector>

Track::Track(GLint        givenVertexBufferLoc,
             GLint        givenNormalBufferLoc) {
    
    setLoader(new ObjLoader());
    ObjLoader * myLoaderRef = getLoader();
    myLoaderRef -> loadObj(TRACK_PATH, MTL_BASEPATH);
    
    // In this OpenGL program, x-z is the horizontal plane.
    // The following code snippet grabs the x and z coordinates of all the
    // vertices that are at the bottom of the inner walls of the track.
    std::cout << "ListPlot[ { " << std::endl;
    std::vector<GLfloat> vertices = myLoaderRef->getVertices();
    for (int i = 0; i < vertices.size(); i += 3) {
        if (vertices[i + 1] > 1) continue;
        if (i) std::cout << ", ";
        std::cout << "{" << vertices[i] << ", " << vertices[i + 2] << "}";
    }
    std::cout << " } ]" << std::endl;
    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 myLoaderRef -> getVertices().size() * sizeof(GLfloat),
                 myLoaderRef -> getVertices().data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 myLoaderRef -> getNormals().size() * sizeof(GLfloat),
                 myLoaderRef -> getNormals().data(),
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
    
    // setup initial model matrix
    scaleMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f)) * glm::mat4(1.0f);
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

Track::~Track() {
    
    setLoader(nullptr);
    
    // clean up vertex array, which is generated in the constructor
    glDeleteVertexArraysAPPLE(1, &vertexArrayObjectHandle);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &indexBuffer);
}

void Track::draw() {
    glBindVertexArrayAPPLE(vertexArrayObjectHandle);
    glDrawElements(GL_TRIANGLES,
                   vertexCount,
                   GL_UNSIGNED_INT,
                   0);
    glBindVertexArrayAPPLE(0);
}

drawableObjectType_t Track::type() {
    return kDrawableObjectTypeSample;
}

#warning How to detect interaction between the track and the motorcycle