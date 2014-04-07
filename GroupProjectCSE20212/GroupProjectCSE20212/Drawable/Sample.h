//
//  Sample.h
//  GroupProjectCSE20212
//
//  Created by Shuyang Li on 03/29/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#ifndef __GroupProjectCSE20212__Sample__
#define __GroupProjectCSE20212__Sample__

#include <iostream>
#include "Drawable.h"

/**
 * @class Sample
 * A sample class as a concrete subclass of Drawable; draws a motorcycle wheel
 */
class Sample : public Drawable {
    
public:
    
    /**
     * Constructor with already created buffer objects
     * @warning need to make sure provided vertex count matches
     * @param givenVertexBuffer     handle to vertex buffer object
     * @param givenVertexBufferLoc  attrib location for vertex buffer
     * @param givenVertexCount      count of vertices of object
     * @param givenIndexBuffer      handle to buffer object containing indices
     */
    Sample(GLuint       givenVertexBuffer,
           GLint        givenVertexBufferLoc,
           unsigned int givenVertexCount,
           GLuint       givenNormalBuffer,
           GLint        givenNormalBufferLoc,
           GLuint       givenIndexBuffer);
    ~Sample();
    
    /**
     * Draws the object on screen
     */
    void draw();
    
    /**
     * Increases the object's turn speed (to the right)
     */
    void increaseTurn();
    
    /**
     * Decreases the object's turn speed
     */
    void decreaseTurn();
    
    /**
     * Calculate model matrix
     */
    virtual void calculateModelMatrix();
    
    /**
     * Performs introspection on the Drawable object
     * @return corresponding enum
     */
    drawableObjectType_t type();
    
private:
    GLuint vertexArrayObjectHandle;
    unsigned int vertexCount;
    
    float rotateSpeed;
    float rotateAngle;
    glm::mat4 scaleMatrix;
};

#endif /* defined(__GroupProjectCSE20212__Sample__) */
