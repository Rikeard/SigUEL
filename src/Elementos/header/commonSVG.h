#ifndef COMMONSVG_H
#define COMMONSVG_H

#include "../SVG/header/Point.h"
#include "../SVG/header/Circle.h"
#include "../SVG/header/Rectangle.h"
#include "../SVG/header/Ellipse.h"
#include "../SVG/header/Line.h"
#include "../SVG/header/Text.h"

typedef enum geometryType{
    CIRCLE, RECTANGLE, TEXT, LINE, ELLIPSE, POINT
} geometryType;

typedef enum Classe{
    DEFAULT, QUADRA, HIDRANTE, SEMAFORO, RADIO
} Classe;

void SVG_Liberar(geometryType x, void* elemento);

#endif