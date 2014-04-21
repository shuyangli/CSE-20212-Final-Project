//
//  Constants.h
//  GroupProjectCSE20212
//
//  Created by Shuyang Li on 03/07/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#ifndef GroupProjectCSE20212_Constants_h
#define GroupProjectCSE20212_Constants_h

/* Window Configuration */
#define STARTING_WINDOW_WIDTH       800
#define STARTING_WINDOW_HEIGHT      800
#define WINDOW_TITLE                "CSE 20212 Group Project"

/* GLSL Programs */
#pragma mark - GLSL Programs
#define VERTEX_SHADER_PATH          "./GLSL/vertex.glsl"
#define FRAGMENT_SHADER_PATH        "./GLSL/fragment.glsl"
#define TEXTURED_VERTEX_SHADER_PATH "./GLSL/vertexSkybox.glsl"
#define TEXTURED_FRAGMENT_SHADER_PATH "./GLSL/fragmentSkybox.glsl"

#define ATTRIB_NAME_INPUT_VERTEX    "inputVertexPosModelSpace"
#define ATTRIB_NAME_INPUT_NORMAL    "inputVertexNormalModelSpace"

#define UNIFORM_NAME_MVP_MATRIX     "mvpMatrix"
#define UNIFORM_NAME_NORMAL_MV_MATRIX   "normalModelViewMatrix"
#define UNIFORM_NAME_DIRECTION_TO_LIGHT "directionToLight"
#define UNIFORM_NAME_LIGHT_INTENSITY    "directionalLightIntensity"
#define UNIFORM_NAME_AMBIENT_INTENSITY  "ambientLightIntensity"

/* Objects */
#pragma mark - Objects
#define WHEEL_PATH                  "./Resource/Wheel.obj"
#define CUBE_PATH                   "./Resource/Cube.obj"
#define MTL_BASEPATH                "./Resource/"

#define SKYBOX_PATH                 "./Resource/Skybox.obj"
#define SKYBOX_TEXTURE_PATH         "./Resource/skybox_texture.jpg"

#endif
