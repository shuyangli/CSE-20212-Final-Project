//
//  Sample.h
//  GroupProjectCSE20212
//
//  Created by Shuyang Li on 03/29/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#ifndef __GroupProjectCSE20212__Sample__
#define __GroupProjectCSE20212__Sample__

#include <iostream>
#include "Drawable.h"

/**
 * @class Sample
 * A sample class as a concrete subclass of Drawable; draws a motorcycle wheel
 */
class Sample : public Drawable {
    
public:
    
    Sample(GLuint program);
    ~Sample();
    
    /**
     * Draws the object on screen
     */
    void draw();
    
    /**
     * Performs introspection on the Drawable object
     * @return corresponding enum
     */
    drawableObjectType_t type();
};

#endif /* defined(__GroupProjectCSE20212__Sample__) */
