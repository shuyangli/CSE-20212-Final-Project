//
//  Vehicle.h
//  GroupProjectCSE20212
//
//  Created by Yuxuan Chen on 3/28/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#ifndef __GroupProjectCSE20212__Vehicle__
#define __GroupProjectCSE20212__Vehicle__

#include <iostream>
#include "Drawable.h"

class Vehicle : public Drawable {
    
public:
    Vehicle();
    ~Vehicle();
    void draw();
};

#endif /* defined(__GroupProjectCSE20212__Vehicle__) */

