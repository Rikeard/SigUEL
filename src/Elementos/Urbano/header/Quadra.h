#ifndef QUADRA_H
#define QUADRA_H

#include "../../header/svgObject.h"
#include "../../SVG/header/Rectangle.h"

typedef void* Quadra;

svgObject Quadra_getElemento(Quadra q);

Quadra Quadra_new(char* cep, double x, double y, double w, double h, char* fill, char* borda, char* style);

bool Quadra_comparadorID(void* objeto, void* argumento);

#endif