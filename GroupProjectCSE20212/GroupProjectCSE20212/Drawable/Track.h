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
#include <vector>
#include "Drawable.h"

class Track : public Drawable {
    
public:
    Track(GLint        givenVertexBufferLoc,
          GLint        givenNormalBufferLoc);
    ~Track();
    
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
    
    // The track has walls that flank the road.
    // The inner part of the walls are considered to consist of multiple neighboring rectangular faces.
    // This vector stores the coordinates of the bottom two vertices of these rectangular faces.
    std::vector<std::vector <glm::vec3> > innerWallFaces;
    
};

#endif /* defined(__GroupProjectCSE20212__Track__) */
