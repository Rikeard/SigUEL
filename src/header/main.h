#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#if defined(_WIN32)
    #define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
    #define PLATFORM PLATFORM_MAC
#else
    #define PLATFORM PLATFORM_UNIX
#endif

void reportError(const char* funcao, char* error);
void reportSucess(const char* funcao);
void defaultInit();

typedef struct treeNode treeNode;
typedef struct Rectangle Rectangle;
typedef struct Circle Circle;
typedef struct svgObject svgObject;
typedef struct SVG SVG;
typedef enum geometryType geometryType;


#endif