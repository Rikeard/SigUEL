#include "header/Text.h"

typedef struct Texto{
    Point coordenada;
    char* txto;
}Texto;

Text Text_new(double x, double y, char* txt){
    Texto *t= malloc(sizeof(Texto));

    t->coordenada = Point_new(x,y);
    
    if(txt == NULL){
        reportError(__func__, "Texto é nulo");
        free(t);
        return NULL;
    }else{
        t->txto = txt;
    }
    return t;
}

char* Text_getText(Text tt){
    Texto *t = tt;
    return t->txto;
}

Point Text_getCoordenada(Text t){
    Texto *tt = t;
    return tt->coordenada;
}

void Text_liberar(Text t){
    Texto* tt = t;
    Point_liberar(tt->coordenada);
    free(tt->txto);
    free(tt);
}