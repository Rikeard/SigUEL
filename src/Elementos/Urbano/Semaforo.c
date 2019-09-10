#include "header/Semaforo.h"

typedef struct Semaforo_t{
    svgObject elemento;
}Semaforo_t;

svgObject Semaforo_getElemento(Semaforo q){
    if(q == NULL)
        return NULL;
    Semaforo_t *x = q;
    return x->elemento;
}

Semaforo Semaforo_new(char* id, double x, double y, char* fill, char* borda, char* style){
    Semaforo_t *sem = malloc(sizeof(Semaforo_t));
    sem->elemento = svgObject_new(CIRCLE, RADIO, id, Circle_new(x,y, 5),borda, fill, style);
    return sem;
}

bool Semaforo_comparadorID(void* objeto, void* argumento){
    if(objeto != NULL){
        svgObject ob = Semaforo_getElemento(objeto);
        return stringEquals(svgObject_getId(ob), argumento);
    }else{
        return false;
    }
}