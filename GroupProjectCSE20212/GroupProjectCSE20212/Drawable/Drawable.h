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

#warning Part of an attempt to add introspection to Drawable objects; this enum list must be updated with all new subclasses of Drawable
typedef enum _drawableObjectType_t {
    kDrawableObjectTypeSkybox = 0,
    kDrawableObjectTypeMotorcycle,
    // and add other kDrawableObjectType... here
} drawableObjectType_t;

class Drawable {
    
public:
    
    /*
     * Abstract class as protocol for all drawable on-screen objects
     *
     * All methods here are public and serve as an outline to the
     * functionalities of all Drawable objects in the game; 
     * implementation detail is deferred to its subclasses
     *
     * Note: All methods MUST BE IMPLEMENTED in all subclasses
     *
     */
    
    Drawable() { };
    virtual ~Drawable() { };
    
    /*
     * void draw()
     *
     * Draws the object on screen with its correct
     *
     */
    virtual void draw() = 0;
    
    /*
     * drawableObjectType_t type()
     *
     * Performs reflection on the Drawable object; returns the corresponding
     * enum defined at the beginning of this header file
     *
     */
    virtual drawableObjectType_t type() = 0;
    
};

#endif /* defined(__Learn_OpenGL__Drawable__) */
