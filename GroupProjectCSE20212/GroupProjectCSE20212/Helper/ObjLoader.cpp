//
//  ObjLoader.cpp
//  Learn-OpenGL
//
//  Created by Shuyang Li on 03/12/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include "ObjLoader.h"
#include <cstdio>   // fopen, fclose, fscanf

ObjLoader::ObjLoader() { }
ObjLoader::~ObjLoader() { }

void ObjLoader::loadObj(std::string filePath) {
    
    std::string errorStr = tinyobj::loadObj(shapes, filePath.c_str());
    if (!errorStr.empty()) throw std::runtime_error(errorStr);
}

void ObjLoader::loadObj(std::string filePath, std::string mtlBasePath) {
    
    std::string errorStr = tinyobj::loadObj(shapes, filePath.c_str(), mtlBasePath.c_str());
    if (!errorStr.empty()) throw std::runtime_error(errorStr);
}

std::vector<GLfloat>& ObjLoader::getVertices(size_t index) {
    return shapes[index].mesh.positions;
}

std::vector<GLfloat>& ObjLoader::getUVs(size_t index) {
    return shapes[index].mesh.texcoords;
}

std::vector<GLfloat>& ObjLoader::getNormals(size_t index) {
    return shapes[index].mesh.normals;
}

std::vector<unsigned int>& ObjLoader::getIndices(size_t index) {
    return shapes[index].mesh.indices;
}

tinyobj::material_t& ObjLoader::getMaterial(size_t index) {
    return shapes[index].material;
}