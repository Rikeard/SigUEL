#include "header/Quadra.h"

typedef struct Quadra_t{
    svgObject elemento;
}Quadra_t;

svgObject Quadra_getElemento(Quadra q){
    Quadra_t *x = q;
    return x->elemento;
}

Quadra Quadra_new(char* cep, double x, double y, double w, double h, char* fill, char* borda, char* style){
    Quadra_t *qadra = malloc(sizeof(Quadra_t));
    qadra->elemento = svgObject_new(RECTANGLE, QUADRA, cep, Rectangle_new(x,y,h,w), borda, fill, style);
    return qadra;
}

bool Quadra_comparadorID(void* objeto, void* argumento){
    svgObject ob = Quadra_getElemento(objeto);
    return stringEquals(svgObject_getId(ob), argumento);
}