//
//  main.cpp
//  GroupProjectCSE20212
//
//  Created by Shuyang Li on 03/05/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

// OpenGL header
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

// SDL header
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// Helpers
#include "Constants.h"
#include "Helper/ProgramCreator.h"
#include "Helper/ObjLoader.h"

// GLM headers
#define GLM_FORCE_RADIANS
#include "Helper/glm/glm.hpp"
#include "Helper/glm/gtc/matrix_transform.hpp"      // GLM extensions
#include "Helper/glm/gtc/constants.hpp"             // GLM constants
#include "Helper/glm/gtc/type_ptr.hpp"              // glm::value_ptr()

// Drawable game objects
#include "DrawableList.h"


#pragma mark - Declarations

/**
 * @typedef myMenuSelection_t
 * enum for selections in game menu
 */
typedef enum _myMenuSelection_t {
    kMyMenuSelectionDefault = -1,
    kMyMenuSelectionQuit = 0,
    kMyMenuSelectionMainGame,
    kMyMenuSelectionSetting
} myMenuSelection_t;

/**
 * @typedef myGameStatus_t
 * enum for current game status
 */
typedef enum _myGameStatus_t {
    kMyGameStatusEnd,
    kMyGameStatusOngoing
} myGameStatus_t;


void initSDL();
void initOpenGL();
void quit(int exitCode);

void deleteObjects();
void deleteBuffers();
void deletePrograms();

void processEvents(myGameStatus_t &status);
void keyDownFunc(SDL_Keysym *keysym);

myMenuSelection_t displayMainMenu();
void displaySetting();
void newGame();
void redrawGameScreen();
void calculateObjects();


#pragma mark - Global variables

SDL_Window * globalWindow = NULL;
SDL_GLContext globalGLContext;

GLuint globalProgram = 0;

std::vector<Drawable *> globalDrawableObjects;
std::vector<GLuint> globalBuffers;

Sample * sampleObj; // controllable object

#pragma mark - Main

int main(int argc, const char * argv[]) {

    initSDL();
    initOpenGL();
/*    myMenuSelection_t sel = kMyMenuSelectionDefault;
    
    while (sel != kMyMenuSelectionQuit) {
        
        sel = displayMainMenu();
        
        switch (sel) {
            case kMyMenuSelectionMainGame:
                newGame();
                break;
                
            case kMyMenuSelectionSetting:
                displaySetting();
                break;
                
            default:
                break;
        }
    }
 */
    
    while (true) {                  // temporary
        myGameStatus_t temp;        // temporary
        processEvents(temp);        // temporary
        calculateObjects();         // temporary
        redrawGameScreen();         // temporary
    }                               // temporary
    
    quit(0);                        // temporary
    
    // never reached
    return 0;
}


#pragma mark - Setup and event processing functions

void initSDL() {
    
    // init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL failed to initialize: " << SDL_GetError() << std::endl;
        quit(1);
    }
    
    // create window with OpenGL context
    globalWindow = SDL_CreateWindow(WINDOW_TITLE,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  STARTING_WINDOW_WIDTH,
                                  STARTING_WINDOW_HEIGHT,
                                  SDL_WINDOW_OPENGL);
    globalGLContext = SDL_GL_CreateContext(globalWindow);
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);       // writing to depth buffer
    glDepthFunc(GL_LESS);       // incoming fragment < depth buffer
    glDepthRange(0.0f, 1.0f);   // all accepted depth range
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // error checking
    if (globalWindow == 0) {
        std::cerr << "Video mode set failed: " << SDL_GetError() << std::endl;
        quit(1);
    }
}

void initOpenGL() {
    
    // create program
    ProgramCreator myProgramCreator;
    myProgramCreator.loadShader(GL_VERTEX_SHADER, VERTEX_SHADER_PATH);
    myProgramCreator.loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_PATH);
    
    globalProgram = myProgramCreator.linkProgram();
    
    // bind buffer location
    GLint vertexBufferLoc = glGetAttribLocation(globalProgram, ATTRIB_NAME_INPUT_VERTEX);
    if (vertexBufferLoc == -1) quit(9);
    GLint normalBufferLoc = glGetAttribLocation(globalProgram, ATTRIB_NAME_INPUT_NORMAL);
    if (normalBufferLoc == -1) quit(9);
    
    // setup all buffer objects
    // we don't wrap loader into drawable classes because we need to keep track of allocated buffers on gpu memory, and free them when they're out of scope
    ObjLoader loader;
    loader.loadObj(CUBE_PATH, MTL_BASEPATH);
    
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 loader.getVertices().size() * sizeof(GLfloat),
                 loader.getVertices().data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    globalBuffers.push_back(vertexBuffer);
    
    GLuint normalBuffer;
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 loader.getNormals().size() * sizeof(GLfloat),
                 loader.getNormals().data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    globalBuffers.push_back(normalBuffer);
    
    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 loader.getIndices().size() * sizeof(GLuint),
                 loader.getIndices().data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    globalBuffers.push_back(indexBuffer);
    
    
    // create drawable objects
    Sample * mySampleObject = new Sample(vertexBuffer,
                                         vertexBufferLoc,
                                         (unsigned int) loader.getIndices().size(),
                                         normalBuffer,
                                         normalBufferLoc,
                                         indexBuffer);
    sampleObj = mySampleObject;
    globalDrawableObjects.push_back(mySampleObject);
}

