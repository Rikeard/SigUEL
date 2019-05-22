#ifndef RADIO_H
#define RADIO_H

#include "../../header/svgObject.h"
#include "../../SVG/header/Circle.h"

typedef void* Radio;

svgObject Radio_getElemento(Radio q);

Radio Radio_new(char* id, double x, double y, char* fill, char* borda, char* style);

bool Radio_comparadorID(void* objeto, void* argumento);

#endif