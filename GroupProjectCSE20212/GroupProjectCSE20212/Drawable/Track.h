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

class Track : public Drawable {
    
public:
    Track(GLint        givenVertexBufferLoc,
          GLint        givenNormalBufferLoc);
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
    
    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint indexBuffer;
    
    glm::mat4 scaleMatrix;
};

#endif /* defined(__GroupProjectCSE20212__Track__) */
