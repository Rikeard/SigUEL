#ifndef HIDRANTE_H
#define HIDRANTE_H

#include "../../header/svgObject.h"
#include "../../SVG/header/Circle.h"

typedef void* Hidrante;

svgObject Hidrante_getElemento(Hidrante q);

Hidrante Hidrante_new(char* id, double x, double y, char* fill, char* borda, char* style);

bool Hidrante_comparadorID(void* objeto, void* argumento);

#endif