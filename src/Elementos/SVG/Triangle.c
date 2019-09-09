#include "header/Triangle.h"

typedef struct Triangulo_t{
    Point p1, p2, p3;
}Triangulo_t;

Triangle Triangle_new(Point p1, Point p2, Point p3){
    Triangle tr = malloc(sizeof(Triangulo_t));
    Triangulo_t* t = (Triangulo_t*) tr;

    t->p1 = p1;
    t->p2 = p2;
    t->p3 = p3;

    return tr;    
}

Point Triangle_getP1(Triangle t){
    Triangulo_t* rt = t;
    return rt->p1;
}

Point Triangle_getP2(Triangle t){
    Triangulo_t* rt = t;
    return rt->p2;
}

Point Triangle_getP3(Triangle t){
    Triangulo_t* rt = t;
    return rt->p3;
}

void Triangle_liberar(Triangle t){
    Triangulo_t *x = t;
    Point_liberar(x->p1);
    Point_liberar(x->p2);
    Point_liberar(x->p3);
    free(x);
}