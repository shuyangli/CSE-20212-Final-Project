//
//  Skybox.h
//  GroupProjectCSE20212
//
//  Created by Shuyang Li on 03/31/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#ifndef __GroupProjectCSE20212__Skybox__
#define __GroupProjectCSE20212__Skybox__

#include <iostream>
#include "Drawable.h"

class Skybox : public Drawable {

public:
    Skybox(GLuint       givenVertexBuffer,
           GLint        givenVertexBufferLoc,
           unsigned int givenVertexCount,
           GLuint       givenNormalBuffer,
           GLint        givenNormalBufferLoc,
           GLuint       givenIndexBuffer,
           GLuint       givenTextureHandle);
    virtual ~Skybox();
    
    /**
     * Draws the object on screen
     */
    virtual void draw();
    
    /**
     * Performs introspection on the Drawable object
     * @return  corresponding enum
     */
    virtual drawableObjectType_t type();
    
private:
    GLuint vertexArrayObjectHandle;
    unsigned int vertexCount;
    glm::mat4 scaleMatrix;
};

#endif /* defined(__GroupProjectCSE20212__Skybox__) */
