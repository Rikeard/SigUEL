#ifndef GEO_H
#define GEO_H

#include "main.h"
#include "../Elementos/header/commonSVG.h"
#include "../Elementos/header/svgObject.h"
#include "../Elementos/SVG/header/Rectangle.h"
#include "../Elementos/SVG/header/Point.h"
#include "../Elementos/SVG/header/Circle.h"
#include "../Elementos/SVG/header/Ellipse.h"


/*
    Molda uma elipse em volta do retângulo
*/
Ellipse moldarRetangulo(Rectangle rec);

/*
    Obtem o centro de massa de um Circulo ou Retangulo
*/
Point obterCentroDeMassa(geometryType tipo, void* obj);

/*
    Distância euclidiana entre 2 pontos √((x1-x2)² + (y1-y2)²)
*/
double distanciaEntrePontos(Point p1, Point p2);

/*
    Bonding Box - Circle
    Retorna um Retângulo que envolve um círculo
*/
Rectangle bbCircle(Circle c1);

/*
    Chamada padrão do Bonding Box, automaticamente chama os metódos de acordo com os tipos de objeto
*/
Rectangle envolveObjeto(svgObject obj1, svgObject obj2);

/*
    Retorna um retângulo que é um bonding box entre outros dois retângulos
*/
Rectangle bbRectangle(Rectangle rect1, Rectangle rect2);

/*
    Verifica a colisão entre 2 retângulos
*/
bool colideRetanguloRetangulo(Rectangle rect1, Rectangle rect2);

/*
    Verifica a colisão entre um retângulo e um círculo
*/
bool colideCirculoRetangulo(Rectangle rect, Circle circle);

/*
    Verifica a colisão entre 2 círculos
*/
bool colideCirculoCirculo(Circle cir1, Circle cir2);

/*
    Função geral para verificação de colisão
*/
bool colide(svgObject objeto1, svgObject objeto2);

/*
    Retorna o valor máximo/mínimo possível dentro do intervalo
*/
double clamp(double valor, double intervalo1, double intervalo2);

/*
    Verifica se o valor está dentro do intervalo, possui a opção de incluir a borda ou não
*/
bool contidoNoIntervalo(double valor, double intervalo1, double intervalo2, bool incluirBorda);
/*
    Distância entre a borda mais distante do retângulo e o ponto
*/
double maiorDistancia(Rectangle r, Point p, bool l1); 

bool isContido(svgObject contido, Rectangle box);






#endif
