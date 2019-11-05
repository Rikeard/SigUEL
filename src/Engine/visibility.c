#include "header/visibility.h"

//TIPOS E CONSTRUTORES

typedef void* Segmento;
typedef void* Endpoint;
typedef void* Triangulo;

typedef struct Endpoint_t{
    double x, y;
    bool comeco;
    Segmento segmento;
    double angulo;
}Endpoint_t;

typedef struct Ponto{
    double x, y;
}Ponto;

typedef struct Segmento_t{
    Endpoint p1, p2;
}Segmento_t;

typedef struct Lista_t{
    void** lista;
    int tamanho, emUso;
}Lista_t;

typedef struct infiniteList_t{
    struct infiniteList_t* proximoNode;
    struct infiniteList_t* anteriorNode;
    void* elemento;
}infiniteList_t;

//DECLARADORES


Lista_t* Lista_new(int tamanho);
Segmento Segmento_new(double x1, double y1, double x2, double y2);
void Endpoint_setSegment(Endpoint e, Segmento s);
Endpoint Endpoint_new(double x1, double y1);
infiniteList_t* calcularVisibilidade(Ponto lightSource, Lista_t* endpoints);
void adicionarNaLista(Lista_t* lista, void* elemento);
int endpointComparator(const void* p1, const void* p2);
Lista_t* carregarMapa(Ponto lightSource, listaStatic muros, listaStatic predios);
void obterEndpoints(void* elemento, void* argumento);
void converterMuros(void* elemento, void* argumento);
void converterPredios(void* elemento, void* argumento);
void processarSegmentos(Ponto lightSource, Lista_t* segmentos);
void calcularAnguloEndpoint(Segmento seg, Ponto lightSource);
void calcularComecoSegmento(Segmento seg);
Triangulo obterPontosDoTriangulo(Ponto lightSource, double angulo1, double angulo2, Segmento segment);
void linhaParaSegmento(Line l, Lista_t* lista);
void retanguloParaSegmentos(Rectangle r, Lista_t* lista);
void printSegmento(Segmento elemento);
void printEndpoint(Endpoint e);

//FUNCOES

infiniteList_t* adicionarElemento(infiniteList_t* lista, void* elemento){

    if(lista == NULL){
        lista = malloc(sizeof(infiniteList_t));

        lista->anteriorNode = NULL;
        lista->proximoNode = NULL;
        lista->elemento = elemento;
        return lista;
    }else{
        infiniteList_t* aux, *novo;
        aux = lista;
        
        while(aux->proximoNode != NULL){
            aux = aux->proximoNode;
        }

        novo = malloc(sizeof(infiniteList_t));
        novo->anteriorNode = aux;
        novo->proximoNode = NULL;
        novo->elemento = elemento;
        aux->proximoNode = novo;
        return lista;
    }

}

infiniteList_t* adicionarElementoAfter(infiniteList_t* lista, int x, void* elemento){
        infiniteList_t* aux, *novo;
        aux = lista;
        int index = 0;

        if(x == 0){
            novo = malloc(sizeof(infiniteList_t));
            novo->anteriorNode = NULL;
            novo->proximoNode = lista;
            novo->elemento = elemento;
            lista->anteriorNode = novo;
            return novo;
        }

        while(true){

            if(index == x-1){
                break;
            }

            if(aux->proximoNode == NULL){
                printf("Solicitado %d, Atual %d\n", x, index);
                reportError(__func__, "Shit");
                break;
            }
            
            aux = aux->proximoNode;
            index++;
        }

        novo = malloc(sizeof(infiniteList_t));
        novo->anteriorNode = aux;
        novo->proximoNode = aux->proximoNode;
        novo->elemento = elemento;
        aux->proximoNode->anteriorNode = novo;
        aux->proximoNode = novo;
        return lista;

}

void* obterElemento(infiniteList_t* lista, int pos){
    for(int i = 0; i < pos; i++){
        if(lista != NULL){
            lista = lista->proximoNode;
        }else{
            return NULL;
        }
    }
    if(lista != NULL)
        return lista->elemento;
    else
        return NULL;
}

