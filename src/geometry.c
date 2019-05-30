#include "header/geometry.h"

/*
    Molda uma elipse em volta do retângulo
*/
Ellipse moldarRetangulo(Rectangle rec){
    //Obtem o centro de massa
    Point centro = obterCentroDeMassa(RECTANGLE, rec);

    double centroX = Point_getX(centro);
    double centroY = Point_getY(centro);
    double largura = Rectangle_getLargura(rec);
    double altura = Rectangle_getAltura(rec);

    //Cria uma ellipse no centro do retângulo com os raios com metade da altura/largura
    Ellipse el = Ellipse_new(centroX, centroY, largura/2, altura/2);
    
    Point_liberar(centro);
    return el;
}

/*
    Obtem o centro de massa de um Circulo ou Retangulo
*/
Point obterCentroDeMassa(geometryType tipo, void* obj){
    switch(tipo){
        case CIRCLE:
            {
                Circle c1 = obj;
                Point cord = Circle_getCoordenada(c1);
                double x = Point_getX(cord);
                double y = Point_getY(cord);

                return Point_new(x,y);
                break;
            }
        
        case RECTANGLE:
            {
                Rectangle r1 = obj;
                Point cord = Rectangle_getCoordenada(r1);
                double x = Point_getX(cord);
                double y = Point_getY(cord);
                double largura = Rectangle_getLargura(r1);
                double altura = Rectangle_getAltura(r1);

                Point p1 = Point_new(x+ (largura/2), y + (altura/2));
                return p1;
                break;
            }

        default:
            reportError(__func__, "Tipo não tratado");
            break;
    }

    return NULL;
}

/*
    Distância euclidiana entre 2 pontos √((x1-x2)² + (y1-y2)²)
*/
double distanciaEntrePontos(Point p1, Point p2){

    double x1 = Point_getX(p1);
    double y1 = Point_getY(p1);
    double x2 = Point_getX(p2);
    double y2 = Point_getY(p2);

    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}

/*
    Bonding Box - Circle
    Retorna um Retângulo que envolve um círculo
*/
Rectangle bbCircle(Circle c1){
    double x = -1, y = -1, w = -1, h = -1;

    Point cord = Circle_getCoordenada(c1);
    double cx = Point_getX(cord);
    double cy = Point_getY(cord);
    double raio = Circle_getRaio(c1);

    x = cx - raio;
    y = cy - raio;
    w = raio * 2;
    h = raio * 2;


    if(x == -1 || y == -1 || w == -1 || h == -1)
        reportError(__func__, "Valor do círculo inválido");

    Rectangle a = Rectangle_new(x,y,h,w);
    return a;

}

/*
    Chamada padrão do Bonding Box, automaticamente chama os metódos de acordo com os tipos de objeto
*/
Rectangle envolveObjeto(svgObject obj1, svgObject obj2){
    Rectangle r1, r2;
    bool createdR1 = false, createdR2 = false;

    //Cria bondings box entre Retangulos e Circulos. No caso de circulos, ele os transforma em retângulos primeiro

    void* elemento1 = svgObject_getElemento(obj1);
    void* elemento2 = svgObject_getElemento(obj2);

    if(svgObject_getTipo(obj1) == CIRCLE){
        r1 = bbCircle(elemento1);
        createdR1 = true;
    }else{
        r1 = elemento1;
    }

    if(svgObject_getTipo(obj2) == CIRCLE){
        r2 = bbCircle(elemento2);
        createdR2 = true;
    }else{
        r2 = elemento2;
    }

    Rectangle resp = bbRectangle(r1, r2);

    if(createdR1){
        Rectangle_liberar(r1);
    }

    if(createdR2){
        Rectangle_liberar(r2);
    }

    return resp;


}

