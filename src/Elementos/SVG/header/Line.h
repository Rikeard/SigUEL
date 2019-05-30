#ifndef LINE_H
#define LINE_H

#include "Point.h"
#include <stdlib.h>

typedef void* Line;

Line Line_newXY(double x1, double y1, double x2, double y2);

Line Line_newCord(Point coord1, Point coord2);

Point Line_getCoordenada1(Line line);

Point Line_getCoordenada2(Line line);

void Line_liberar(Line line);

#endif