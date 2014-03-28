//
//  ProgramCreator.h
//  Learn-OpenGL
//
//  Created by Shuyang Li on 03/06/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#ifndef __Learn_OpenGL__ProgramCreator__
#define __Learn_OpenGL__ProgramCreator__

#include <iostream>
#include <vector>
#include <exception>

#include <OpenGL/gl.h>

/**
 * @class ProgramCreator
 * Helper class to load and compile shaders, and to link program
 */
class ProgramCreator {
    
public:
    /**
     * Create empty ProgramCreator object
     */
    ProgramCreator();
    
    /**
     * Destroy ProgramCreator object
     */
    ~ProgramCreator();
    
    /**
     * Load one shader from file path; throws std::invalid_argument when shader is not correctly compiled
     * 
     * @throws std::invalid_argument
     *
     * @param shaderType
     *      valid values: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER
     *
     * @param shaderFileName
     *      file path
     */
    void loadShader(GLenum shaderType, const std::string &shaderFilePath);
    
    /**
     * Load multiple shaders from file path list; throws std::invalid_argument when any shader is not correctly compiled
     *
     * @throws std::invalid_argument
     *
     * @param shaderType
     *      valid values: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER
     *
     * @param shaderFileName
     *      a std::vector of file paths; must all be the same type of shader
     */
    void loadShader(GLenum shaderType, const std::vector<std::string> &shaderFileList);
    
    /**
     * Compile program with given shaders; throws std::runtime_error when program is not correctly linked
     * @warning all shaders are deleted when the program is compiled, therefore it's not possible to reuse a ProgramCreator
     *
     * @throws std::runtime_error
     *
     * @return program handle returned by glCreateProgram()
     */
    GLuint linkProgram();
    
    /**
     * Return compiled program handle; throws std::runtime_error when program is not yet linked or is already destroyed
     * @warning all shaders are deleted when the program is compiled, therefore it's not possible to reuse a ProgramCreator
     *
     * @throws std::runtime_error
     *
     * @return program handle returned by glCreateProgram()
     */
    GLuint getProgramHandle();
    
private:
    // program handle
    GLuint program;
    
    // shader handles prior to compilation
    std::vector<GLuint> shaderHandleList;
    
};

#endif /* defined(__Learn_OpenGL__ProgramCreator__) */
