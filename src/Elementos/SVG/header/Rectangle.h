#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Point.h"

typedef void* Rectangle;

Rectangle Rectangle_new(double x, double y, double h, double w);

Point Rectangle_getCoordenada(Rectangle rect);

double Rectangle_getAltura(Rectangle rect);

double Rectangle_getLargura(Rectangle rect);

void Rectangle_liberar(Rectangle rect);

#endif