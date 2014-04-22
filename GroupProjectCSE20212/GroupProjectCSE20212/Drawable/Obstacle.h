//
//  Obstacle.h
//  GroupProjectCSE20212
//
//  Created by Yuxuan Chen on 3/28/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#ifndef __GroupProjectCSE20212__Obstacle__
#define __GroupProjectCSE20212__Obstacle__

#include <iostream>
#include "Drawable.h"

class Obstacle : Drawable {
    
public:
    
    Obstacle(GLuint       givenVertexBuffer,
               GLint        givenVertexBufferLoc,
               unsigned int givenVertexCount,
               GLuint       givenNormalBuffer,
               GLint        givenNormalBufferLoc,
               GLuint       givenIndexBuffer);
    ~Obstacle();
    
    void draw(); // Draws the obstacle
    int isInCollision();        // Determines if any motorcycle has crashed into the obstacle
    
    // Getters and setters
    
     
private:
    // drawing
    GLuint vertexArrayObjectHandle;
    unsigned int vertexCount;
    
    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint indexBuffer;
    
    glm::vec3 position;
    
    glm::vec3 direction;
    
    float speed; // (Unit: unit length per centisecond)
    
    float acceleration; // Acceleration for speed increase and/or decrease (Unit: unit length per centisecond squared)
    
    float rotation; // Change in direction angle when turning left or right(Unit: rad per centisecond)
    
    glm::mat4 scaleMatrix;
    
};


#endif /* defined(__GroupProjectCSE20212__Obstacle__) */
