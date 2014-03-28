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

/**
 * @class ObjLoader
 * Object-oriented wrapper around tiny_obj_loader
 */
class ObjLoader {
    
public:
    ObjLoader();
    ~ObjLoader();
    
    /**
     * Load one .obj file from file path; throws std::runtime_error when .obj file or .mtl path cannot be resolved, or file is corrupted
     * @warning If the .obj file references a .mtl file, @c loadObj(filePath, mtlBasePath) should be used
     *
     * @throws std::runtime_error
     *
     * @param filePath
     *      .obj file path
     */
    void loadObj(std::string filePath);
    
    /**
     * Load one .obj file from file path; throws std::runtime_error when .obj file or .mtl path cannot be resolved, or file is corrupted
     *
     * @throws std::runtime_error
     *
     * @param filePath
     *      .obj file path
     *
     * @param mtlBasePath
     *      path to directory containing .mtl files
     */
    void loadObj(std::string filePath, std::string mtlBasePath);
    
    /**
     * Return a vector of vertices
     *
     * @return vector of vertices
     */
    std::vector<GLfloat>& getVertices(size_t index = 0);
    
    /**
     * Return a vector of UVs
     *
     * @return vector of UVs
     */
    std::vector<GLfloat>& getUVs(size_t index = 0);
    
    /**
     * Return a vector of normals
     *
     * @return vector of normals
     */
    std::vector<GLfloat>& getNormals(size_t index = 0);
    
    /**
     * Return a vector of indices
     *
     * @return vector of indices
     */
    std::vector<unsigned int>& getIndices(size_t index = 0);
    
private:
    std::vector<tinyobj::shape_t> shapes;
    
};

#endif /* defined(__Learn_OpenGL__ObjLoader__) */
