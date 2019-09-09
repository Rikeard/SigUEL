#include "header/Predio.h"

typedef struct Predio_t{
    svgObject elemento;
}Predio_t;

svgObject Predio_getElemento(Predio q){
    Predio_t *x = q;
    return x->elemento;
}

Predio Predio_new(char* cep, double x, double y, double w, double h, char* fill, char* borda, char* style){
    Predio_t *predio = malloc(sizeof(Predio_t));
    predio->elemento = svgObject_new(RECTANGLE, PREDIO, cep, Rectangle_new(x,y,h,w), borda, fill, style);
    return predio;
}