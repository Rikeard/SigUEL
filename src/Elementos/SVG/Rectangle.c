#include "header/Rectangle.h"

typedef struct Retangulo{
    Point* coordenada;
    double height, width;
}Retangulo;

Rectangle Rectangle_new(double x, double y, double h, double w){
    Retangulo *rt = malloc(sizeof(Retangulo));
    rt->height = h;
    rt->width = w;
    rt->coordenada = Point_new(x, y);
    return rt;
}

Point Rectangle_getCoordenada(Rectangle rect){
    Retangulo* rt = rect;
    return rt->coordenada;
}

double Rectangle_getAltura(Rectangle rect){
    Retangulo* rt = rect;
    return rt->height;
}

double Rectangle_getLargura(Rectangle rect){
    Retangulo* rt = rect;
    return rt->width;
}

void Rectangle_liberar(Rectangle rect){
    Retangulo *x = rect;
    Point_liberar(x->coordenada);
    free(x);
}