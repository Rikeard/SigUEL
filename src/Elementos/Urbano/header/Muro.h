#ifndef MURO_H
#define MURO_H

#include "../../header/svgObject.h"
#include "../../SVG/header/Line.h"

typedef void* Muro;

svgObject Muro_getElemento(Muro q);

Muro Muro_new(double x1, double y1, double x2, double y2, char* fill, char* borda, char* style);

#endif