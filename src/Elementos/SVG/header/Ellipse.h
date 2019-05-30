#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "Point.h"

typedef void* Ellipse;

Ellipse Ellipse_new(double x, double y, double raioX, double raioY);

Point Ellipse_getCoordenada(Ellipse elp);

double Ellipse_getRaioX(Ellipse elp);

double Ellipse_getRaioY(Ellipse elp);

void Ellipse_liberar(Ellipse elp);

#endif