#include "header/geometry.h"
#include "header/free.h"

void* criarTexto(double x, double y, char* texto){
    Text *t= malloc(sizeof(Text));

    t->coordenada = criarPonto(x,y);
    
    if(texto == NULL){
        reportError(__func__, "Texto é nulo");
        free(t);
        return NULL;
    }else{
        t->texto = texto;
    }
    return t;
}

void *criarLinha(double x1, double y1, double x2, double y2){
    Line *l = malloc(sizeof(Line));
    l->coordenada1 = criarPonto(x1, y1);
    l->coordenada2 = criarPonto(x2, y2);
    return l;
}

Ellipse* moldarRetangulo(svgObject *rec){
    Point *x = obterCentroDeMassa(rec);
    Rectangle *rect1 = rec->elemento;
    Ellipse *el =  criarElipse(x->x, x->y, rect1->width/2, rect1->height/2);
    liberarPoint(x);
    return el;
}

void *criarElipse(double x, double y, double raioX, double raioY){
    Ellipse *el = malloc(sizeof(Ellipse));
    
    el->raioX = raioX;
    el->raioY = raioY;
    el->coordenada = criarPonto(x,y);
    return el;
}

Point* obterCentroDeMassa(svgObject *obj){
    switch(obj->tipo){

        case CIRCLE:
            {
                Circle *c1 = obj->elemento;
                return criarPonto(c1->coordenada->x, c1->coordenada->y);
                break;
            }
        
        case RECTANGLE:
            {
                Rectangle *r1 = obj->elemento;
                Point *p1 = criarPonto(r1->coordenada->x + (r1->width/2), r1->coordenada->y + (r1->height/2));
                return p1;
                break;
            }

        default:
            reportError(__func__, "Tipo não tratado");
            break;
    }

    return NULL;
}

Point* criarPonto(double x, double y){
    Point *p = malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    return p;
}

void* criarRetangulo(double x, double y, double width, double height){
    Rectangle *rect = malloc(sizeof(Rectangle));

    rect->coordenada = criarPonto(x,y);
    rect->width = width;
    rect->height = height;

    return rect;
}

void* criarCirculo(double x, double y, double raio){
    Circle *circl = malloc(sizeof(Circle));

    circl->coordenada = criarPonto(x,y);
    circl->raio = raio;
    return circl;
}

double distanciaEntrePontos(Point* p1, Point* p2){
    return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
}


Rectangle* boundingBoxCIRCLE(Circle *c1){
    double x = -1, y = -1, w = -1, h = -1;

    x = c1->coordenada->x - c1->raio;
    y = c1->coordenada->y - c1->raio;
    w = c1->raio*2;
    h = c1->raio*2;


    if(x == -1 || y == -1 || w == -1 || h == -1)
        reportError(__func__, "Valor do círculo inválido");

    Rectangle *a = criarRetangulo(x,y,w,h);
    return a;

}

Rectangle* envolveObjeto(svgObject *obj1, svgObject *obj2){
    Rectangle *r1, *r2;
    bool createdR1 = false, createdR2 = false;

    if(obj1->tipo == CIRCLE){
        r1 = boundingBoxCIRCLE(obj1->elemento);
        createdR1 = true;
    }else{
        r1 = obj1->elemento;
    }

    if(obj2->tipo == CIRCLE){
        r2 = boundingBoxCIRCLE(obj2->elemento);
        createdR2 = true;
    }else{
        r2 = obj2->elemento;
    }

    Rectangle* resp =  boundingBoxRECTANGLE(r1, r2);

    if(createdR1){
        liberarRectangle(r1);
    }

    if(createdR2){
        liberarRectangle(r2);
    }


    return resp;


}

Rectangle* boundingBoxRECTANGLE(Rectangle *rect1, Rectangle *rect2){
    double maxX = fmax(rect1->coordenada->x, fmax(rect2->coordenada->x, fmax(rect1->coordenada->x + rect1->width, rect2->coordenada->x + rect2->width)));
    double maxY = fmax(rect1->coordenada->y, fmax(rect2->coordenada->y, fmax(rect1->coordenada->y + rect1->height, rect2->coordenada->y + rect2->height)));
    double minX = fmin(rect1->coordenada->x, fmin(rect2->coordenada->x, fmin(rect1->coordenada->x + rect1->width, rect2->coordenada->x + rect2->width)));
    double minY = fmin(rect1->coordenada->y, fmin(rect2->coordenada->y, fmin(rect1->coordenada->y + rect1->height, rect2->coordenada->y + rect2->height)));

    return criarRetangulo(minX, minY, maxX-minX, maxY-minY);
}

bool colideRetanguloRetangulo(Rectangle* rect1, Rectangle* rect2){
    bool colide = false;
    

    bool colideX = false;
    bool colideY = false; 
    //Verificar se o X e o X + width do rect1 está entre o X e o X+width do rect2
    if(contidoNoIntervalo(rect1->coordenada->x, rect2->coordenada->x, rect2->coordenada->x + rect2->width, true) ||
       contidoNoIntervalo(rect1->coordenada->x + rect1->width, rect2->coordenada->x, rect2->coordenada->x + rect2->width, true)){
           colideX = true;
    }

    if(contidoNoIntervalo(rect1->coordenada->y, rect2->coordenada->y, rect2->coordenada->y + rect2->height, true) ||
       contidoNoIntervalo(rect1->coordenada->y + rect1->height, rect2->coordenada->y, rect2->coordenada->y + rect2->height, true)){
           colideY = true;
    }
         
    return colideX && colideY;
}

bool colideCirculoRetangulo(Rectangle* rect, Circle* circle){

    Point *pontoMaisProximo = criarPonto(clamp(circle->coordenada->x, rect->coordenada->x , rect->coordenada->x + rect->width),
                                         clamp(circle->coordenada->y, rect->coordenada->y, rect->coordenada->y + rect->height));

    double dist = distanciaEntrePontos(pontoMaisProximo, circle->coordenada);
    bool resp = false;

    if(dist <= circle->raio){
        resp =  true;
    }

    liberarPoint(pontoMaisProximo);
    return resp;

}

bool colideCirculoCirculo(Circle *cir1, Circle *cir2){

    double dist = distanciaEntrePontos(cir1->coordenada, cir2->coordenada);

    if(dist <= cir1->raio + cir2->raio){
        return true;
    }else{
        return false;
    }
}

bool colide(svgObject* objeto1, svgObject* objeto2){
    switch(objeto1->tipo){

        //Verificação de CIRCULOS com X
        case CIRCLE:
            {

            Circle *c1 = objeto1->elemento;

            switch(objeto2->tipo){
                //Circulo com Circulo;
                case CIRCLE:
                    {
                        Circle *c2 = objeto2->elemento;
                        return colideCirculoCirculo(c1, c2);
                        break;
                    }

                //Circulo com Retangulo
                case RECTANGLE:
                    {
                        Rectangle *r2 = objeto2->elemento;
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
            
            Rectangle *r1 = objeto1->elemento;

            switch(objeto2->tipo){
                //Retangulo com Circulo --> Caso repetido do primeiro switch: Chama a função de novo com a ordem invertida
                case CIRCLE:
                    {
                        return colide(objeto2, objeto1);
                        break;
                    }
                //Retangulo com Retangulo
                case RECTANGLE:
                    {
                        Rectangle *r2 = objeto2->elemento;
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


