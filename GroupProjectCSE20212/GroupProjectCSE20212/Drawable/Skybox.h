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
#include "Constants.h"

class Skybox : public Drawable {

public:
    Skybox(GLint    givenVertexBufferLoc,
           GLint    givenNormalBufferLoc,
           GLint    givenUVBufferLoc);
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
    
    
    GLuint getTextureHandle();
    
private:
    GLuint vertexArrayObjectHandle;
    unsigned int vertexCount;
    
    GLuint vertexBuffer;
    GLuint indexBuffer;
    GLuint uvBuffer;
    
    glm::mat4 scaleMatrix;
};

#endif /* defined(__GroupProjectCSE20212__Skybox__) */
