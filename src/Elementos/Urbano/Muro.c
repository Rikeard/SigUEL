#include "header/Muro.h"

typedef struct Muro_t{
    svgObject elemento;
}Muro_t;

svgObject Muro_getElemento(Muro q){
    Muro_t *x = q;
    return x->elemento;
}

Muro Muro_new(double x1, double y1, double x2, double y2, char* fill, char* borda, char* style){
    Muro_t *Muro = malloc(sizeof(Muro_t));
    Muro->elemento = svgObject_new(LINE, MURO, NULL, Line_newXY(x1, y1, x2, y2), borda, fill, style);
    return Muro;
}
