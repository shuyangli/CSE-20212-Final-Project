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
    void setTime(double);
    double getTime() const;
    */
     
private:
    
    /* Position on the road */
    /*
    double position_x;
    double position_y;
    double position_z;
     */
    
    
    
    /* Record the time (unit: millisecond) from the start to the finish line */
};


#endif /* defined(__GroupProjectCSE20212__Obstacle__) */
