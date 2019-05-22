#ifndef QUERYHANDLER_H
#define QUERYHANDLER_H

#include "../Elementos/SVG/header/Circle.h"
#include "../Elementos/header/svgObject.h"
#include "../Commons/header/cmm.h"
#include "geometry.h"
#include "../Commons/header/listaEncadeada.h"
#include "Container.h"
#include "fileMaker.h"
#include "fileHandler.h"
#include "../Elementos/Urbano/header/Hidrante.h"
#include "../Elementos/Urbano/header/Quadra.h"
#include "../Elementos/Urbano/header/Semaforo.h"
#include "../Elementos/Urbano/header/Radio.h"

void geo_Nx(char* comando, Runtime rt);

void geo_C(char* comando, listaStatic lista, Runtime rt);

void geo_R(char* comando, listaStatic lista, Runtime rt);

void geo_T(char* comando, listaStatic lista);

void qry_O(char* comando, listaStatic lista, FILE* respostaFile, listaStatic svgList);

void qry_I(char* comando, listaStatic lista, FILE* respostaFile, listaStatic svgList);

void qry_D(char* comando, listaStatic lista, FILE *respostaFile, listaStatic svgList);

void qry_BB(char* comando, listaStatic lista, char* DirComNomeBase);

/*
# T2
*/

void geo_Q(char* comando, listaStatic quadra, Runtime rt);
void geo_H(char* comando, listaStatic hidra, Runtime rt);

void geo_S(char* comando, listaStatic sema, Runtime rt);

void geo_RB(char* comando, listaStatic radio, Runtime rt);

void geo_CQ(char* comando, Runtime rt);
void geo_CH(char* comando, Runtime rt);

void geo_CR(char* comando, Runtime rt);
void geo_CS(char* comando, Runtime rt);
void geo_SW(char* comando, Runtime rt);

void qry_DQ(char* comando, Runtime rt, FILE* respostaFile);

void qry_Del(char* comando, Runtime rt, FILE* respostaFile);

void qry_CBQ(char* comando, listaStatic quadras, FILE* respostaFile);

void qry_CRD(char* comando, Runtime rt, FILE* respostaFile);

void qry_Trns(char* comando, Runtime rt, FILE* respostaFile);



#endif