/*
    Retorna um retângulo que é um bonding box entre outros dois retângulos
*/
Rectangle bbRectangle(Rectangle rect1, Rectangle rect2){
    Point cord1 = Rectangle_getCoordenada(rect1);
    Point cord2 = Rectangle_getCoordenada(rect2);

    double x1 = Point_getX(cord1);
    double y1 = Point_getY(cord1);
    double l1 = Rectangle_getLargura(rect1);
    double a1 = Rectangle_getAltura(rect1);

    double x2 = Point_getX(cord2);
    double y2 = Point_getY(cord2);
    double l2 = Rectangle_getLargura(rect2);
    double a2 = Rectangle_getAltura(rect2);

    double maxX = fmax(x1, fmax(x2, fmax(x1 + l1, x2 + l2)));
    double maxY = fmax(y1, fmax(y2, fmax(y1 + a1, y2 + a2)));
    double minX = fmin(x1, fmin(x2, fmin(x1 + l1, x2 + l2)));
    double minY = fmin(y1, fmin(y2, fmin(y1 + a1, y2 + a2)));


    return Rectangle_new(minX, minY, maxY - minY, maxX - minX);
}

/*
    Verifica a colisão entre 2 retângulos
*/
bool colideRetanguloRetangulo(Rectangle rect1, Rectangle rect2){
    bool colide = false;
    
    bool colideX = false;
    bool colideY = false; 

    Point cord1 = Rectangle_getCoordenada(rect1);
    Point cord2 = Rectangle_getCoordenada(rect2);

    double x1 = Point_getX(cord1);
    double y1 = Point_getY(cord1);
    double l1 = Rectangle_getLargura(rect1);
    double a1 = Rectangle_getAltura(rect1);

    double x2 = Point_getX(cord2);
    double y2 = Point_getY(cord2);
    double l2 = Rectangle_getLargura(rect2);
    double a2 = Rectangle_getAltura(rect2);

    //Verificar se o X e o X + width do rect1 está entre o X e o X+width do rect2
    if(contidoNoIntervalo(x1, x2, x2 + l2, true) ||
       contidoNoIntervalo(x1 + l1, x2, x2 + l2, true)){
           colideX = true;
    }

    if(contidoNoIntervalo(y1, y2, y2 + a2, true) ||
       contidoNoIntervalo(y1 + a1, y2, y2 + a2, true)){
           colideY = true;
    }
         
    return colideX && colideY;
}

/*
    Verifica a colisão entre um retângulo e um círculo
*/
bool colideCirculoRetangulo(Rectangle rect, Circle circle){
    Point cordCirc = Circle_getCoordenada(circle);
    Point cordRect = Rectangle_getCoordenada(rect);

    double xc = Point_getX(cordCirc);
    double yc = Point_getY(cordCirc);
    double r = Circle_getRaio(circle);

    double xr = Point_getX(cordRect);
    double yr = Point_getY(cordRect);
    double l = Rectangle_getLargura(rect);
    double a = Rectangle_getAltura(rect);

    Point pontoMaisProximo = Point_new(clamp(xc, xr , xr + l),
                                         clamp(yc, yr, yr + a));

    double dist = distanciaEntrePontos(pontoMaisProximo, cordCirc);
    bool resp = false;

    if(dist <= r){
        resp =  true;
    }

    Point_liberar(pontoMaisProximo);
    return resp;

}

double distanciaL2(Point p1, Point p2){
    double disX = Point_getX(p1)-Point_getX(p2);
    double disY = Point_getY(p1)-Point_getY(p2);

    if(disX < 0)
        disX *= -1;
    
    if(disY < 0)
        disY *= -1;

    return disX + disY;
}

double maiorDistancia(Rectangle r, Point p, bool l1){
    Point rec = Rectangle_getCoordenada(r);
    double x = Point_getX(rec);
    double y = Point_getY(rec);
    double a = Rectangle_getAltura(r);
    double l = Rectangle_getLargura(r);

    Point bordas[4];
    bordas[0] = Point_new(x,y);
    bordas[1] = Point_new(x, y+a);
    bordas[2] = Point_new(x+l, y);
    bordas[3] = Point_new(x+l, y+a);

    double maior = 0;
    for(int i = 0; i < 4; i++){
        
        double dd;
        if(!l1){
            dd = distanciaEntrePontos(bordas[i], p);
        }else{ 
            dd = distanciaL2(bordas[i], p);
        }
        if(dd > maior){
            maior = dd;
        }
    }

    return maior;

}