void deleteObjects() {
    std::for_each(globalDrawableObjects.begin(), globalDrawableObjects.end(), [](Drawable * obj) {
        delete obj;
    });
    globalDrawableObjects.clear();
}

void deleteBuffers() {
    std::cout << "deleting buffers" << std::endl;
    std::for_each(globalBuffers.begin(), globalBuffers.end(), [](GLuint buffer) {
        glDeleteBuffers(1, &buffer);
    });
}

void deletePrograms() {
    glDeleteProgram(globalProgram);
}

void quit(int exitCode) {
    
    deleteObjects();
    deletePrograms();
    deleteBuffers();
    SDL_GL_DeleteContext(globalGLContext);
    SDL_DestroyWindow(globalWindow);
    SDL_Quit();
    exit(exitCode);
}

void processEvents(myGameStatus_t &status) {
    
    // Grab all the events off the event queue
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_KEYDOWN:
                keyDownFunc(&event.key.keysym);
                break;
                
            case SDL_QUIT:
                status = kMyGameStatusEnd;
                break;
                
            default:
                break;
        }
    }
}

void keyDownFunc(SDL_Keysym * keysym) {
    std::cout << "key down" << std::endl;
    
    switch (keysym -> sym) {
        case SDLK_ESCAPE:
#warning Ideally this should display an in-game menu instead of quitting directly
            quit(0);
            break;
            
        case SDLK_a:
            sampleObj -> decreaseTurn();
            break;
        case SDLK_d:
            sampleObj -> increaseTurn();
            break;
            
        default:
            break;
    }
}


#pragma mark - Game Functions

myMenuSelection_t displayMainMenu() {
    myMenuSelection_t tempSel = kMyMenuSelectionDefault;
    
#warning TODO
    
    return tempSel;
}

void displaySetting() {
#warning TODO
    
}

void newGame() {
#warning TODO
    
    myGameStatus_t gameStatus = kMyGameStatusOngoing;
    
    // main event loop
    while (gameStatus != kMyGameStatusEnd) {
        processEvents(gameStatus);
        calculateObjects();
        redrawGameScreen();
    }
    
}

void calculateObjects() {
    std::cout << "calculate" << std::endl;
    std::for_each(globalDrawableObjects.begin(), globalDrawableObjects.end(), [](Drawable * obj){
        obj -> calculateModelMatrix();
    });
}


void redrawGameScreen() {
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // get uniform location
    glUseProgram(globalProgram);
    GLint mvpMatLoc = glGetUniformLocation(globalProgram, UNIFORM_NAME_MVP_MATRIX);
    GLint normalModelViewMatLoc = glGetUniformLocation(globalProgram, UNIFORM_NAME_NORMAL_MV_MATRIX);
    GLint directionToLightLoc = glGetUniformLocation(globalProgram, UNIFORM_NAME_DIRECTION_TO_LIGHT);
    GLint lightIntensityLoc = glGetUniformLocation(globalProgram, UNIFORM_NAME_LIGHT_INTENSITY);
    GLint ambientLightIntensityLoc = glGetUniformLocation(globalProgram, UNIFORM_NAME_AMBIENT_INTENSITY);
    
    // proj matrix (clip space) only changes when fov or aspect ratio changes, so we don't modify it
    static const glm::mat4 projMat = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    
    // actual drawing (not very efficient, but works)
    std::for_each(globalDrawableObjects.begin(), globalDrawableObjects.end(), [&](Drawable * obj){
        
        glm::mat4 modelMat = obj -> getModelMatrix();
        glm::mat4 viewMat = glm::lookAt(glm::vec3(8, 5, 5),
                                        glm::vec3(0, 0, 0),
                                        glm::vec3(0, 1, 0));
        glm::mat4 mvpMat = projMat * viewMat * modelMat;
        glm::mat3 mvMat = glm::transpose(glm::inverse(glm::mat3(viewMat * modelMat)));
        
        // bind uniforms
        glUniformMatrix4fv(mvpMatLoc, 1, GL_FALSE, glm::value_ptr(mvpMat));
        glUniformMatrix3fv(normalModelViewMatLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
        
        glUniform3fv(directionToLightLoc, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, -1.0f)));
        glUniform4f(lightIntensityLoc, 0.8f, 0.8f, 0.8f, 1.0f);
        glUniform4f(ambientLightIntensityLoc, 0.2f, 0.2f, 0.2f, 1.0f);
        
        obj -> draw();
    });
    
    glUseProgram(0);
    
    SDL_GL_SwapWindow(globalWindow);
}
