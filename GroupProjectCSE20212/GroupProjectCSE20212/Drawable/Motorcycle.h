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

class Motorcycle : public Drawable {
    
public:
    Motorcycle(GLint        givenVertexBufferLoc,
               GLint        givenNormalBufferLoc,
               glm::vec3    objPosition,
               glm::vec3    objDirection,
               double       objAcceleration,
               double       objRotation);
    ~Motorcycle();
    
    virtual drawableObjectType_t type();
    
    virtual void draw(); // Draws the motorcycle
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
    
    // drawing
    GLuint vertexArrayObjectHandle[4];
    unsigned int vertexCount[4];
    
    GLuint vertexBuffer[4];
    GLuint normalBuffer[4];
    GLuint indexBuffer[4];
    
    
    glm::vec3 position;
    
    glm::vec3 direction;
    
    float speed; // (Unit: unit length per centisecond)
    
    float acceleration; // Acceleration for speed increase and/or decrease (Unit: unit length per centisecond squared)
    
    float rotation; // Change in direction angle when turning left or right(Unit: rad per centisecond)
    
    glm::mat4 scaleMatrix;
};

#endif /* defined(__GroupProjectCSE20212__Motorcycle__) */
