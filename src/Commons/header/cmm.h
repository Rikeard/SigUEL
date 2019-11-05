#ifndef CMM_H
#define CMM_H

#include "../../header/main.h"

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
int safeStrlen(char* str);
char* nullToSpace(char* ss);
bool stringEquals(char* s1, char* s2);
char* safeString(char* str, int size);
void safeFree(void* elemento);

#endif