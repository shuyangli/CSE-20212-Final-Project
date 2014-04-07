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
    
    Motorcycle() { }
    ~Motorcycle() { }
    
    void draw(); // Draws the motorcycle
    void move(); // Moves the motorcycle
    int isInBounds();           // Determines if the motorcycle is in on the road
    int isInCollision();        // Determines if the motorcycle collides with something
    
    // Getters and Setters
    void setSpeed(double);
    double getSpeed() const;
    void setPosition(double);
    double getPosition() const;
    
    int hasCollision();
    
private:
    
    double speed;    // Speed of the motorcycle
    double position; // Position on the road, range from 0 to the width of the road
    
};

#endif /* defined(__GroupProjectCSE20212__Motorcycle__) */
