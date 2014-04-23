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
    
    // Draws the motorcycle
    virtual void draw();
    
    // Special drawing for motorcycle for different parts
    void draw(GLint lightIntensityLoc, GLint ambientLightIntensityLoc);
    
    // calculate rotation
    virtual void calculateModelMatrix();
    
    glm::vec3 getCameraLocation();
    glm::vec3 getCameraFocus();
    
    // Moves the motorcycle
    void move(unsigned int deltaTime);
    
    /* Controls for the motorcycle */
    void turnLeft(unsigned int deltaTime);
    void turnRight(unsigned int deltaTime);
    void incSpeed(unsigned int deltaTime);
    void decSpeed(unsigned int deltaTime);
    
    int isInBounds();           // Determines if the motorcycle is in on the road
    int isInCollision();        // Determines if the motorcycle collides with something
    int isFinished();           // Determines if the motorcycle gets to the finish line
    
private:
    
    // drawing
    GLuint vertexArrayObjectHandle[4];
    unsigned int vertexCount[4];
    
    GLuint vertexBuffer[4];
    GLuint normalBuffer[4];
    GLuint indexBuffer[4];
    
    
    // position of motorcycle; coordinates of motorcycle's origin
    glm::vec3 position;
    
    // "front" direction of motorcycle
    glm::vec3 direction;
    glm::vec3 initialDirection;     // used to calculate rotation angle
    
    // speed of motorcycle;
    // Unit: unit length per millisecond
    float speed;
    
    // Acceleration for speed increase and/or decrease
    // Acceleration is a CONSTANT
    // Unit: unit length per millisecond^2
    float acceleration;
    
    // Change in direction angle when turning left or right
    // Also a CONSTANT
    // Unit: rad per millisecond
    float rotation;
    
    // angle of motorcycle's head direction
    float angleToFront;
    
    glm::mat4 scaleMatrix;
};

#endif /* defined(__GroupProjectCSE20212__Motorcycle__) */
