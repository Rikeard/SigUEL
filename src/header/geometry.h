#ifndef GEO_H
#define GEO_H

#include "main.h"

typedef enum geometryType{
    CIRCLE, RECTANGLE, TEXT, LINE, ELLIPSE
} geometryType;

//Elementos de gerais de cada objeto do SVG
typedef struct svgObject{
    geometryType tipo;
    int id;
    void *elemento; //Usar o geometryType pra saber qual tipo instanciar
    char* corBorda;
    char* corPreenchimento;
    char* style;
}svgObject;

typedef struct Point{
    double x, y;
} Point;

typedef struct Line{
    Point *coordenada1, *coordenada2;
} Line;

typedef struct Ellipse{
    Point* coordenada;
    double raioX, raioY;
} Ellipse;

typedef struct Text{
    Point* coordenada;
    char* texto;
}Text;

//Elemento de lógica
typedef struct Rectangle{
    Point* coordenada;
    double height, width;
}Rectangle;

typedef struct Circle{
    Point* coordenada;
    double raio;
}Circle;


double clamp(double valor, double intervalo1, double intervalo2);
void* criarRetangulo(double x, double y, double width, double height);
bool colide(svgObject* objeto1, svgObject* objeto2);
void* criarCirculo(double x, double y, double raio);
bool colideRetanguloRetangulo(Rectangle* rect1, Rectangle* rect2);
Ellipse* moldarRetangulo(svgObject *rec);
double distanciaEntrePontos(Point* p1, Point* p2);
bool contidoNoIntervalo(double valor, double intervalo1, double intervalo2, bool incluirBorda);
Point* obterCentroDeMassa(svgObject *obj);
void *criarElipse(double x, double y, double raioX, double raioY);
Rectangle* boundingBoxCIRCLE(Circle *c1);
Rectangle* boundingBoxRECTANGLE(Rectangle *rect1, Rectangle *rect2);
void* criarTexto(double x, double y, char* texto);
Rectangle* envolveObjeto(svgObject *obj1, svgObject *obj2);
Point* criarPonto(double x, double y);
void *criarLinha(double x1, double y1, double x2, double y2);



#endif
