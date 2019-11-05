#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Point.h"

typedef void* Triangle;

Triangle Triangle_new(Point p1, Point p2, Point p3);

Point Triangle_getP1(Triangle t);

Point Triangle_getP2(Triangle t);

Point Triangle_getP3(Triangle t);

void Triangle_liberar(Triangle t);

#endif