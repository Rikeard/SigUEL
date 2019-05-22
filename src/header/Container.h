#ifndef CONTAINER_C
#define CONTAINER_C

#include "../Commons/header/listaEncadeada.h"
#include "../Elementos/header/commonSVG.h"

typedef enum Listas{
    L_GERAL, L_SEMAFOROS, L_QUADRAS, L_HIDRANTES, L_RADIOS, L_QUERY
}Listas;

typedef enum styleType{
    S_RECTANGLE, S_CIRCLE, S_QUADRA, S_HIDRANTE, S_SEMAFORO, S_RADIO
}styleType;

typedef enum Styles{
    COR_BORDA, COR_PREENCHIMENTO, STYLE, ESPESSURA
}Styles;

typedef void* Runtime;

void runTime_setStyle(Runtime t, Listas type, char* fill, char* borda, char* style, double stroke);

bool runTime_styleIsSet(Runtime t, Listas type);

double runTime_getBordaSize(Runtime t, Listas type);

char* runTime_getStyle(Runtime t, Listas type, Styles st);

Runtime runTime_new();

listaStatic runTime_getLista(void* runtime, Listas type);

void runTime_setLista(void* runtime, Listas type, listaStatic lista);

#endif