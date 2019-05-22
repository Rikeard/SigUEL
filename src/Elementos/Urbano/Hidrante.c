#include "header/Hidrante.h"

typedef struct Hidrante_t{
    svgObject elemento;
}Hidrante_t;

svgObject Hidrante_getElemento(Hidrante q){
    if(q == NULL)
        return NULL;
    Hidrante_t *x = q;
    return x->elemento;
}

Hidrante Hidrante_new(char* id, double x, double y, char* fill, char* borda, char* style){
    Hidrante_t *r = malloc(sizeof(Hidrante_t));
    r->elemento = svgObject_new(CIRCLE, HIDRANTE, id, Circle_new(x,y,5),borda, fill, style);
    return r;
}

bool Hidrante_comparadorID(void* objeto, void* argumento){
    svgObject ob = Hidrante_getElemento(objeto);
    return stringEquals(svgObject_getId(ob), argumento);
}