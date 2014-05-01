//
//  ImageLoader.cpp
//  GroupProjectCSE20212
//
//  Created by Shuyang Li on 04/21/14.
//  Copyright (c) 2014 Shuyang Li. All rights reserved.
//

#include "ImageLoader.h"
#include "Constants.h"

#include <ImageIO/ImageIO.h>

GLuint ImageLoader::loadImageAsTexture(char * filename) {
    
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
    CGContextRef context = CGBitmapContextCreate(imageData,
                                                 imageWidth,
                                                 imageHeight,
                                                 8,
                                                 4 * imageWidth,
                                                 colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Host);
    
    // draw image into context
    CGContextClearRect(context, CGRectMake(0, 0, imageWidth, imageHeight));
    CGContextTranslateCTM(context, 0, 0);
    
    // fix flip problem
    CGAffineTransform flipVertical = CGAffineTransformMake(1, 0, 0, -1, 0, imageHeight);
    CGContextConcatCTM(context, flipVertical);
    
    CGContextDrawImage(context, CGRectMake(0, 0, imageWidth, imageHeight), image);
    
    
    // generate texture
    GLuint textureHandle;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    
    // we're not using mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // use image data from context as texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 imageWidth, imageHeight,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE, imageData);
    std::cout << "glTexImage2D: " << glGetError() << std::endl;
    
    {
        // debug: write image to file
        CGDataProviderRef writtenImageDataProvider = CGDataProviderCreateWithData(nullptr,
                                                                                  imageData,
                                                                                  imageWidth * imageHeight * 4,
                                                                                  nullptr);
        CGImageRef writtenImage = CGImageCreate(imageWidth,
                                                imageHeight, 8,
                                                24,
                                                imageWidth * 3,
                                                colorSpace,
                                                kCGBitmapByteOrderDefault,
                                                writtenImageDataProvider,
                                                nullptr,
                                                false,
                                                kCGRenderingIntentDefault);
        
        CFStringRef destURLString = CFStringCreateWithCString(nullptr, "test.jpg", kCFStringEncodingUTF8);
        CFStringRef imageTypeString = CFStringCreateWithCString(nullptr, "public.jpeg", kCFStringEncodingUTF8);
        CFURLRef destURL = CFURLCreateWithString(nullptr,
                                                 destURLString,
                                                 nullptr);
        CGImageDestinationRef imageDest = CGImageDestinationCreateWithURL(destURL,
                                                                          imageTypeString,
                                                                          1,
                                                                          nullptr);
        CGImageDestinationAddImage(imageDest,
                                   writtenImage,
                                   nullptr);
        
        CGImageDestinationFinalize(imageDest);
        
        // clean up
        CGDataProviderRelease(writtenImageDataProvider);
        CGImageRelease(writtenImage);
    }
    
    
    CGColorSpaceRelease(colorSpace);
    CGContextRelease(context);
    CGImageRelease(image);
    CGDataProviderRelease(imageDataProvider);
    free(imageData);
    
    // return handle
    return textureHandle;
}