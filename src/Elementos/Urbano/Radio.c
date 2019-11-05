#include "header/Radio.h"

typedef struct Radio_t{
    svgObject elemento;
}Radio_t;

svgObject Radio_getElemento(Radio q){
    if(q == NULL)
        return NULL;
    Radio_t *x = q;
    return x->elemento;
}

Radio Radio_new(char* id, double x, double y, char* fill, char* borda, char* style){
    Radio_t *r = malloc(sizeof(Radio_t));
    r->elemento = svgObject_new(CIRCLE, RADIO, id, Circle_new(x,y,5),borda, fill, style);
    return r;
}

bool Radio_comparadorID(void* objeto, void* argumento){
    svgObject ob = Radio_getElemento(objeto);
    return stringEquals(svgObject_getId(ob), argumento);
}