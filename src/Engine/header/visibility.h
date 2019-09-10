#ifndef VISIBILITY_H
#define VISIBILITY_H


#define M_PI 3.14159265358979323846

#include <stdbool.h>
#include "../../Elementos/SVG/header/Point.h"
#include "../../Commons/header/listaEncadeada.h"
#include "../../Elementos/SVG/header/Rectangle.h"
#include "../../Elementos/SVG/header/Line.h"
#include "../../Elementos/Urbano/header/Muro.h"
#include "../../Elementos/Urbano/header/Predio.h"
#include "../../Elementos/header/svgObject.h"
#include "../../Elementos/SVG/header/Triangle.h"
#include <math.h>

/*
    Processa a visibilidade, recebendo a lista de muros e de prédios, o x e y da bomba e retornando uma lista de triângulos para serem printados no SVG
*/

listaStatic processarVisibilidade(double x, double y, listaStatic muros, listaStatic predios);

#endif