//
//  ImageLoader.h
//  GroupProjectCSE20212
//
//  Created by Shuyang Li on 04/21/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#ifndef __GroupProjectCSE20212__ImageLoader__
#define __GroupProjectCSE20212__ImageLoader__

#include <iostream>
#include <CoreGraphics/CoreGraphics.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

// caveat: CGImage has the y axis inverted!
class ImageLoader {
    
public:
    static GLuint loadImageAsTexture(char * filename);
    
};

#endif /* defined(__GroupProjectCSE20212__ImageLoader__) */
