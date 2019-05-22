#include "header/Line.h"

typedef struct Linha{
    Point *coordenada1, *coordenada2;
} Linha;

Line Line_newXY(double x1, double y1, double x2, double y2){
    Linha *ln = malloc(sizeof(Linha));
    ln->coordenada1 = Point_new(x1, y1);
    ln->coordenada2 = Point_new(x2, y2);
    return ln;
}

Line Line_newCord(Point* coord1, Point* coord2){
    Linha *ln = malloc(sizeof(Linha));
    ln->coordenada1 = coord1;
    ln->coordenada2 = coord2;
    return ln;
}

Point Line_getCoordenada1(Line* line){
    Linha* ln = line;
    return ln->coordenada1;
}

Point Line_getCoordenada2(Line* line){
    Linha* ln = line;
    return ln->coordenada2;
}

void Line_liberar(Line* line){
    Linha* ln = line;
    Point_liberar(ln->coordenada1);
    Point_liberar(ln->coordenada2);
    free(ln);
}
