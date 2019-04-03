#ifndef FILEMAKER_H
#define FILEMAKER_H

#include "main.h"

char* toString(svgObject* objeto);
void iniciarSVGFILE(char* path);
void adicionarSVGFile(svgObject* a, FILE* f);
void finalizarSVGFILE(char* path);

#endif