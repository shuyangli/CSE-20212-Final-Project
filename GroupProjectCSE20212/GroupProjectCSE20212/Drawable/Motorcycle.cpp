//
//  Motorcycle.cpp
//  GroupProjectCSE20212
//
//  Created by Yuxuan Chen on 3/28/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include "Motorcycle.h"
#include <iostream>
#include "Drawable.h"
#include "Obstacle.h"
#include "Skybox.h"
#include "Track.h"

Motorcycle::Motorcycle(GLuint       givenVertexBuffer,
                       GLint        givenVertexBufferLoc,
                       unsigned int givenVertexCount,
                       GLuint       givenNormalBuffer,
                       GLint        givenNormalBufferLoc,
                       GLuint       givenIndexBuffer)
{
    
}

Motorcycle::~Motorcycle()
{
    
}
    
void Motorcycle::draw() // Draws the motorcycle
{
    
}

void Motorcycle::move() // Moves the motorcycle
{
    pos += speed;
}

int Motorcycle::isInBounds()           // Determines if the motorcycle is in on the road
{
    return 1;
}

int Motorcycle::isInCollision()        // Determines if the motorcycle collides with something
{
    return 0;
}

int Motorcycle::isFinished()           // Determines if the motorcycle gets to the finish line
{
    return 0;
}

/*
    // Getters and Setters
void Motorcycle::setSpeedX(double)
{
    
}

double Motorcycle::getSpeedX() const
{
    return 0;
}

void Motorcycle::setSpeedY(double)
{
    
}

double Motorcycle::getSpeedY() const
{
    return 0;
}

void Motorcycle::setSpeedZ(double)
{
    
}

double Motorcycle::getSpeedZ() const
{
    return 0;
}

void Motorcycle::setPositionX(double)
{
    
}

double Motorcycle::getPositionX() const
{
    return 0;
}

void Motorcycle::setPositionY(double)
{
    
}

double Motorcycle::getPositionY() const
{
    return 0;
}

void Motorcycle::setPositionZ(double)
{
    
}

double Motorcycle::getPositionZ() const
{
    return 0;
}
*/
 
void Motorcycle::setTime(double)
{
    
}

double Motorcycle::getTime() const
{
    return 0;
}
 
