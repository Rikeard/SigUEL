#include "header/svgObject.h"

typedef struct svgObjecto{
    geometryType tipo;
    Classe cl;
    char* id;
    void *elemento; //Usar o geometryType pra saber qual tipo instanciar
    char* corBorda;
    char* corPreenchimento;
    char* style;
}svgObjecto;

void svgObject_liberar(svgObject x){
    svgObjecto *z = x;
    SVG_Liberar(z->tipo, z->elemento);
    safeFree(z->id);
    safeFree(z->corBorda);
    safeFree(z->corPreenchimento);
    safeFree(z->style);
    free(z);
}

Classe svgObject_getClasse(svgObject x){
    svgObjecto *z = x;
    return z->cl;
}

void svgObject_setClasse(svgObject x, Classe c){
    svgObjecto *z = x;
    z->cl = c;
}

void* svgObject_getElemento(svgObject x){
    svgObjecto *z = x;
    return z->elemento;
}

char* svgObject_getId(svgObject x){
    svgObjecto *z = x;
    return z->id;
}

void svgObject_setID(svgObject x, char* idt){
    svgObjecto *z = x;
    char* sz = calloc(64, sizeof(char));
    sprintf(sz, "%s", idt);
    z->id = sz;
}

void svgObject_setStyle(svgObject x, char* cor){
    svgObjecto *z = x;
    char* sz = calloc(64, sizeof(char));
    sprintf(sz, "%s", cor);
    z->style = sz;
}

void svgObject_setCorBorda(svgObject x, char* cor){
    svgObjecto *z = x;
    char* sz = calloc(64, sizeof(char));
    sprintf(sz, "%s", cor);
    z->corBorda = sz;
}

void svgObject_setCorPreenchimento(svgObject x, char* cor){
    svgObjecto *z = x;
    char* sz = calloc(64, sizeof(char));
    sprintf(sz, "%s", cor);
    z->corPreenchimento = sz;
}

geometryType svgObject_getTipo(svgObject x){
    svgObjecto *z = x;
    return z->tipo;
}

char* svgObject_getStyle(svgObject x){
    svgObjecto *z = x;
    return z->style;
}

bool svgObject_haveStyle(svgObject x){
    svgObjecto *z = x;
    if(z->style == NULL){
        return false;
    }else{
        return true;
    }
}

char* svgObject_getCorBorda(svgObject x){
    svgObjecto *z = x;
    return z->corBorda;
}

char* svgObject_getCorPreenchimento(svgObject x){
    svgObjecto *z = x;
    return z->corPreenchimento;
}

bool svgObject_comparadorID(void* objeto, void* argumento){
    svgObjecto *ob = objeto;
    return stringEquals(ob->id, argumento);
}

svgObject svgObject_new(geometryType t, Classe cls, char* id, void* elemento, char* corBorda, char* corPreenchimento, char* style){
    svgObjecto* obj = malloc(sizeof(svgObjecto));
    obj->tipo = t;
    obj->elemento = elemento;
    
    char *cb = NULL, *cp = NULL, *st = NULL;

    if(id == NULL || safeStrlen(id) < 1){
        obj->id = NULL;
    }else{
        char* strnew = calloc(safeStrlen(id)+1, sizeof(char));
        sprintf(strnew, "%s", id);
        obj->id = strnew;
    }

    if(corBorda != NULL){
        if(safeStrlen(corBorda) > 1){
            cb = calloc((safeStrlen(corBorda)+1),sizeof(char));
            sprintf(cb, "%s", corBorda);
        }
    }

    if(corPreenchimento != NULL ){
        if(safeStrlen(corPreenchimento) > 1){
            cp = calloc((safeStrlen(corPreenchimento)+1),sizeof(char));
            sprintf(cp, "%s", corPreenchimento);
        }
    }

    if(style != NULL){
        if(safeStrlen(style) > 1){
            st = calloc((safeStrlen(style)+1),sizeof(char));
            sprintf(st, "%s", style);
        }
    }

    obj->corBorda = cb;
    obj->corPreenchimento = cp;
    obj->style = st;
    obj->cl = cls;
}