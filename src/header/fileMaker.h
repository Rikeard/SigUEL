#ifndef FILEMAKER_H
#define FILEMAKER_H

#include "main.h"
#include "../Elementos/header/svgObject.h"
#include "../Elementos/SVG/header/Text.h"
#include "../Elementos/SVG/header/Rectangle.h"
#include "../Elementos/SVG/header/Circle.h"
#include "../Elementos/SVG/header/Ellipse.h"
#include "../Elementos/SVG/header/Line.h"
#include "../Elementos/header/commonSVG.h"
#include "fileHandler.h"

/*
    Inicia e fecha os arquivos SVG, colocando suas tags.
    Também processa todo procedimento de adição de elementos ao SVG ao receber um svgObject
*/

void iniciarSVGFILE(char* path);
void adicionarSVGFile(svgObject a, FILE* f);
void finalizarSVGFILE(char* path);

#endif