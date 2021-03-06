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
#include "ObjLoader.h"
#define GLM_FORCE_RADIANS
#include "../Helper/glm/glm.hpp"
#include "../Helper/glm/gtc/matrix_transform.hpp"

typedef enum _drawableObjectType_t {
    kDrawableObjectTypeSample = -1,
    kDrawableObjectTypeSkybox = 0,
    kDrawableObjectTypeMotorcycle,
    kDrawableObjectTypeTrack
    // and add other kDrawableObjectType... here
} drawableObjectType_t;


/**
 * @class Drawable
 * Abstract class as protocol for all drawable on-screen objects
 */

class Drawable {
    
public:
    
    Drawable() { }
    virtual ~Drawable() { setLoader(nullptr); }
    
    /**
     * Draws the object on screen
     */
    virtual void draw() = 0;
    
    /**
     * Performs introspection on the Drawable object
     * @return  corresponding enum
     */
    virtual drawableObjectType_t type() = 0;
    
    /**
     * Returns a model matrix used in translation to draw this object
     * @return model matrix
     */
    glm::mat4 getModelMatrix() { return modelMatrix; }
    
    /**
     * Modify model matrix used in translation to draw this object
     * @param newModelMatrix    new model matrix
     */
    void setModelMatrix(glm::mat4 newModelMatrix) { modelMatrix = newModelMatrix; }
    
    /**
     * Returns the scale vector corresponding to size relative to model size
     * @return scale vector
     */
    glm::vec3 getScaleVector() { return modelScale; }
    
    /**
     * Modify scale vector
     * @param newScaleVector    new scale vector
     */
    void setScaleVector(glm::vec3 newScaleVector) { modelScale = newScaleVector; }
    
    /**
     * Returns pointer to object loader
     * @return loader
     */
    ObjLoader * getLoader() { return loader; }
    
    /**
     * Set pointer to object loader
     * @param newLoader     new loader
     */
    void setLoader(ObjLoader * newLoader) {
        if (loader == nullptr) delete loader;
        loader = newLoader;
    }
    
    /**
     * Calculate new model matrix, called once each frame
     */
    virtual void calculateModelMatrix() { }
    
    /**
     * Perform introspection
     * @param   type    enum of drawable object type
     * @return  string of drawable object type
     */
    static const char * getTypeString(drawableObjectType_t type) {
        switch (type) {
            case kDrawableObjectTypeSample:
                return "Sample";
                break;
                
            case kDrawableObjectTypeMotorcycle:
                return "Motorcycle";
                break;
            
            case kDrawableObjectTypeSkybox:
                return "Skybox";
                break;
                
            case kDrawableObjectTypeTrack:
                return "Track";
                break;
                
            default:
                return "Error";
                break;
        }
    }
    
private:
    glm::mat4 modelMatrix;
    glm::vec3 modelScale = glm::vec3(1.0f);
    ObjLoader *loader = nullptr;
};

#endif /* defined(__Learn_OpenGL__Drawable__) */
