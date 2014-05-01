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
#include "ImageLoader.h"

// GLM headers
#define GLM_FORCE_RADIANS
#include "Helper/glm/glm.hpp"
#include "Helper/glm/gtc/matrix_transform.hpp"      // GLM extensions
#include "Helper/glm/gtc/constants.hpp"             // GLM constants
#warning Need Better Comment
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
void deletePrograms();

void processEvents(myGameStatus_t &status);
void keyDownFunc(SDL_Keysym *keysym, unsigned int deltaTime);

myMenuSelection_t displayMainMenu();
void displaySetting();
void newGame();
void redrawGameScreen();
void calculateObjects();


#pragma mark - Global variables

SDL_Window * globalWindow = NULL;
SDL_GLContext globalGLContext;

GLuint globalProgram = 0;
GLuint globalTexturedProgram = 0;

std::vector<Drawable *> globalDrawableObjects;
std::vector<GLuint> globalBuffers;

Motorcycle * motorcycle;    // Motorcycle object
Track * track;              // Track object
Skybox * skybox;            // textured object
GLuint skyboxTextureHandle = -1;    // texture handle

#pragma mark - Determine collision between the motorcycle and the track
// Returns the area, which is half the determinant of the vertices which should be in order, either clockwise or counterclockwise
float area(const std::vector<glm::vec2> P) {
    float result = 0.0, x1, y1, x2, y2;
    for (int i = 1; i < (int)P.size() - 1; i++) {
        x1 = P[i].x - P[0].x; x2 = P[i+1].x - P[0].x;
        y1 = P[i].y - P[0].y; y2 = P[i+1].y - P[0].y;
        result += (x1 * y2 - x2 * y1);
    }
    return fabsf(result) / 2.0;
}

// Determine if a point is inside a rectangle
bool isPointInsideRectangle(glm::vec2 point, std::vector<glm::vec2> rect) {
    if (rect.size() != 4) return false; // return false if it's not a rectanle
    std::vector<glm::vec2> subrect_1;
    subrect_1.push_back(rect[0]);subrect_1.push_back(rect[1]);subrect_1.push_back(point);
    std::vector<glm::vec2> subrect_2;
    subrect_2.push_back(rect[1]);subrect_2.push_back(rect[2]);subrect_2.push_back(point);
    std::vector<glm::vec2> subrect_3;
    subrect_3.push_back(rect[2]);subrect_3.push_back(rect[3]);subrect_3.push_back(point);
    std::vector<glm::vec2> subrect_4;
    subrect_4.push_back(rect[3]);subrect_4.push_back(rect[0]);subrect_4.push_back(point);
    float subrect_1_area = area(subrect_1);
    float subrect_2_area = area(subrect_2);
    float subrect_3_area = area(subrect_3);
    float subrect_4_area = area(subrect_4);
    float rect_area = area(rect);
    float subrects_area_sum = subrect_1_area + subrect_2_area + subrect_3_area + subrect_4_area;
    return subrects_area_sum <= rect_area;
}

// Determine if the motorcycle is on track
bool isMotorcycleOnTrack(Motorcycle *motorcycle, Track *track)           // Determines if the motorcycle is in on the road
{
    std::vector<glm::vec2> wrappingBoxVertices = motorcycle->getWrappingBox();
    std::vector<glm::vec2> walls = track->getWalls();
    for (int i = 0; i < walls.size(); ++i) {
        glm::vec2 point = walls[i];
        if (isPointInsideRectangle(point, wrappingBoxVertices))
            return false;
    }
    return true;
}

#pragma mark - Main

int main(int argc, const char * argv[]) {

    initSDL();
    initOpenGL();
    
    myMenuSelection_t sel = kMyMenuSelectionDefault;
    
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
    
    glEnable(GL_TEXTURE_2D);
    
    // error checking
    if (globalWindow == 0) {
        std::cerr << "Video mode set failed: " << SDL_GetError() << std::endl;
        quit(1);
    }
    
    std::cout << "InitSDL: " << glGetError() << std::endl;
}

