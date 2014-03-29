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

// temp
const GLfloat vertexPositions[] = {
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f
};

const GLuint indexPositions[] = {
    0, 1, 2,
    0, 2, 3,
    3, 2, 4,
    3, 4, 5,
    6, 4, 2,
    6, 2, 1,
    0, 7, 6,
    0, 6, 1,
    5, 7, 0,
    5, 0, 3,
    5, 4, 6,
    5, 6, 7
};

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
    
    // error checking
    if (mainWindow == 0) {
        std::cerr << "Video mode set failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    return true;
}

void setupOpenGL() {
    
#warning Setup buffer objects
    
    loader.loadObj(WHEEL_PATH, MTL_BASEPATH);
    
    // temporary
    
    glGenBuffers(1, &wheelObjectBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, wheelObjectBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER, loader.getVertices().size() * sizeof(GLfloat), loader.getVertices().data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &wheelIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wheelIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexPositions), indexPositions, GL_STATIC_DRAW);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, loader.getIndices().size() * sizeof(GLuint), loader.getIndices().data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    
#warning Create program
    
    // create program
    ProgramCreator myProgramCreator;
    
    myProgramCreator.loadShader(GL_VERTEX_SHADER, MY_VERTEX_SHADER_PATH);
    myProgramCreator.loadShader(GL_FRAGMENT_SHADER, MY_FRAGMENT_SHADER_PATH);
    
    // link program
    globalProgram = myProgramCreator.linkProgram();
    
    
#warning Initiate vertex array object
    glGenVertexArraysAPPLE(1, &vaoObject);
    glBindVertexArrayAPPLE(vaoObject);
    
    GLint wheelAttribIndex = glGetAttribLocation(globalProgram, "inputCoords");
    glEnableVertexAttribArray(wheelAttribIndex);
    glBindBuffer(GL_ARRAY_BUFFER, wheelObjectBuffer);
    
    glVertexAttribPointer(wheelAttribIndex,
                          3,                // number of components per vertex
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          0);               // this call changes vao state
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wheelIndexBuffer);
    glBindVertexArrayAPPLE(0);
    
    // temporary up to here
}

void deleteBuffers() {
    glDeleteVertexArraysAPPLE(1, &vaoObject);
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
  
    // camera angle
    // query mouse position
    int mouseX, mouseY, windowWidth, windowHeight;
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_GetWindowSize(mainWindow, &windowWidth, &windowHeight);
    SDL_WarpMouseInWindow(mainWindow, windowWidth / 2, windowHeight / 2);
    
    // figure out view angle
    static GLfloat horizontalAngle = 0.0f, verticalAngle = 0.0f;
    static float mouseSpeed = 0.005f;
    
    horizontalAngle += mouseSpeed * (float)(windowWidth / 2 - mouseX);
    verticalAngle += mouseSpeed * (float)(windowHeight / 2 - mouseY);
    
    // get direction vector
    glm::vec3 directionVec(cos(verticalAngle) * sin(horizontalAngle),
                           sin(verticalAngle),
                           cos(verticalAngle) * cos(horizontalAngle));
    glm::vec3 rightVec(sin(horizontalAngle - (glm::pi<GLfloat>() / 2.0f)),
                       0.0f,
                       cos(horizontalAngle - (glm::pi<GLfloat>() / 2.0f)));
    glm::vec3 upVec = glm::cross(rightVec, directionVec);
    
    static glm::vec3 position(6, 3, 3);
    
    glm::mat4 modelMat = glm::mat4(1.0f);
    glm::mat4 viewMat = glm::lookAt(position,  // position is unchanged
                                    position + directionVec,
                                    upVec);
    glm::mat4 projMat = glm::perspective(glm::radians(45.0f),           // fov
                                         1.0f,                          // width / height ratio
                                         0.1f,                          // near cutoff point
                                         100.0f);                       // far cutoff point
    glm::mat4 mvpMat = projMat * viewMat * modelMat;
    
    GLint mvpMatLoc = glGetUniformLocation(globalProgram, "mvpMat");
    
    glUniformMatrix4fv(mvpMatLoc, 1, GL_FALSE, glm::value_ptr(mvpMat)); // this correctly happen in rendering step as part of program state
    
    // actual drawing
    glBindVertexArrayAPPLE(vaoObject);
    glDrawElements(GL_TRIANGLES,
                   36,                                                  // *** number of VERTICES, NOT TRIANGLES ***
                   GL_UNSIGNED_INT,
                   0);
    glBindVertexArrayAPPLE(0);
    
    glUseProgram(0);
    
    // all temporary up to here
    
    SDL_GL_SwapWindow(mainWindow);
}
