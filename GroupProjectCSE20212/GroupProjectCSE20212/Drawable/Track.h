//
//  Track.h
//  GroupProjectCSE20212
//
//  Created by Yuxuan Chen on 3/28/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#ifndef __GroupProjectCSE20212__Track__
#define __GroupProjectCSE20212__Track__

#include <iostream>
#include "Drawable.h"

class Track : Drawable {
    
public:
    Track(GLuint       givenVertexBuffer,
           GLint        givenVertexBufferLoc,
           unsigned int givenVertexCount,
           GLuint       givenNormalBuffer,
           GLint        givenNormalBufferLoc,
           GLuint       givenIndexBuffer);
    ~Track();
    
    /**
     * Draws the object on screen
     */
    void draw();
    
    /**
     * Performs introspection on the Drawable object
     * @return corresponding enum
     */
    drawableObjectType_t type();
    
private:
    GLuint vertexArrayObjectHandle;
    unsigned int vertexCount;
    
    glm::mat4 scaleMatrix;
    
};

#endif /* defined(__GroupProjectCSE20212__Track__) */
