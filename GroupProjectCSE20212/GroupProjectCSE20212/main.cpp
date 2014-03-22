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

// Game objects
#include "Drawable/Drawable.h"


#pragma mark - Declarations

typedef enum _myMenuSelection_t {
    kMyMenuSelectionDefault = -1,
    kMyMenuSelectionQuit = 0,
    kMyMenuSelectionMainGame,
    kMyMenuSelectionSetting
} myMenuSelection_t;

typedef enum _myGameStatus_t {
    kMyGameStatusEnd,
    kMyGameStatusOngoing
} myGameStatus_t;


bool initSDL();
void setupOpenGL();
static void quit(int exitCode);


static void processEvents(myGameStatus_t &status);
static void keyDownFunc(SDL_Keysym *keysym);
//static void resizeFunc(SDL_Event *resizeEvent);   // currently disabled window resize


myMenuSelection_t displayMainMenu();
void displaySetting();
void newGame();
void redrawGameScreen();


#pragma mark - Global variables

SDL_Window * mainWindow = NULL;
SDL_GLContext mainContext;

GLuint globalProgram = 0;


#pragma mark - Main

GLuint vaoObject;               // temporary
GLuint wheelObjectBuffer;       // temporary
GLuint wheelIndexBuffer;        // temporary
ObjLoader loader;               // temporary

int main(int argc, const char * argv[])
{

    if (!initSDL()) quit(1);
    setupOpenGL();
    
//    myMenuSelection_t sel = kMyMenuSelectionDefault;
//    
//    while (sel != kMyMenuSelectionQuit) {
//        
//        sel = displayMainMenu();
//        
//        switch (sel) {
//            case kMyMenuSelectionMainGame:
//                newGame();
//                break;
//                
//            case kMyMenuSelectionSetting:
//                displaySetting();
//                break;
//                
//            default:
//                break;
//        }
//    }
    
    
    
    
    while (true) {              // temporary
        myGameStatus_t temp;
        processEvents(temp);        // temporary
        redrawGameScreen();     // temporary
    }                           // temporary
    
    quit(0);
    
    // never reached
    return 0;
}


#pragma mark - Setup and event processing functions

bool initSDL() {
    
    // init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL failed to initialize: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // create window with OpenGL context
    mainWindow = SDL_CreateWindow(WINDOW_TITLE,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  STARTING_WINDOW_WIDTH, STARTING_WINDOW_HEIGHT,
                                  SDL_WINDOW_OPENGL);
    mainContext = SDL_GL_CreateContext(mainWindow);
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    // enable depth test, backface culling
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    
    glEnableClientState(GL_INDEX_ARRAY);
    
    // error checking
    if (mainWindow == 0) {
        std::cerr << "Video mode set failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    return true;
}

void setupOpenGL() {
    
#warning Initialize buffer objects and feed data
    
    loader.loadObj(WHEEL_PATH, MTL_BASEPATH);
    
    // temporary
    
    glGenBuffers(1, &wheelObjectBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, wheelObjectBuffer);
    glBufferData(GL_ARRAY_BUFFER, loader.getVertices().size() * sizeof(GLfloat), loader.getVertices().data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &wheelIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wheelIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, loader.getIndices().size() * sizeof(GLuint), loader.getIndices().data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
#warning Initiate vertex array object
    glGenVertexArraysAPPLE(1, &vaoObject);
    glBindVertexArrayAPPLE(vaoObject);
    
    glBindBuffer(GL_ARRAY_BUFFER, wheelObjectBuffer);
    
    GLuint wheelAttribIndex = glGetAttribLocation(globalProgram, "inputCoords");
    glEnableVertexAttribArray(wheelAttribIndex);
    
    int vertexCount = (int) loader.getVertices().size();
    int indiceCount = (int) loader.getIndices().size();
    int triangleCount = indiceCount / 3;
    
    glVertexAttribPointer(wheelAttribIndex, vertexCount, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wheelIndexBuffer);
    
    glBindVertexArrayAPPLE(0);
    
    
#warning Create program and load shaders
    
    // create program
    ProgramCreator myProgramCreator;
    
    myProgramCreator.loadShader(GL_VERTEX_SHADER, MY_VERTEX_SHADER_PATH);
    myProgramCreator.loadShader(GL_FRAGMENT_SHADER, MY_FRAGMENT_SHADER_PATH);
    
    // link program
    globalProgram = myProgramCreator.linkProgram();
    
    // temporary up to here

}

void deleteBuffers() {
    glDeleteBuffers(1, &wheelObjectBuffer);
    glDeleteBuffers(1, &wheelIndexBuffer);
}

static void quit(int exitCode) {
    
    deleteBuffers();
    
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();
    exit(exitCode);
}

static void processEvents(myGameStatus_t &status) {
    
    // Grab all the events off the event queue
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_KEYDOWN:
                keyDownFunc(&event.key.keysym);
                break;
                
            case SDL_QUIT:
                status = kMyGameStatusEnd;
//                quit(5);
                break;
                
            default:
                break;
        }
    }
}

static void keyDownFunc(SDL_Keysym * keysym) {
    
    std::cout << "key down" << std::endl;
    
    switch (keysym -> sym) {
        case SDLK_ESCAPE:
#warning Ideally this should display an in-game menu instead of quitting directly
            quit(0);
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
        redrawGameScreen();
    }
    
}


void redrawGameScreen() {
#warning TODO
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // all temporary here
    
    glUseProgram(globalProgram);
    
    glBindVertexArrayAPPLE(vaoObject);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    glBindVertexArrayAPPLE(0);
    
    glUseProgram(0);
    
    // all temporary up to here
    
    SDL_GL_SwapWindow(mainWindow);
}
