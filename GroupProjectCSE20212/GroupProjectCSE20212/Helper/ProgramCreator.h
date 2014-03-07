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

class ProgramCreator {
    
public:
    /*
     * default constructor and destructor
     * 
     * Description:
     * create or destroy empty ProgramCreator object
     *
     */
    ProgramCreator();
    ~ProgramCreator();
    
    /*
     * void loadShader(GLenum shaderType, const std::string &shaderFilePath) THROWS std::invalid_argument
     *
     * Load one shader from file path
     *
     * Arguments:
     * shaderType: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER
     * shaderFileName: file path represented by std::string
     *
     * THROWS std::invalid_argument when shader is not correctly compiled
     *
     */
    void loadShader(GLenum shaderType, const std::string &shaderFilePath);
    
    /*
     * void loadShader(GLenum shaderType, const std::vector<std::string> &shaderFileList) THROWS std::invalid_argument
     *
     * Load multiple shaders from file path
     *
     * Arguments:
     * shaderType: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER
     * shaderFileList: file paths represented by std::string contained in a std::vector
     *
     * THROWS std::invalid_argument when loadShader(string) throws
     *
     */
    void loadShader(GLenum shaderType, const std::vector<std::string> &shaderFileList);
    
    /*
     * GLuint compileProgram() THROWS std::runtime_error
     *
     * Compile program with given shaders
     * Caution: all shaders are deleted when the program is compiled!
     * Therefore it's not possible to reuse a ProgramCreator
     *
     * Return value:
     * Program handle returned by glCreateProgram()
     *
     * THROWS std::runtime_error when program is not correctly linked
     *
     */
    GLuint linkProgram();
    
    /*
     * GLuint getProgramHandle() THROWS std::runtime_error
     *
     * Return compiled program handle
     *
     * Return value:
     * Program handle returned by glCreateProgram()
     *
     * THROWS std::runtime_error when program is not yet linked or is already destroyed
     *
     */
    GLuint getProgramHandle();
    
private:
    // program handle
    GLuint program;
    
    // shader handles prior to compilation
    std::vector<GLuint> shaderHandleList;
    
};

#endif /* defined(__Learn_OpenGL__ProgramCreator__) */
