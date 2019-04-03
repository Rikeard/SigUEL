#ifndef SVG_H
#define SVG_H

#include "main.h"
#include "geometry.h"


//Struct que contém todo o SVG de um detemrinado caso
typedef struct SVG{
    int width, height;
    treeNode *inicio;
    char *data;
    char *header;
    char *ending;
} SVG;

SVG* inicializarContainerSVG(int width, int height);
char* exportarSVG(SVG *objeto);
svgObject* criarSVGObject(geometryType tipo, int id, void* elemento, char* corBorda, char* CorPreenchimento, char* style);


#endif