bool isContido(svgObject contido, Rectangle box){
    Point ret = Rectangle_getCoordenada(box);
    double rx = Point_getX(ret);
    double ry = Point_getY(ret);
    double ra = Rectangle_getAltura(box);
    double rl = Rectangle_getLargura(box);

    Rectangle bd;

    if(svgObject_getTipo(contido) == CIRCLE){
        Circle c = svgObject_getElemento(contido);
        bd = bbCircle(c);

    }else if(svgObject_getTipo(contido) == RECTANGLE){
        bd = svgObject_getElemento(contido);

    }else{
        reportError(__func__, "Tipo não encaixado");
        printf("TIPO:%d\n", svgObject_getTipo(contido));
    }

    Point ret2 = Rectangle_getCoordenada(bd);
    double bx = Point_getX(ret2);
    double by = Point_getY(ret2);
    double ba = Rectangle_getAltura(bd);
    double bl = Rectangle_getLargura(bd);

    if(contidoNoIntervalo(bx, rx, rx+rl, true) && contidoNoIntervalo(by, ry, ry+ra, true) &&
        contidoNoIntervalo(bx+bl, rx, rx+rl, true) && contidoNoIntervalo(by+ba, ry, ry+ra, true)){
            return true;
        }else{
            return false;
        }



}

/*
    Verifica a colisão entre 2 círculos
*/
bool colideCirculoCirculo(Circle cir1, Circle cir2){

    double dist = distanciaEntrePontos(Circle_getCoordenada(cir1), Circle_getCoordenada(cir2));

    double r1 = Circle_getRaio(cir1);
    double r2 = Circle_getRaio(cir2);

    if(dist <= r1+r2){
        return true;
    }else{
        return false;
    }
}

/*
    Função geral para verificação de colisão
*/
bool colide(svgObject objeto1, svgObject objeto2){
    switch(svgObject_getTipo(objeto1)){

        //Verificação de CIRCULOS com X
        case CIRCLE:
            {

            Circle c1 = svgObject_getElemento(objeto1);

            switch(svgObject_getTipo(objeto2)){
                //Circulo com Circulo;
                case CIRCLE:
                    {
                        Circle c2 = svgObject_getElemento(objeto2);
                        return colideCirculoCirculo(c1, c2);
                        break;
                    }

                //Circulo com Retangulo
                case RECTANGLE:
                    {
                        Rectangle r2 = svgObject_getElemento(objeto2);
                        return colideCirculoRetangulo(r2, c1);
                        break;
                    }

                    break;

                default:
                    reportError(__func__, "Tipo não encontrado");
                    break;
            }

            break;
        


            }
        //Verificação de tipos de rectangulo com X
        case RECTANGLE: 
            {
            
            Rectangle r1 = svgObject_getElemento(objeto1);

            switch(svgObject_getTipo(objeto2)){
                //Retangulo com Circulo --> Caso repetido do primeiro switch: Chama a função de novo com a ordem invertida
                case CIRCLE:
                    {
                        return colide(objeto2, objeto1);
                        break;
                    }
                //Retangulo com Retangulo
                case RECTANGLE:
                    {
                        Rectangle r2 = svgObject_getElemento(objeto2);
                        return colideRetanguloRetangulo(r1, r2);
                        break;
                    }
                default:
                    reportError(__func__, "Tipo não encontrado");
                    break;
            }

            break;
            }

        default:
            reportError(__func__, "Tipo não encontrado");
            break;
    }

}

/*
    Retorna o valor máximo/mínimo possível dentro do intervalo
*/
double clamp(double valor, double intervalo1, double intervalo2){

    double menor, maior;
    if(intervalo1 >= intervalo2){
        maior = intervalo1;
        menor = intervalo2;
    }else{
        maior = intervalo2;
        menor = intervalo1;
    }

    if(valor >= maior){
        return maior;
    }else if(valor <= menor){
        return menor;
    }else{
        return valor;
    }
}

/* 
    Verifica se o valor está dentro do intervalo, possui a opção de incluir a borda ou não
*/
bool contidoNoIntervalo(double valor, double intervalo1, double intervalo2, bool incluirBorda){
    
    double menor, maior;
    if(intervalo1 >= intervalo2){
        maior = intervalo1;
        menor = intervalo2;
    }else{
        maior = intervalo2;
        menor = intervalo1;
    }

    if(incluirBorda){
        if(valor <= maior && valor >= menor){
            return true;
        }else{
            return false;
        }
    }else{
        if(valor < maior && valor > menor){
            return true;
        }else{
            return false;
        }
    }

}