void initOpenGL() {
    
    // create program
    ProgramCreator myProgramCreator;
    myProgramCreator.loadShader(GL_VERTEX_SHADER, VERTEX_SHADER_PATH);
    myProgramCreator.loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_PATH);
    globalProgram = myProgramCreator.linkProgram();
    
    ProgramCreator myTexturedProgramCreator;
    myTexturedProgramCreator.loadShader(GL_VERTEX_SHADER, TEXTURED_VERTEX_SHADER_PATH);
    myTexturedProgramCreator.loadShader(GL_FRAGMENT_SHADER, TEXTURED_FRAGMENT_SHADER_PATH);
    globalTexturedProgram = myTexturedProgramCreator.linkProgram();
    std::cout << "Link global textured program: " << glGetError() << std::endl;
    
    // bind buffer location
    GLint vertexBufferLoc = glGetAttribLocation(globalProgram, ATTRIB_NAME_INPUT_VERTEX);
    if (vertexBufferLoc == -1) quit(9);
    GLint normalBufferLoc = glGetAttribLocation(globalProgram, ATTRIB_NAME_INPUT_NORMAL);
    if (normalBufferLoc == -1) quit(9);
    
    GLint texVertexBufferLoc = glGetAttribLocation(globalTexturedProgram, ATTRIB_NAME_INPUT_VERTEX);
    if (texVertexBufferLoc == -1) quit(9);
    GLint texUVBufferLoc = glGetAttribLocation(globalTexturedProgram, ATTRIB_NAME_INPUT_UV);
    if (texUVBufferLoc == -1) quit(9);
    
    // setup all objects
    motorcycle = new Motorcycle(vertexBufferLoc,
                                normalBufferLoc,
                                glm::vec3(0, 0, 0),
                                glm::vec3(1, 0, 0),
                                0.0001f,
                                0.05f);
    globalDrawableObjects.push_back(motorcycle);
    
    track = new Track(vertexBufferLoc, normalBufferLoc);
    
    globalDrawableObjects.push_back(track);
    
    skyboxTextureHandle = ImageLoader::loadImageAsTexture(SKYBOX_TEXTURE_PATH);
    std::cout << "loadImageAsTexture: " << glGetError() << std::endl;
    
    skybox = new Skybox(texVertexBufferLoc, normalBufferLoc, texUVBufferLoc);
    std::cout << "new Skybox:" << glGetError() << std::endl;
//    globalDrawableObjects.push_back(new Ground(vertexBufferLoc, normalBufferLoc));
    
    std::cout << "InitOpenGL: " << glGetError() << std::endl;
}

void deleteObjects() {
    std::for_each(globalDrawableObjects.begin(), globalDrawableObjects.end(), [](Drawable * obj) {
        delete obj;
    });
    globalDrawableObjects.clear();
    motorcycle = nullptr;
    
    glDeleteTextures(1, &skyboxTextureHandle);
}

void deletePrograms() {
    glDeleteProgram(globalProgram);
}

void quit(int exitCode) {
    
    deleteObjects();
    deletePrograms();
    SDL_GL_DeleteContext(globalGLContext);
    SDL_DestroyWindow(globalWindow);
    SDL_Quit();
    exit(exitCode);
}

void processEvents(myGameStatus_t &status) {
    
    // initial setup for timer
    static unsigned int lastTick = SDL_GetTicks();
    
    // calculate delta time for each time events are processed
    unsigned int deltaTime = SDL_GetTicks() - lastTick;
    std::cout << deltaTime << std::endl;
    // Grab all the events off the event queue
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        
        switch(event.type) {
            case SDL_KEYDOWN:
                keyDownFunc(&event.key.keysym, deltaTime);
                break;
                
            case SDL_QUIT:
                status = kMyGameStatusEnd;
                break;
                
            default:
                break;
        }
    }
    
    // move motorcycle here
    motorcycle -> move(deltaTime);
    
    static int collisionCount = 0;
    
    if (!isMotorcycleOnTrack(motorcycle, track)) {
        std::cout << ++collisionCount << "\nCollision!" << std::endl;
        int n = 4;
        while (n--)
        motorcycle->turnLeft(deltaTime);
    }
    
    // update time after each frame
    lastTick += deltaTime;
}

