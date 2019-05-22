#ifndef SEMAFORO_H
#define SEMAFORO_H

#include "../../header/svgObject.h"
#include "../../SVG/header/Circle.h"

typedef void* Semaforo;

svgObject Semaforo_getElemento(Semaforo q);

Semaforo Semaforo_new(char* id, double x, double y, char* fill, char* borda, char* style);

bool Semaforo_comparadorID(void* objeto, void* argumento);

#endif