#ifndef SVGOBJECT_H
#define SVGOBJECT_H

#include "commonSVG.h"
#include "../../Commons/header/cmm.h"

typedef void* svgObject;

void svgObject_liberar(svgObject x);

void* svgObject_getElemento(svgObject x);

bool svgObject_haveStyle(svgObject x);

char* svgObject_getId(svgObject x);

void svgObject_setID(svgObject x, char* idt);

void svgObject_setStyle(svgObject x, char* cor);

void svgObject_setCorBorda(svgObject x, char* cor);

void svgObject_setCorPreenchimento(svgObject x, char* cor);

geometryType svgObject_getTipo(svgObject x);

char* svgObject_getStyle(svgObject x);

char* svgObject_getCorBorda(svgObject x);

char* svgObject_getCorPreenchimento(svgObject x);

svgObject svgObject_new(geometryType t, Classe cls, char* id, void* elemento, char* corBorda, char* corPreenchimento, char* style);

bool svgObject_comparadorID(void* objeto, void* argumento);

Classe svgObject_getClasse(svgObject x);

void svgObject_setClasse(svgObject x, Classe c);

#endif