void keyDownFunc(SDL_Keysym * keysym, unsigned int deltaTime) {
    
    switch (keysym -> sym) {
        case SDLK_ESCAPE:
            quit(0);
            break;
            
        case SDLK_UP:
            motorcycle -> incSpeed(deltaTime);
            break;
            
        case SDLK_DOWN:
            motorcycle -> decSpeed(deltaTime);
            break;
            
        case SDLK_LEFT:
            motorcycle -> turnLeft(deltaTime);
            break;
            
        case SDLK_RIGHT:
            motorcycle -> turnRight(deltaTime);
            break;
            
        default:
            break;
    }
}


#pragma mark - Game Functions

myMenuSelection_t displayMainMenu() {
    myMenuSelection_t tempSel = kMyMenuSelectionDefault;
    
    tempSel = kMyMenuSelectionMainGame;
    
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
    std::for_each(globalDrawableObjects.begin(), globalDrawableObjects.end(), [](Drawable * obj){
        obj -> calculateModelMatrix();
    });
}

void redrawGameScreen() {
    
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // get uniform location
    GLint mvpMatLoc = glGetUniformLocation(globalProgram, UNIFORM_NAME_MVP_MATRIX);
    GLint normalModelViewMatLoc = glGetUniformLocation(globalProgram, UNIFORM_NAME_NORMAL_MV_MATRIX);
    GLint directionToLightLoc = glGetUniformLocation(globalProgram, UNIFORM_NAME_DIRECTION_TO_LIGHT);
    GLint lightIntensityLoc = glGetUniformLocation(globalProgram, UNIFORM_NAME_LIGHT_INTENSITY);
    GLint ambientLightIntensityLoc = glGetUniformLocation(globalProgram, UNIFORM_NAME_AMBIENT_INTENSITY);
    GLint materialColorLoc = glGetUniformLocation(globalProgram, UNIFORM_NAME_MATERIAL_COLOR);
    
    GLint texMvpMatLoc = glGetUniformLocation(globalTexturedProgram, UNIFORM_NAME_MVP_MATRIX);
    GLint texSamplerLoc = glGetUniformLocation(globalTexturedProgram, UNIFORM_NAME_TEXTURE_SAMPLER);
    
    // render untextured objects
    glUseProgram(globalProgram);
    
    // proj matrix (clip space) only changes when fov or aspect ratio changes, so we don't modify it
    static const glm::mat4 projMat = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 1000.0f);
    
    // actual drawing (not very efficient, but works)
    std::for_each(globalDrawableObjects.begin(), globalDrawableObjects.end(), [&](Drawable * obj){
        
        glm::mat4 modelMat = obj -> getModelMatrix();
        glm::mat4 viewMat = glm::lookAt(motorcycle->getCameraLocation(),
                                        motorcycle->getCameraFocus(),
                                        glm::vec3(0, 1, 0));
        glm::mat4 mvpMat = projMat * viewMat * modelMat;
        glm::mat3 mvMat = glm::transpose(glm::inverse(glm::mat3(viewMat * modelMat)));
        
        // bind uniforms
        glUniformMatrix4fv(mvpMatLoc, 1, GL_FALSE, glm::value_ptr(mvpMat));

        // for lighting
        glUniformMatrix3fv(normalModelViewMatLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
        glUniform4f(materialColorLoc, 0.6f, 0.6f, 0.6f, 1.0f);
        glUniform3fv(directionToLightLoc, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
        glUniform4f(lightIntensityLoc, 0.7f, 0.7f, 0.7f, 1.0f);
        glUniform4f(ambientLightIntensityLoc, 0.3f, 0.3f, 0.3f, 1.0f);
        
        obj -> draw();
    });
    
    // used textured program to render skybox
    glUseProgram(globalTexturedProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skyboxTextureHandle);
    glUniform1i(texSamplerLoc, 0);                          // "TEXTURE 0"
    glm::mat4 modelMat = glm::mat4(50.0f);
    glm::mat4 viewMat = glm::lookAt(motorcycle->getCameraLocation(),
                                    motorcycle->getCameraFocus(),
                                    glm::vec3(0, 1, 0));
    glm::mat4 mvpMat = projMat * viewMat * modelMat;
    glUniformMatrix4fv(texMvpMatLoc, 1, GL_FALSE, glm::value_ptr(mvpMat));
    std::cout << "glUniformMatrix4fv: " << glGetError() << std::endl;
    skybox -> draw();
    
    glUseProgram(0);
    
    SDL_GL_SwapWindow(globalWindow);
}
