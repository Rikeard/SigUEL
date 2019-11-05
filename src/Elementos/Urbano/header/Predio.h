#ifndef PREDIO_H
#define PREDIO_H

#include "../../header/svgObject.h"
#include "../../SVG/header/Rectangle.h"

typedef void* Predio;

svgObject Predio_getElemento(Predio q);

Predio Predio_new(char* cep, double x, double y, double w, double h, char* fill, char* borda, char* style);

/* bool Quadra_comparadorID(void* objeto, void* argumento); */

#endif