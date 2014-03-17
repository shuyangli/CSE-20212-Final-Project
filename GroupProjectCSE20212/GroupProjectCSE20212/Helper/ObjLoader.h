//
//  ObjLoader.h
//  Learn-OpenGL
//
//  Created by Shuyang Li on 03/12/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#ifndef __Learn_OpenGL__ObjLoader__
#define __Learn_OpenGL__ObjLoader__

#include <iostream>
#include <vector>
#include <exception>

#include <OpenGL/gl.h>
#include "glm/glm.hpp"

#include "tiny_obj_loader.h"


// OO wrapper around tiny_obj_loader loadObject function
class ObjLoader {
    
public:
    ObjLoader();
    ~ObjLoader();
    
    /*
     * void loadObj(std::string& filePath) THROWS std::runtime_error
     * void loadObj(std::string& filePath, std::string& mtlBasePath) THROWS std::runtime_error
     *
     * Description:
     * Load one .obj file from file path
     *
     * Arguments:
     * filePath: .obj file path
     * mtlBasePath: path to directory containing .mtl files
     *
     * THROWS std::runtime_error when .obj file or .mtl path cannot be resolved, or file is corrupted
     *
     */
    void loadObj(std::string filePath);
    void loadObj(std::string filePath, std::string mtlBasePath);
    
    /*
     * getter methods
     *
     * Description:
     * Return reference to different attributes in the shape
     *
     */
    std::vector<GLfloat>& getVertices(size_t index = 0);
    std::vector<GLfloat>& getUVs(size_t index = 0);
    std::vector<GLfloat>& getNormals(size_t index = 0);
    std::vector<unsigned int>& getIndices(size_t index = 0);
    
private:
    std::vector<tinyobj::shape_t> shapes;
    
};

#endif /* defined(__Learn_OpenGL__ObjLoader__) */
