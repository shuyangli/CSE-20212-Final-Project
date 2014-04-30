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
#include "Motorcycle.h"

class Track : public Drawable {
    
public:
    Track(GLint        givenVertexBufferLoc,
          GLint        givenNormalBufferLoc);
    ~Track();
    
    void draw();
    
    // Getter of the walls variable
    std::vector<glm::vec2> getWalls() const;
    
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
    // This variable stores the bottom vertices of the walls
    std::vector<glm::vec2> walls;
    
    
};

#endif /* defined(__GroupProjectCSE20212__Track__) */
