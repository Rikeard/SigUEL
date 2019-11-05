#ifndef CIRCLE_H
#define CIRCLE_H

#include "Point.h"

typedef void* Circle;

Circle Circle_new(double x, double y, double r);

double Circle_getRaio(Circle circle);

Point Circle_getCoordenada(Circle circle);

void Circle_liberar(Circle circle);

#endif