infiniteList_t* removerIndex(infiniteList_t* lista, int x){
    int index = 0;
    infiniteList_t* start = lista;

    //printf("Solicitado pra remover em %d\n", x);
    if(lista == NULL){
        reportError(__func__, "Shit");
    }

    do{
        if(index == x){
            break;
        }

        if(lista->proximoNode == NULL){
            reportError(__func__, "Shit");
            break;
        }
        
        lista = lista->proximoNode;
        index++;
    }while(true);

    //printf("Após loop, estados %d %d %d\n", lista->anteriorNode, lista, lista->proximoNode);

    if(lista->anteriorNode == NULL && lista->proximoNode == NULL){
        return NULL;
    }


    //Primeiro
    if(lista->anteriorNode == NULL){
        lista->proximoNode->anteriorNode = NULL;
        return lista->proximoNode;
    }

    if(lista->proximoNode == NULL){
        lista->anteriorNode->proximoNode = NULL;
        return start;
    }

    //Meio
    lista->anteriorNode->proximoNode = lista->proximoNode;
    lista->proximoNode->anteriorNode = lista->anteriorNode;
    return start;
}

int indexOfElemento(infiniteList_t* lista, void* elemento){
    int index = 0;

    if(lista == NULL){
        return -1;
    }

    do{
        if(lista->elemento == elemento){
            return index;
        }

        index++;
        lista = lista->proximoNode;
    }while(lista != NULL);
    //printf("Não encotrou não tio\n");
    return -1;

}

Lista_t* Lista_new(int tamanho){
    Lista_t* ls = malloc(sizeof(Lista_t));
    ls->lista = malloc(tamanho * sizeof(void*));
    ls->tamanho = tamanho;
    ls->emUso = 0;
    return ls;
}

Segmento Segmento_new(double x1, double y1, double x2, double y2){
    Segmento sg = malloc(sizeof(Segmento_t));
    Segmento_t* s = (Segmento_t*) sg;

    s->p1 = Endpoint_new(x1, y1);
    s->p2 = Endpoint_new(x2, y2);

    Endpoint_setSegment(s->p1, sg);
    Endpoint_setSegment(s->p2, sg);

    return sg;    
}

void Endpoint_setSegment(Endpoint e, Segmento s){
    Endpoint_t* et = (Endpoint_t*) e;
    et->segmento = s;
}

Endpoint Endpoint_new(double x1, double y1){
    Endpoint ed = malloc(sizeof(Endpoint_t));
    Endpoint_t* e = (Endpoint_t*) ed;

    e->x = x1;
    e->y = y1;
    e->comeco = false;
    e->segmento = NULL;
    e->angulo = 0;
    return ed;
}

void adicionarNaLista(Lista_t* lista, void* elemento){
    if(lista->emUso+1 > lista->tamanho){
        printf("Lista lotada\n");
        return;
    }
    lista->lista[lista->emUso] = elemento;
    lista->emUso += 1;
}

int tamanhoLista(infiniteList_t* lista){
    int index = 0;
    while(lista != NULL){
        index++;
        lista = lista->proximoNode;
    }
    return index;
}

void debugLista(infiniteList_t* lista){
    int index = 0;
    while(lista != NULL){
        Segmento_t* s = (Segmento_t*) lista->elemento;
        //printf("\n\nLISTA: ");
        //printSegmento(s);
        lista = lista->proximoNode;
        index++;
    }
    printf("Tamanho da lista: %d\n\n", index);
}

//FUNCOES


listaStatic processarVisibilidade(double x, double y, listaStatic muros, listaStatic predios){
    Ponto lightSource = {x,y};
    Lista_t* endpoints = carregarMapa(lightSource, muros, predios);
    infiniteList_t* resultado = calcularVisibilidade(lightSource, endpoints);
    listaStatic resp = listaStatic_new(tamanhoLista(resultado));

    while(resultado != NULL){
        listaStatic_add(resp, resultado->elemento);
        resultado = resultado->proximoNode;
    }

    return resp;
}

//visibility.js

bool naEsquerda(Segmento a, Ponto p){
    Segmento_t* segmento = (Segmento_t*) a;
    Endpoint_t* p1 = (Endpoint_t*) segmento->p1;
    Endpoint_t* p2 = (Endpoint_t*) segmento->p2;

    double cross = (p2->x - p1->x) * (p.y - p1->y) - (p2->y - p1->y) * (p.x - p1->x);
    if(cross < 0){
        return true;
    }else{
        return false;
    }
}


