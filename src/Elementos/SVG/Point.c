#include "header/Point.h"

typedef struct Ponto{
    double x, y;
} Ponto;

Point Point_new(double x, double y){
    Ponto* ps = malloc(sizeof(Ponto));
    ps->x = x;
    ps->y = y;
    return ps;
}

void Point_change(Point p, double x, double y){
    Ponto *ps = p;
    ps->x += x;
    ps->y += y;
}

double Point_getX(Point x){
    Ponto* p = x;
    return p->x;
}

double Point_getY(Point x){
    Ponto* p = x;
    return p->y;
}

void Point_liberar(Point x){
    Ponto* p = x;
    free(p);
}