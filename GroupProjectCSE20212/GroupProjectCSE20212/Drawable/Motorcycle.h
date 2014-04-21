//
//  Motorcycle.h
//  GroupProjectCSE20212
//
//  Created by Yuxuan Chen on 3/28/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#ifndef __GroupProjectCSE20212__Motorcycle__
#define __GroupProjectCSE20212__Motorcycle__

#include <iostream>
#include "Drawable.h"

class Motorcycle : Drawable {
    
public:
    
    Motorcycle(GLuint       givenVertexBuffer,
               GLint        givenVertexBufferLoc,
               unsigned int givenVertexCount,
               GLuint       givenNormalBuffer,
               GLint        givenNormalBufferLoc,
               GLuint       givenIndexBuffer,
               glm::vec3, glm::vec3, double, double);
    ~Motorcycle();
    
    void draw(); // Draws the motorcycle
    void move(); // Moves the motorcycle
    
    /* Controls for the motorcycle */
    void turnLeft();
    void turnRight();
    void incSpeed();
    void decSpeed();
    
    int isInBounds();           // Determines if the motorcycle is in on the road
    int isInCollision();        // Determines if the motorcycle collides with something
    int isFinished();           // Determines if the motorcycle gets to the finish line
    
    
    /* Getters and Setters */
    void setTime(); // Set time to current system timestamp
    float getTime() const; // Get elapsed time from last timestamp
    void setPosition(glm::vec3);
    glm::vec3 getPosition() const;
    void setDirection(glm::vec3);
    glm::vec3 getDirection() const;
    void setSpeed(double);
    float getSpeed() const;
    void setAcceleration(double);
    float getAcceleration() const;
    void setRotation(double);
    float getRotation() const;
    
    
private:

    GLuint vertexArrayObjectHandle;
    
    unsigned int vertexCount;
    
    glm::vec3 position;
    
    glm::vec3 direction;
    
    float speed; // (Unit: unit length per centisecond)
    
    float acceleration; // Acceleration for speed increase and/or decrease (Unit: unit length per centisecond squared)
    
    float rotation; // Change in direction angle when turning left or right(Unit: rad per centisecond)
    
    glm::mat4 scaleMatrix;
    
    clock_t time; // Timer (unit: millisecond)
};

#endif /* defined(__GroupProjectCSE20212__Motorcycle__) */
