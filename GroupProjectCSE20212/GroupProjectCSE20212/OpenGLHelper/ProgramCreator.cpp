//
//  ProgramCreator.cpp
//  Learn-OpenGL
//
//  Created by Shuyang Li on 03/06/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include "ProgramCreator.h"

#include <fstream>
#include <OpenGL/gl.h>

/*
 * default constructor and destructor
 */
ProgramCreator::ProgramCreator() {
    program = -1;
}

ProgramCreator::~ProgramCreator() {
    
    if (program == -1) {
        // if the program is not yet linked, delete all shaders
        while (!shaderHandleList.empty()) {
            glDeleteShader(shaderHandleList.back());
            shaderHandleList.pop_back();
        }
    }
}

/*
 * void loadShader(GLenum shaderType, const std::string &shaderFilePath) THROWS std::invalid_argument
 *
 * shaderType: used directly in glCreateShader
 * shaderFileName: file path
 */
void ProgramCreator::loadShader(GLenum shaderType, const std::string &shaderFilePath) {
    
    GLuint shader = glCreateShader(shaderType);
    
    // read shader from file
    std::ifstream loader(shaderFilePath, std::ios::in);
    const char * shaderContent;   // placeholder
    
    if (loader.is_open()) {
        // stream is correctly opened, read file
        
        // read file into string
        std::string contentString((std::istreambuf_iterator<char>(loader)), std::istreambuf_iterator<char>());
        shaderContent = contentString.c_str();
        
        // compile shader
        glShaderSource(shader, 1, &shaderContent, NULL);
        glCompileShader(shader);
        
        // check compile status and report error
        GLint compileStatus;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
        
        if (compileStatus == GL_FALSE) {
            GLint infoLogLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            
            GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
            
            std::cerr << "Compile failure in " << shaderFilePath << ":" << std::endl << strInfoLog << std::endl;
            
            delete [] strInfoLog;
            throw std::invalid_argument("Compile failure");
        }
        
    } else {
        std::cerr << "Error opening file: " << shaderFilePath << std::endl;
        throw std::invalid_argument("Error opening file");
    }
    
    // add shader to list
	shaderHandleList.push_back(shader);

}

/*
 * void loadShader(GLenum shaderType, const std::vector<std::string> &shaderFileList) THROWS std::invalid_argument
 */
void ProgramCreator::loadShader(GLenum shaderType, const std::vector<std::string> &shaderFileList) {
    
    for (size_t i = 0; i < shaderFileList.size(); ++i) {
        loadShader(shaderType, shaderFileList[i]);
    }
}

/*
 * GLuint linkProgram() THROWS std::runtime_error
 */
GLuint ProgramCreator::linkProgram() {
    
    program = glCreateProgram();
    
    // attach shaders and link program
    for(size_t i = 0; i < shaderHandleList.size(); ++i)
    	glAttachShader(program, shaderHandleList[i]);
    
    glLinkProgram(program);
    
    // check status and report error
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        
        GLchar * strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        std::cerr << "Linker failure:" << std::endl << strInfoLog << std::endl;
        delete [] strInfoLog;
        
        throw std::runtime_error("Linker failure");
    }
    
    // detach and delete shaders
    for(size_t i = 0; i < shaderHandleList.size(); ++i) {
        glDetachShader(program, shaderHandleList[i]);
        glDeleteShader(shaderHandleList[i]);
    }
    
    return program;
}

/*
 * GLuint getProgramHandle() THROWS std::runtime_error
 */
GLuint ProgramCreator::getProgramHandle() {
    if (program == -1) throw std::runtime_error("Program not linked");
    return program;
}
