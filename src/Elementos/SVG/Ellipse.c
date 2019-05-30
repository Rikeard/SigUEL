#include "header/Ellipse.h"

typedef struct Elipse{
    Point coordenada;
    double raioX, raioY;
} Elipse;

Ellipse Ellipse_new(double x, double y, double raioX, double raioY){
    Elipse* el = malloc(sizeof(Elipse));
    el->raioX = raioX;
    el->raioY = raioY;
    el->coordenada = Point_new(x,y);
    return el;
}

Point Ellipse_getCoordenada(Ellipse elp){
    Elipse *el = elp;
    return el->coordenada;
}

double Ellipse_getRaioX(Ellipse elp){
    Elipse *el = elp;
    return el->raioX;
}

double Ellipse_getRaioY(Ellipse elp){
    Elipse *el = elp;
    return el->raioY;
}

void Ellipse_liberar(Ellipse elp){
    Elipse *el = elp;
    Point_liberar(el->coordenada);
    free(el);
}