Ponto interpolar(Endpoint p1, Endpoint p2, double f){
    Endpoint_t* a = (Endpoint_t*) p1;
    Endpoint_t* b = (Endpoint_t*) p2;
    
    Ponto res = {a->x * (1-f) + b->x*f, a->y*(1-f) + b->y*f};
/*     printf("X %lf Y %lf\n", res.x, res.y); */
    return res;
}

bool segmentoNaFrente(Segmento a, Segmento b, Ponto pontoRelativo){
/*     printf("INPUT: \n");
    printSegmento(a);
    printSegmento(b);  */
    Segmento_t* segmentoA = (Segmento_t*) a;
    Endpoint_t* Ap1 = (Endpoint_t*) segmentoA->p1;
    Endpoint_t* Ap2 = (Endpoint_t*) segmentoA->p2;

    Segmento_t* segmentoB = (Segmento_t*) b;
    Endpoint_t* Bp1 = (Endpoint_t*) segmentoB->p1;
    Endpoint_t* Bp2 = (Endpoint_t*) segmentoB->p2;


    bool A1 = naEsquerda(a, interpolar(Bp1, Bp2, 0.01));
    bool A2 = naEsquerda(a, interpolar(Bp2, Bp1, 0.01));
    bool A3 = naEsquerda(a, pontoRelativo);
    bool B1 = naEsquerda(b, interpolar(Ap1, Ap2, 0.01));
    bool B2 = naEsquerda(b, interpolar(Ap2, Ap1, 0.01));
    bool B3 = naEsquerda(b, pontoRelativo);
/* 
    printf("%d %d %d %d %d %d\n", A1, A2, A3, B1, B2,B3);  */

    if(B1 == B2 && B2 != B3) return true;
    if(A1 == A2 && A2 == A3) return true;
    if(A1 == A2 && A2 != A3) return false;
    if(B1 == B2 && B2 == B3) return false;

    return false;
}

int endpointComparator(const void* p1, const void* p2){
    Endpoint_t* e1 = (Endpoint_t*) (* (void **) p1);
    Endpoint_t* e2 = (Endpoint_t*) (* (void **) p2);

    if(e1->angulo > e2->angulo)
        return 1;
    
    if (e1->angulo < e2->angulo)
        return -1;

    if(!e1->comeco && e2->comeco)
        return 1;

    if(e1->comeco && !e2->comeco)
        return -1;

    return 0;
}



infiniteList_t* calcularVisibilidade(Ponto lightSource, Lista_t* endpoints){
    infiniteList_t* segmentosAbertos = NULL;
    infiniteList_t* saida = NULL;
    double anguloInicial = 0;

    qsort(endpoints->lista, endpoints->emUso, sizeof(void*), endpointComparator);

    for(int x = 0; x < 2; x+= 1){

        for(int i = 0; i < endpoints->emUso; i+= 1){

            Endpoint_t* endpoint = endpoints->lista[i];
            Segmento_t* segmentoAberto = obterElemento(segmentosAbertos, 0);

            if(endpoint->comeco){
                int index = 0;
                Segmento_t* segmento = obterElemento(segmentosAbertos, index);

                while(segmento != NULL && segmentoNaFrente(endpoint->segmento, segmento, lightSource)){
                    index += 1;
                    segmento = obterElemento(segmentosAbertos, index);
                }

                if(segmento == NULL){
                    segmentosAbertos = adicionarElemento(segmentosAbertos, endpoint->segmento);
                }else{
                    segmentosAbertos = adicionarElementoAfter(segmentosAbertos, index, endpoint->segmento);
                }

            }else{
                int index = indexOfElemento(segmentosAbertos, endpoint->segmento);
                if(index > -1) {
                    segmentosAbertos = removerIndex(segmentosAbertos, index);
                }
            }


            if(segmentoAberto != obterElemento(segmentosAbertos, 0)){
                if(x == 1){
                    Triangulo t = obterPontosDoTriangulo(lightSource, anguloInicial, endpoint->angulo, segmentoAberto);
                    saida = adicionarElemento(saida, t);
                }
                anguloInicial = endpoint->angulo;

            }

        }
    }


    return saida;


}

Ponto interseccaoDeLinha(Ponto p1, Ponto p2, Ponto p3, Ponto p4){
    double s = (    (p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)   ) / (   (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y)  );
    Ponto res = {p1.x + s * (p2.x - p1.x), p1.y + s * (p2.y - p1.y)};
    return res;
}

