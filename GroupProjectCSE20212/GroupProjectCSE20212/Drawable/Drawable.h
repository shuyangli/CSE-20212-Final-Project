//
//  Drawable.h
//  Learn-OpenGL
//
//  Created by Shuyang Li on 03/12/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#ifndef __Learn_OpenGL__Drawable__
#define __Learn_OpenGL__Drawable__

#include <iostream>
#include <OpenGL/gl.h>

class Drawable {
    
    /*
     * Abstract class as protocol for all drawable on-screen objects
     *
     * Note: nothing is implemented; all methods must be implemented in
     * all subclasses of Drawable
     *
     */
    
public:
    
    Drawable();
    virtual ~Drawable();
    
    /*
     * void draw()
     *
     * Draws the object on screen
     *
     */
    virtual void draw() = 0;
    
    /*
     * public variables for program and shader used to
     *
     */
    
    GLuint program;
    
    
};

// should #include all drawable classes here to minimize main.cpp overhead

#endif /* defined(__Learn_OpenGL__Drawable__) */
