#include "header/Circle.h"

typedef struct Circulo{
    Point* coordenada;
    double raio;
}Circulo;

Circle Circle_new(double x, double y, double r){
    Circulo *c = malloc(sizeof(Circulo));
    c->raio = r;
    c->coordenada = Point_new(x, y); 
}

double Circle_getRaio(Circle* circle){
    Circulo *c = circle;
    return c->raio;
}

Point Circle_getCoordenada(Circle* circle){
    Circulo *c = circle;
    return c->coordenada;
}

void Circle_liberar(Circle* circle){
    Circulo *x  = circle;
    Point_liberar(x->coordenada);
    free(circle);
}