Triangle obterPontosDoTriangulo(Ponto lightSource, double angulo1, double angulo2, Segmento segment){

    Ponto p1 = lightSource;
    Ponto p2 = {lightSource.x + cos(angulo1), lightSource.y + sin(angulo1)};
    Ponto p3 = {0,0};
    Ponto p4 = {0,0};

    if(segment != NULL){
        Segmento_t* s = (Segmento_t*) segment;

        Endpoint_t* end1 = (Endpoint_t*) s->p1;
        Endpoint_t* end2 = (Endpoint_t*) s->p2;

        p3.x = end1->x;
        p3.y = end1->y;
        p4.x = end2->x;
        p4.y = end2->y;
    }else{
        p3.x = lightSource.x + cos(angulo1) * 200;
        p3.y = lightSource.y + sin(angulo1) * 200;
        p4.x = lightSource.x + cos(angulo2) * 200;
        p4.y = lightSource.y + sin(angulo2) * 200;
    }

    Ponto inicio = interseccaoDeLinha(p3, p4, p1, p2);

    p2.x = lightSource.x + cos(angulo2);
    p2.y = lightSource.y + sin(angulo2);

    Ponto final = interseccaoDeLinha(p3, p4, p1, p2);

    Point init = Point_new(inicio.x, inicio.y);
    Point fin = Point_new(final.x, final.y);
    Point light = Point_new(lightSource.x, lightSource.y);

    return Triangle_new(init, fin, light);
}


//loadMap.js

Lista_t* carregarMapa(Ponto lightSource, listaStatic muros, listaStatic predios){
    //recebe todos elementos
    //retornar endpoints
    Lista_t* segmentos = Lista_new((listaStatic_length(predios) * 4) + listaStatic_length(muros) + 4);

    listaStatic_forEach(muros, converterMuros, segmentos);
    listaStatic_forEach(predios, converterPredios, segmentos);

    double xMin  = 0, xMax = 0, yMin = 0, yMax = 0;
    for(int i = 0; i < segmentos->emUso; i++){
        Segmento_t* s = segmentos->lista[i];
        Endpoint_t* x = s->p1;

        if(x->x > xMax){
            xMax = x->x;
        }else if(x->x < xMin){
            xMin = x->x;
        }

        if(x->y > yMax){
            yMax = x->y;
        }else if(x->y < yMin){
            xMin = x->y;
        }

        x = s->p2;


        if(x->x > xMax){
            xMax = x->x;
        }else if(x->x < xMin){
            xMin = x->x;
        }

        if(x->y > yMax){
            yMax = x->y;
        }else if(x->y < yMin){
            xMin = x->y;
        }
    }

    if(lightSource.x > xMax){
        xMax = lightSource.x;
    }else if(lightSource.x < xMin){
        xMin = lightSource.x;
    }

    if(lightSource.y > yMax){
        yMax = lightSource.y;
    }else if(lightSource.y < yMin){
        yMin = lightSource.y;
    }

    double cons = 100;

    xMax += cons; yMax += cons; yMin -= cons; xMin -= cons;

    Segmento t1 =  Segmento_new(xMin, yMin,xMax, yMin);
    Segmento t2 =  Segmento_new(xMin, yMin,xMin, yMax);
    Segmento t3 =  Segmento_new(xMax, yMax,xMax, yMin);
    Segmento t4 =  Segmento_new(xMax, yMax,xMin, yMax);

    adicionarNaLista(segmentos, t1);
    adicionarNaLista(segmentos, t2);
    adicionarNaLista(segmentos, t3);
    adicionarNaLista(segmentos, t4);

    processarSegmentos(lightSource, segmentos);
    Lista_t* endpoints = Lista_new((segmentos->tamanho*2));


    for(int i = 0; i < segmentos->emUso; i++){
        obterEndpoints(segmentos->lista[i], endpoints);
    }

    return endpoints;
}

void obterEndpoints(void* elemento, void* argumento){
    Segmento_t* s = (Segmento_t*) elemento;

    Endpoint_t* p1 = (Endpoint_t*) s->p1;
    Endpoint_t* p2 = (Endpoint_t*) s->p2;
    
    adicionarNaLista(argumento, p1);
    adicionarNaLista(argumento, p2);
}


void converterMuros(void* elemento, void* argumento){
    svgObject s = Muro_getElemento(elemento);
    Line l = svgObject_getElemento(s);
    linhaParaSegmento(l, argumento);
}

