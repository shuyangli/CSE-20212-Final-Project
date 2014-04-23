//
//  Ground.h
//  GroupProjectCSE20212
//
//  Created by Shuyang Li on 04/23/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#ifndef __GroupProjectCSE20212__Ground__
#define __GroupProjectCSE20212__Ground__

#include <iostream>

class Ground : public Drawable {
    
public:
    Ground(GLint        givenVertexBufferLoc,
          GLint        givenNormalBufferLoc);
    ~Ground();
    
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

#endif /* defined(__GroupProjectCSE20212__Ground__) */
