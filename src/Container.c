#include "header/Container.h"

typedef struct Style_t{
    char* corPreenchimento;
    char* corBorda;
    char* style;
    double espessura;
    bool isSet;
}Style_t;

typedef struct Runtime_t{
    listaStatic l_geral;
    listaStatic l_quadras;
    listaStatic l_hidrantes;
    listaStatic l_radio;
    listaStatic l_semaforo;
    listaStatic l_query;

    Style_t s_circulo;
    Style_t s_retangulo;
    Style_t s_quadra;
    Style_t s_hidrante;
    Style_t s_radio;
    Style_t s_semaforo;

} Runtime_t;

void runTime_setStyle(Runtime t, Listas type, char* fill, char* borda, char* style, double stroke){
    Runtime_t *r = t;
    Style_t *s;
    switch(type){
        case S_RECTANGLE:
            s = &(r->s_retangulo);
            break;
        case S_CIRCLE:
            s = &(r->s_circulo);
            break;
        case S_QUADRA:
            s = &(r->s_quadra);
            break;
        case S_HIDRANTE:
            s = &(r->s_hidrante);
            break;
        case S_RADIO:
            s = &(r->s_radio);
            break;
        case S_SEMAFORO:
            s = &(r->s_semaforo);
            break;
        default:
            reportError(__func__, "Unhandled type");
            return;
            break;
    }

    s->isSet = true;
    s->corBorda = safeString(borda, 32);
    s->corPreenchimento = safeString(fill, 32);
    s->style = safeString(style, 32);
    s->espessura = stroke;

    if(style == NULL){
        char* tempBuffer = calloc(64, sizeof(char));
        sprintf(tempBuffer, "stroke-width:%lf;", stroke);
        s->style = tempBuffer;
    }else{
        char* tempBuffer = calloc(128, sizeof(char));
        sprintf(tempBuffer, "%sstroke-width:%lf;", style, stroke);
        s->style = tempBuffer;
    }
}

double runTime_getBordaSize(Runtime t, Listas type){
    Runtime_t *r = t;
    Style_t *s;
    switch(type){
        case S_RECTANGLE:
            s = &(r->s_retangulo);
            break;
        case S_CIRCLE:
            s = &(r->s_circulo);
            break;
        case S_QUADRA:
            s = &(r->s_quadra);
            break;
        case S_HIDRANTE:
            s = &(r->s_hidrante);
            break;
        case S_RADIO:
            s = &(r->s_radio);
            break;
        case S_SEMAFORO:
            s = &(r->s_semaforo);
            break;
        default:
            reportError(__func__, "Unhandled type");
            return 0;
            break;
    }

    return s->espessura;
}

bool runTime_styleIsSet(Runtime t, Listas type){
    Runtime_t *r = t;
    Style_t *s;
    switch(type){
        case S_RECTANGLE:
            s = &(r->s_retangulo);
            break;
        case S_CIRCLE:
            s = &(r->s_circulo);
            break;
        case S_QUADRA:
            s = &(r->s_quadra);
            break;
        case S_HIDRANTE:
            s = &(r->s_hidrante);
            break;
        case S_RADIO:
            s = &(r->s_radio);
            break;
        case S_SEMAFORO:
            s = &(r->s_semaforo);
            break;
        default:
            reportError(__func__, "Unhandled type");
            return NULL;
            break;
    }

    return s->isSet;
}

char* runTime_getStyle(Runtime t, Listas type, Styles st){
    Runtime_t *r = t;
    Style_t *s;
    switch(type){
        case S_RECTANGLE:
            s = &(r->s_retangulo);
            break;
        case S_CIRCLE:
            s = &(r->s_circulo);
            break;
        case S_QUADRA:
            s = &(r->s_quadra);
            break;
        case S_HIDRANTE:
            s = &(r->s_hidrante);
            break;
        case S_RADIO:
            s = &(r->s_radio);
            break;
        case S_SEMAFORO:
            s = &(r->s_semaforo);
            break;
        default:
            reportError(__func__, "Unhandled type");
            return NULL;
            break;
    }

    switch(st){
        case COR_BORDA:
            return s->corBorda;
            break;
        case COR_PREENCHIMENTO:
            return s->corPreenchimento;
            break;
        case STYLE:
            return s->style;
            break;
        default:
            reportError(__func__, "Unhandled type");
            return NULL;
            break;
    }
}

Runtime runTime_new(){
    Runtime_t *t = malloc(sizeof(Runtime_t));
    t->l_geral = listaStatic_new(1000);
    t->l_quadras = listaStatic_new(1000);
    t->l_hidrantes = listaStatic_new(1000);
    t->l_radio = listaStatic_new(1000);
    t->l_semaforo = listaStatic_new(1000);
    t->l_query = listaStatic_new(100000);

    t->s_hidrante.isSet = false;
    t->s_circulo.isSet = false;
    t->s_quadra.isSet = false;
    t->s_radio.isSet = false;
    t->s_semaforo.isSet = false;
    t->s_retangulo.isSet = false;

    return t;
}

listaStatic runTime_getLista(Runtime runtime, Listas type){
    Runtime_t *r = runtime;
    switch(type){
        case L_GERAL:
            return r->l_geral;
            break;
        case L_QUADRAS:
            return r->l_quadras;
            break;
        case L_HIDRANTES:
            return r->l_hidrantes;
            break;
        case L_RADIOS:
            return r->l_radio;
            break;
        case L_SEMAFOROS:
            return r->l_semaforo;
            break;
        case L_QUERY:
            return r->l_query;
            break;
        default:
            reportError(__func__, "Unhandled type");
            return NULL;
            break;
    }
}

void runTime_setLista(void* runtime, Listas type, listaStatic lista){
    Runtime_t *r = runtime;
    switch(type){
        case L_GERAL:
            r->l_geral = lista;
            break;
        case L_QUADRAS:
            r->l_quadras = lista;
            break;
        case L_HIDRANTES:
            r->l_hidrantes = lista;
            break;
        case L_RADIOS:
            r->l_radio = lista;
            break;
        case L_SEMAFOROS:
            r->l_semaforo = lista;
            break;
        case L_QUERY:
            r->l_query = lista;
            break;
        default:
            reportError(__func__, "Unhandled type");
            break;
    }
}