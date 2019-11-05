#ifndef POINT_H
#define POINT_H

#include <stdlib.h>

typedef void* Point;

Point Point_new(double x, double y);

double Point_getX(Point x);

double Point_getY(Point x);

void Point_change(Point p, double x, double y);

void Point_liberar(Point x);

#endif