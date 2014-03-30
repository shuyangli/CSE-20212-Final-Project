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
#include "DrawableList.h"


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


void initSDL();
void setupOpenGL();
static void quit(int exitCode);

void deleteObjects();
void deleteBuffers();
void deletePrograms();

static void processEvents(myGameStatus_t &status);
static void keyDownFunc(SDL_Keysym *keysym);

myMenuSelection_t displayMainMenu();
void displaySetting();
void newGame();
void redrawGameScreen();


#pragma mark - Global variables

SDL_Window * mainWindow = NULL;
SDL_GLContext mainContext;

GLuint globalProgram = 0;

std::vector<Drawable *> allDrawableObjects;
std::vector<GLuint> allBuffers;

#pragma mark - Main

int main(int argc, const char * argv[]) {

    initSDL();
    setupOpenGL();
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
    mainWindow = SDL_CreateWindow(WINDOW_TITLE,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  STARTING_WINDOW_WIDTH,
                                  STARTING_WINDOW_HEIGHT,
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
        quit(1);
    }
}

void setupOpenGL() {
    
    // create program
    ProgramCreator myProgramCreator;
    myProgramCreator.loadShader(GL_VERTEX_SHADER, VERTEX_SHADER_PATH);
    myProgramCreator.loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_PATH);
    
    globalProgram = myProgramCreator.linkProgram();
    
    // bind buffer location
    GLint vertexBufferLoc = glGetAttribLocation(globalProgram, ATTRIB_NAME_INPUT_VERTEX);
    
    
    // setup all buffer objects
    // we don't wrap loader into drawable classes because we need to keep track of allocated buffers on gpu memory, and free them when they're out of scope
    ObjLoader loader;
    loader.loadObj(WHEEL_PATH, MTL_BASEPATH);
    
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 loader.getVertices().size() * sizeof(GLfloat),
                 loader.getVertices().data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    allBuffers.push_back(vertexBuffer);
    
    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 loader.getIndices().size() * sizeof(GLuint),
                 loader.getIndices().data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    allBuffers.push_back(indexBuffer);
    
    
    // create drawable objects
    Sample * mySampleObject = new Sample(globalProgram,
                                         vertexBuffer,
                                         vertexBufferLoc,
                                         (unsigned int) loader.getIndices().size(),
                                         indexBuffer);
    allDrawableObjects.push_back(mySampleObject);
}

void deleteObjects() {
    std::for_each(allDrawableObjects.begin(), allDrawableObjects.end(), [](Drawable * obj) {
        delete obj;
    });
    allDrawableObjects.clear();
}

void deleteBuffers() {
    std::cout << "deleting buffers" << std::endl;
    std::for_each(allBuffers.begin(), allBuffers.end(), [](GLuint buffer) {
        glDeleteBuffers(1, &buffer);
    });
}

void deletePrograms() {
    glDeleteProgram(globalProgram);
}

static void quit(int exitCode) {
    
    deleteObjects();
    deletePrograms();
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
                break;
                
            default:
                break;
        }
    }
}

static void keyDownFunc(SDL_Keysym * keysym) {
    
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
    
    // set up program state
    glm::mat4 modelMat = glm::mat4(1.0f);
    glm::mat4 viewMat = glm::lookAt(glm::vec3(8, 5, 5),
                                    glm::vec3(0, 0, 0),
                                    glm::vec3(0, 1, 0));
    glm::mat4 projMat = glm::perspective(glm::radians(45.0f),           // fov
                                         1.0f,                          // width / height ratio
                                         0.1f,                          // near cutoff point
                                         100.0f);                       // far cutoff point
    glm::mat4 mvpMat = projMat * viewMat * modelMat;
    
    // actual drawing (not very efficient, but works)
    std::for_each(allDrawableObjects.begin(), allDrawableObjects.end(), [&mvpMat](Drawable * obj){
        
        glUseProgram(obj -> getProgram());
        
        // bind uniform matrices: this corresponds to program state
#warning this is buggy, since we are giving the objects flexibility to specify program they want to use, we don't know how many or what uniform vars they need in the program
        GLint mvpMatLoc = glGetUniformLocation(globalProgram, UNIFORM_NAME_MVP_MATRIX);
        glUniformMatrix4fv(mvpMatLoc, 1, GL_FALSE, glm::value_ptr(mvpMat));
        
        obj -> draw();
        glUseProgram(0);
    });
    
    SDL_GL_SwapWindow(mainWindow);
}