void converterPredios(void* elemento, void* argumento){
    svgObject s = Predio_getElemento(elemento);
    Rectangle r = svgObject_getElemento(s);
    retanguloParaSegmentos(r, argumento);
}

void processarSegmentos(Ponto lightSource, Lista_t* segmentos){
    for(int i = 0; i < segmentos->emUso; i++){
        calcularAnguloEndpoint(segmentos->lista[i], lightSource);
        calcularComecoSegmento(segmentos->lista[i]);
    }
}

void calcularAnguloEndpoint(Segmento seg, Ponto lightSource){
    double x = lightSource.x, y = lightSource.y;
    Segmento_t* s = (Segmento_t*) seg;

    Endpoint_t* p1 = (Endpoint_t*) s->p1;
    Endpoint_t* p2 = (Endpoint_t*) s->p2;

    p1->angulo = atan2(p1->y - y, p1->x - x);
    p2->angulo = atan2(p2->y - y, p2->x - x);
/*     printf("Calculando atan2 para %lf %lf = %lf\n", p1->y - y, p1->x - x, p1->angulo);
    printf("Calculando atan2 para %lf %lf = %lf\n", p2->y - y, p2->x - x, p2->angulo); */
}

void calcularComecoSegmento(Segmento seg){
    Segmento_t* s = (Segmento_t*) seg;

    Endpoint_t* p1 = (Endpoint_t*) s->p1;
    Endpoint_t* p2 = (Endpoint_t*) s->p2;

    double dAngle = p2->angulo - p1->angulo;

    if(dAngle <= -M_PI)
        dAngle += 2 * M_PI;
    if(dAngle > M_PI)
        dAngle -= 2 * M_PI;

    p1->comeco = dAngle > 0;
    p2->comeco = !p1->comeco;

}

void linhaParaSegmento(Line l, Lista_t* lista){
    Point l1 = Line_getCoordenada1(l), l2 = Line_getCoordenada2(l);
    adicionarNaLista(lista, Segmento_new(Point_getX(l1), Point_getY(l1), Point_getX(l2), Point_getY(l2)));
}

void retanguloParaSegmentos(Rectangle r, Lista_t* lista){
    Point rec = Rectangle_getCoordenada(r);

    Segmento s1 = Segmento_new(Point_getX(rec), Point_getY(rec), Point_getX(rec) + Rectangle_getLargura(r), Point_getY(rec));
    Segmento s2 = Segmento_new(Point_getX(rec), Point_getY(rec), Point_getX(rec), Point_getY(rec) + Rectangle_getAltura(r));
    Segmento s3 = Segmento_new(Point_getX(rec), Point_getY(rec) + Rectangle_getAltura(r), Point_getX(rec) + Rectangle_getLargura(r), Point_getY(rec) + Rectangle_getAltura(r));
    Segmento s4 = Segmento_new(Point_getX(rec) + Rectangle_getLargura(r), Point_getY(rec) + Rectangle_getAltura(r), Point_getX(rec) + Rectangle_getLargura(r), Point_getY(rec));

    adicionarNaLista(lista, s1);
    adicionarNaLista(lista, s2);
    adicionarNaLista(lista, s3);
    adicionarNaLista(lista, s4);
}

void printSegmento(Segmento elemento){
    Segmento_t* s = (Segmento_t*) elemento;

    Endpoint_t* p1 = (Endpoint_t*) s->p1;
    Endpoint_t* p2 = (Endpoint_t*) s->p2;
    printf("P1: X %lf Y %lf ANGULO %lf COMECO %d\n", p1->x, p1->y, p1->angulo, p1->comeco);
    printf("P2: X %lf Y %lf ANGULO %lf COMECO %d\n", p2->x, p2->y, p2->angulo, p2->comeco);
}

void printEndpoint(Endpoint e){
    Endpoint_t* p1 = (Endpoint_t*) e;
    printf("E: X %lf Y %lf ANGULO %lf COMECO %d\n", p1->x, p1->y, p1->angulo, p1->comeco);
}

/* void printTriangulo(Triangulo t){
    Triangulo_t* tr = (Triangulo_t*) t;
    printf("[%lf, %lf] [%lf, %lf] [%lf, %lf]\n", tr->p1.x, tr->p1.y, tr->p2.x, tr->p2.y, tr->p3.x, tr->p3.y);
} */