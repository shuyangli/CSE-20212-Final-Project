//
//  ImageLoader.cpp
//  GroupProjectCSE20212
//
//  Created by Shuyang Li on 04/21/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include "ImageLoader.h"
#include "Constants.h"

GLuint ImageLoader::loadImageAsTexture(char * filename) {
    
    GLuint textureHandle = -1;
    
    // generate texture
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    
    // we're not using mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // actually loading image
    CGDataProviderRef imageDataProvider = CGDataProviderCreateWithFilename(filename);
    CGImageRef image = CGImageCreateWithJPEGDataProvider(imageDataProvider,
                                                         NULL,
                                                         false,
                                                         kCGRenderingIntentDefault);
    // setup parameters
    GLuint imageWidth = (GLuint) CGImageGetWidth(image);
    GLuint imageHeight = (GLuint) CGImageGetHeight(image);

    // create context
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    void * imageData = malloc(imageWidth * imageHeight * 4);
    CGContextRef context = CGBitmapContextCreate(imageData, imageWidth, imageHeight,
                                                 8, 4 * imageWidth, colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Host);
    CGColorSpaceRelease(colorSpace);
    
    // draw image into context
    CGContextClearRect(context, CGRectMake(0, 0, imageWidth, imageHeight));
    CGContextTranslateCTM(context, 0, 0);
    CGContextDrawImage(context, CGRectMake(0, 0, imageWidth, imageHeight), image);
    
    // use image data from context as texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 imageWidth, imageHeight,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE, imageData);
    
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // clean up
    CGContextRelease(context);
    CGImageRelease(image);
    CGDataProviderRelease(imageDataProvider);
    free(imageData);
    
    // return handle
    return textureHandle;
}