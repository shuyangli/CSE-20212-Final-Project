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
               GLuint       givenIndexBuffer);
    ~Motorcycle();
    
    void draw(); // Draws the motorcycle
    void move(); // Moves the motorcycle
    int isInBounds();           // Determines if the motorcycle is in on the road
    int isInCollision();        // Determines if the motorcycle collides with something
    int isFinished();           // Determines if the motorcycle gets to the finish line
    
    // Getters and Setters
    /*
    void setSpeedX(double);
    double getSpeedX() const;
    void setSpeedY(double);
    double getSpeedY() const;
    void setSpeedZ(double);
    double getSpeedZ() const;
    void setPositionX(double);
    double getPositionX() const;
    void setPositionY(double);
    double getPositionY() const;
    void setPositionZ(double);
    double getPositionZ() const;
     */
     
    void setTime(double);
    double getTime() const;
    
private:

    /* Speed in different directions */
    /*
    double speed_x;
    double speed_y;
    double speed_z;
     */
    
    /* Position on the road */
    /*
    double position_x;
    double position_y;
    double position_z;
     */
    
    /* Record the time (unit: millisecond) from the start to the finish line */
    glm::vec3 speed, pos;
    double time;
};

#endif /* defined(__GroupProjectCSE20212__Motorcycle__) */
