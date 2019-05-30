#include "header/queryHandler.h"

void geo_Nx(char* comando, Runtime rt){

    int maxcr = -1;
    int maxq = -1;
    int maxh = -1;
    int maxs = -1;
    int maxr = -1;

    sscanf(comando, "%*s %d %d %d %d %d", &maxcr, &maxq, &maxh, &maxs, &maxr);

    if(maxcr == -1 || maxq == -1 || maxh == -1 || maxs == -1 || maxs == -1 || maxr == -1)
        reportError(__func__, "Valor identificado incorretamente");

    listaStatic_setSize(runTime_getLista(rt, L_GERAL), maxcr);
    listaStatic_setSize(runTime_getLista(rt, L_QUADRAS), maxq);
    listaStatic_setSize(runTime_getLista(rt, L_HIDRANTES), maxh);
    listaStatic_setSize(runTime_getLista(rt, L_SEMAFOROS), maxs);
    listaStatic_setSize(runTime_getLista(rt, L_RADIOS), maxr);

    //Supostamente eu faço algo com isso cofcof
}

void geo_C(char* comando, listaStatic lista, Runtime rt){

    char* id = calloc(64, sizeof(char));
    double r = -1,x = -1,y = -1;
    char *cor1 = calloc(32, sizeof(char));
    char *cor2 = calloc(32, sizeof(char));

    sscanf(comando, "%*s %s %lf %lf %lf %s %s ", id, &r, &x, &y, cor1, cor2);
        
    Circle b = Circle_new(x,y,r);

    svgObject obj;

    if(runTime_styleIsSet(rt, S_CIRCLE)){
        obj = svgObject_new(CIRCLE, DEFAULT,id, b, cor1, cor2, runTime_getStyle(rt, CIRCLE, STYLE));
    }else{
        obj = svgObject_new(CIRCLE, DEFAULT, id, b, cor1, cor2, NULL);
    }

    listaStatic_add(lista, obj);
    //TODO: Adicionar o obj na estrutura agora.

    //o svgObject realiza uma cópia das string dentro dele alocando somente o tamanho necessário
    free(id); free(cor1); free(cor2); 

    if(r == -1 || x == -1 || y == -1){
        reportError(__func__, "Falha no Scanf - Repassado valores negativos (-1) : Circulo");
    }
}

void geo_R(char* comando, listaStatic lista, Runtime rt){

    char* id = calloc(64, sizeof(char));
    double w = -1, h = -1, x = -1,y = -1;
    char *cor1 = calloc(32, sizeof(char));
    char *cor2 = calloc(32, sizeof(char));

    sscanf(comando, "%*s %s %lf %lf %lf %lf %s %s ", id, &w, &h, &x, &y, cor1, cor2);
    
    Rectangle b = Rectangle_new(x,y,h,w);
    svgObject obj;
    
    if(runTime_styleIsSet(rt, S_RECTANGLE)){
        obj = svgObject_new(RECTANGLE, DEFAULT, id, b, cor1, cor2, runTime_getStyle(rt, RECTANGLE, STYLE));
    }else{
        obj = svgObject_new(RECTANGLE, DEFAULT, id, b, cor1, cor2, NULL);
    }
     

    //o svgObject realiza uma cópia das string dentro dele alocando somente o tamanho necessário
    free(id); free(cor1); free(cor2); 

    //TODO: Adicionar o obj na estrutura agora.
    listaStatic_add(lista, obj);

    if(h == -1 ||  w == -1 || x == -1 || y == -1){
        reportError(__func__, "Falha no Scanf - Repassado valores negativos (-1) : Retangulo");
    }      
}

void geo_T(char* comando, listaStatic lista){
    double x = -1, y = -1;
    char *buffer = calloc(256, sizeof(char));
    sscanf(comando, "%*s %lf %lf %[^\n]s", &x, &y, buffer);
    
    char* texto = calloc((safeStrlen(buffer)+1), sizeof(char));
    sprintf(texto, "%s", buffer);
    free(buffer);


    Text t = Text_new(x,y, texto);
    svgObject obj = svgObject_new(TEXT, DEFAULT,  NULL, t, NULL, NULL, NULL);
      //TODO: Adicionar o obj na estrutura agora.
    listaStatic_add(lista, obj);

    //(*idnegativo)--;

}

void qry_O(char* comando, listaStatic lista, FILE* respostaFile, listaStatic svgList){

    char *j = calloc(32, sizeof(char));
    char *k = calloc(32, sizeof(char));
    sscanf(comando, "%*s %s %s", j, k);

    //Busca os objetos
    svgObject jb = listaStatic_find(lista, svgObject_comparadorID, j);
    svgObject kb = listaStatic_find(lista, svgObject_comparadorID, k);

    bool col = colide(jb, kb);
    Rectangle rxt = envolveObjeto(jb, kb);
    svgObject obj;

    if(col){
        //Linha cheia
        obj = svgObject_new(RECTANGLE, DEFAULT, NULL, rxt, "green", "none", NULL);
        fprintf(respostaFile, "%sSIM\n\n", comando);

    }else{
        obj = svgObject_new(RECTANGLE, DEFAULT, NULL, rxt, "green", "none", "stroke-dasharray: 2.5;");
        fprintf(respostaFile, "%sNÃO\n\n", comando);
    }

    listaStatic_add(svgList, obj);
    
    free(j);
    free(k);

    //Adiciona o SVG a ser escrito
    //liberarSVGobject(obj);

    return;
}

void qry_I(char* comando, listaStatic lista, FILE* respostaFile, listaStatic svgList){
        double x = -1, y = -1;
        char *id = calloc(32, sizeof(char));
        sscanf(comando, "%*s %s %lf %lf", id, &x, &y);

        if(id == NULL || x == -1 || y == -1) 
            reportError(__func__, "Erro ao obter os ID ou valor XY");

        svgObject teste = listaStatic_find(lista, svgObject_comparadorID, id);

        if(teste == NULL)
            reportError(__func__, "Incapaz de proceder: Busca retornou NULL");
    

        Point cord = Point_new(x,y);
        Circle cr = Circle_new(Point_getX(cord), Point_getY(cord), 1);


        if(svgObject_getTipo(teste) == CIRCLE){
            Circle c1 = svgObject_getElemento(teste);
            svgObject respt;

            Point c1_cord = Circle_getCoordenada(c1);

            if(distanciaEntrePontos(cord, c1_cord) < Circle_getRaio(c1)){
                //É interno
                respt = svgObject_new(CIRCLE, DEFAULT, NULL, cr, "green", "green", NULL);
                fprintf(respostaFile, "%sSIM\n\n", comando);
            }else{
                respt = svgObject_new(CIRCLE, DEFAULT, NULL, cr, "red", "red", NULL);
                fprintf(respostaFile, "%sNÃO\n\n", comando);
            }


            Line lt = Line_newXY(x, y, Point_getX(c1_cord), Point_getY(c1_cord));
            svgObject lnrt = svgObject_new(LINE, DEFAULT, NULL, lt, "black", "black", NULL);

            listaStatic_add(svgList, lnrt);
            listaStatic_add(svgList, respt);
            // adicionarSVGFile(lnrt, svgFile);
            // adicionarSVGFile(respt, svgFile);

            // liberarSVGobject(lnrt);
            // liberarSVGobject(respt);
            Point_liberar(cord);

        }else if(svgObject_getTipo(teste) == RECTANGLE){
            Rectangle rt1 = svgObject_getElemento(teste);
            Point rtcord = Rectangle_getCoordenada(rt1);

            double rx = Point_getX(rtcord);
            double ry = Point_getY(rtcord);
            double rl = Rectangle_getLargura(rt1);
            double ra = Rectangle_getAltura(rt1);

            bool intern = contidoNoIntervalo(x,rx, rx + rl, false) 
                            && contidoNoIntervalo(y, ry, ry + ra, false);

            svgObject respt;

            if(intern){
                respt = svgObject_new(CIRCLE, DEFAULT, NULL, cr, "green", "green", NULL);
                fprintf(respostaFile, "%sSIM\n\n", comando);
            }else{
                respt = svgObject_new(CIRCLE, DEFAULT, NULL, cr, "red", "red", NULL);
                 fprintf(respostaFile, "%sNÃO\n\n", comando);
            }
            
            Point centroDeMassa = obterCentroDeMassa(RECTANGLE, rt1);
            
            double cord_x = Point_getX(cord);
            double cord_y = Point_getY(cord);
            double cdm_x = Point_getX(centroDeMassa);
            double cdm_y = Point_getY(centroDeMassa);

            Line lt = Line_newXY(cord_x, cord_y, cdm_x, cdm_y);
            svgObject lnrt = svgObject_new(LINE, DEFAULT, NULL, lt, "black", "black", NULL);

            listaStatic_add(svgList, lnrt);
            listaStatic_add(svgList, respt);
            // adicionarSVGFile(lnrt, svgFile);
            // adicionarSVGFile(respt, svgFile);

            // liberarSVGobject(lnrt);
            // liberarSVGobject(respt);
            Point_liberar(centroDeMassa);
            Point_liberar(cord);

            return;


        }else{
            reportError(__func__, "Caso de teste `i?`: Tipo não compatível");
        }

        free(id);

}

void qry_D(char* comando, listaStatic lista, FILE *respostaFile, listaStatic svgList){
    char *id1 = calloc(32, sizeof(char));
    char *id2 = calloc(32, sizeof(char));

    sscanf(comando, "%*s %s %s", id1, id2);

    svgObject jb = listaStatic_find(lista, svgObject_comparadorID, id1);
    svgObject kb = listaStatic_find(lista, svgObject_comparadorID, id2);

    if(kb == NULL || kb == NULL){
        reportError(__func__, "Incapaz de proceder: Busca retornou NULL");
    }

    Point p1 = obterCentroDeMassa(svgObject_getTipo(jb), svgObject_getElemento(jb));
    Point p2 = obterCentroDeMassa(svgObject_getTipo(kb), svgObject_getElemento(kb));

    double dist = distanciaEntrePontos(p1, p2);

    fprintf(respostaFile, "%s%lf\n\n", comando, dist);

    //Traçar uma reta entre os centros e escrever o valor na metade da linha

    double p1x = Point_getX(p1);
    double p1y = Point_getY(p1);
    double p2x = Point_getX(p2);
    double p2y = Point_getY(p2);

    Line lt = Line_newXY(p1x, p1y, p2x, p2y);

    char* textdist = calloc(32, sizeof(char));
    sprintf(textdist, "%lf", dist);
    double dp1 = ((p2x - p1x)) /2;
    double dp2 = ((p2y - p1y)) /2;

    Text tt = Text_new(p1x + dp1, p1y + dp2, textdist);

    svgObject ox1, ox2;

    ox1 = svgObject_new(LINE, DEFAULT, NULL, lt, "black", "black", NULL);
    ox2 = svgObject_new(TEXT, DEFAULT, NULL, tt, "black", "black", NULL);

    listaStatic_add(svgList, ox1);
    listaStatic_add(svgList, ox2);

    Point_liberar(p1);
    Point_liberar(p2);
    // liberarSVGobject(ox1);
    // liberarSVGobject(ox2);
    free(id1);
    free(id2);

}

//INICIO DA PUTARIA DO COMANDO DO BONDING BOX
void qry_BB2(void* obj, void *argumento){ 

    struct argumento_bb_t{
        FILE* sfxsvg;
        char* cor;
    };
    
    struct argumento_bb_t* bx = argumento;
    FILE *svg = bx->sfxsvg;  
    char *cor = bx->cor;

    adicionarSVGFile(obj, svg);
    
    if(svgObject_getTipo(obj) == CIRCLE){
        void *rx = svgObject_getElemento(obj);
        void *rxt = bbCircle(rx);
        svgObject res = svgObject_new(RECTANGLE, DEFAULT,  NULL, rxt, cor, "none", NULL);
        adicionarSVGFile(res, svg);
        svgObject_liberar(res);

    }else if(svgObject_getTipo(obj) == RECTANGLE){
        void *elpt = moldarRetangulo(svgObject_getElemento(obj));
        svgObject res = svgObject_new(ELLIPSE, DEFAULT, NULL, elpt, cor, "none", NULL);
        adicionarSVGFile(res, svg);
        svgObject_liberar(res);
    }
    
}

void qry_BB(char* comando, listaStatic lista, char* DirComNomeBase){
    char *sufixo = calloc(32, sizeof(char));
    char *cor = calloc(32, sizeof(char));
    sscanf(comando, "%*s %s %s", sufixo, cor);

    char *stpath = calloc(128, sizeof(char));
    sprintf(stpath, "%s-%s.svg", DirComNomeBase, sufixo);
    printf("Caminho de arquivo .svg [bb]: %s\n", stpath);

    iniciarSVGFILE(stpath);
    FILE *sfxsvg = abrirArquivo(stpath, APPEND);

    struct argumento_bb_t{
        FILE* sfxsvg;
        char* cor;
    };

    struct argumento_bb_t *xt = malloc(sizeof(struct argumento_bb_t));
    xt->sfxsvg = sfxsvg;
    xt->cor = cor;

    listaStatic_forEach(lista, qry_BB2, xt);
    //processamentoExterno --> aplica a função em todos elementos da lista
    fclose(sfxsvg);
    finalizarSVGFILE(stpath);
}

/*
# T2
*/

void geo_Q(char* comando, listaStatic quadra, Runtime rt){
    char* id = calloc(64, sizeof(char));
    double x = -1,y = -1, w = -1, h = -1;

    sscanf(comando, "%*s %s %lf %lf %lf %lf", id,&x, &y, &w, &h);

    if(runTime_styleIsSet(rt, S_QUADRA)){
        listaStatic_add(quadra, Quadra_new(id, x, y, w, h, 
        runTime_getStyle(rt, S_QUADRA, COR_PREENCHIMENTO), 
        runTime_getStyle(rt, S_QUADRA, COR_BORDA), 
        runTime_getStyle(rt, S_QUADRA, STYLE)));
    }else{
        listaStatic_add(quadra, Quadra_new(id, x, y, w, h, NULL, NULL, NULL));   
    }
        
    //TODO: Adicionar o obj na estrutura agora.

    //o svgObject realiza uma cópia das string dentro dele alocando somente o tamanho necessário
    free(id); 

    if(w == -1 || h == -1){
        reportError(__func__, "Falha no Scanf - Repassado valores negativos (-1)");
    }
}

void geo_H(char* comando, listaStatic hidra, Runtime rt){
    char* id = calloc(64, sizeof(char));
    double x = -1,y = -1;

    sscanf(comando, "%*s %s %lf %lf", id,&x, &y);

    if(runTime_styleIsSet(rt, S_HIDRANTE)){
        listaStatic_add(hidra, Hidrante_new(id, x, y,
        runTime_getStyle(rt, S_HIDRANTE, COR_PREENCHIMENTO), 
        runTime_getStyle(rt, S_HIDRANTE, COR_BORDA), 
        runTime_getStyle(rt, S_HIDRANTE, STYLE)));
    }else{
        listaStatic_add(hidra, Hidrante_new(id, x, y, NULL, NULL, NULL));
    }
        
    //TODO: Adicionar o obj na estrutura agora.

    //o svgObject realiza uma cópia das string dentro dele alocando somente o tamanho necessário
    free(id); 
}

void geo_S(char* comando, listaStatic sema, Runtime rt){
    char* id = calloc(64, sizeof(char));
    double x = -1,y = -1;

    sscanf(comando, "%*s %s %lf %lf", id,&x, &y);
    
    if(runTime_styleIsSet(rt, S_SEMAFORO)){
        listaStatic_add(sema, Semaforo_new(id, x, y,
        runTime_getStyle(rt, S_SEMAFORO, COR_PREENCHIMENTO), 
        runTime_getStyle(rt, S_SEMAFORO, COR_BORDA), 
        runTime_getStyle(rt, S_SEMAFORO, STYLE)));
    }else{
        listaStatic_add(sema, Semaforo_new(id, x, y, NULL, NULL, NULL));
    }

    //TODO: Adicionar o obj na estrutura agora.

    //o svgObject realiza uma cópia das string dentro dele alocando somente o tamanho necessário
    free(id); 
}

void geo_RB(char* comando, listaStatic radio, Runtime rt){
    char* id = calloc(64, sizeof(char));
    double x = -1,y = -1;

    sscanf(comando, "%*s %s %lf %lf", id,&x, &y);

    if(runTime_styleIsSet(rt, S_RADIO)){
        listaStatic_add(radio, Radio_new(id, x, y,
        runTime_getStyle(rt, S_RADIO, COR_PREENCHIMENTO), 
        runTime_getStyle(rt, S_RADIO, COR_BORDA), 
        runTime_getStyle(rt, S_RADIO, STYLE)));
    }else{
        listaStatic_add(radio, Radio_new(id, x, y, NULL, NULL, NULL));
    }
        
    //TODO: Adicionar o obj na estrutura agora.

    //o svgObject realiza uma cópia das string dentro dele alocando somente o tamanho necessário
    free(id); 
}

void geo_CQ(char* comando, Runtime rt){
    char* fill = calloc(64, sizeof(char));
    char* borda = calloc(64, sizeof(char));
    double espessura = -1;

    sscanf(comando, "%*s %s %s %lf", fill,borda, &espessura);
    runTime_setStyle(rt, S_QUADRA, fill, borda, NULL, espessura);
}

void geo_CH(char* comando, Runtime rt){
    char* fill = calloc(64, sizeof(char));
    char* borda = calloc(64, sizeof(char));
    double espessura = -1;

    sscanf(comando, "%*s %s %s %lf", fill,borda, &espessura);
    runTime_setStyle(rt, S_HIDRANTE, fill, borda, NULL, espessura);
}

void geo_CR(char* comando, Runtime rt){
    char* fill = calloc(64, sizeof(char));
    char* borda = calloc(64, sizeof(char));
    double espessura = -1;

    sscanf(comando, "%*s %s %s %lf", fill,borda, &espessura);
    runTime_setStyle(rt, S_RADIO, fill, borda, NULL, espessura);
}

void geo_CS(char* comando, Runtime rt){
    char* fill = calloc(64, sizeof(char));
    char* borda = calloc(64, sizeof(char));
    double espessura = -1;

    sscanf(comando, "%*s %s %s %lf", fill,borda, &espessura);
    runTime_setStyle(rt, S_SEMAFORO, fill, borda, NULL, espessura);
}

void geo_SW(char* comando, Runtime rt){
    double cr = -1, cc = -1;

    sscanf(comando, "%*s %lf %lf", &cc, &cr);
    runTime_setStyle(rt, S_RECTANGLE, NULL, NULL, NULL, cr);
    runTime_setStyle(rt, S_CIRCLE, NULL, NULL, NULL, cc);
}

void deleteQuadras(void* elemento, void* argumento){
  if(elemento != NULL){

    struct argumento_dq_t{
        Point p;
        listaStatic quadras;
        double raio;
        bool metricaL1;
        FILE* respostaFile;
    };

    Quadra q = elemento;
    struct argumento_dq_t *x = argumento;

    svgObject oct = Quadra_getElemento(q);
    Rectangle rer = svgObject_getElemento(oct);

    double mr = 0;
    mr = maiorDistancia(rer, x->p, x->metricaL1);


    if(mr <= x->raio){
        fprintf(x->respostaFile, "--> Quadra ID: %s\n", svgObject_getId(oct));
        listaStatic_remove(x->quadras, Quadra_comparadorID, svgObject_getId(oct), NULL);
        //listaStatic_stats(x->quadras);
    }

  }
        
}

void qry_DQ(char* comando, Runtime rt, FILE* respostaFile){
    char* l1l2 = calloc(8,sizeof(char));
    char* id = calloc(64, sizeof(char));
    double r = -1;

    sscanf(comando, "%*s %s %s %lf", l1l2, id, &r);
    listaStatic quadras = runTime_getLista(rt,L_QUADRAS);
    listaStatic hidrantes = runTime_getLista(rt,L_HIDRANTES);
    listaStatic semaforos = runTime_getLista(rt,L_SEMAFOROS);
    listaStatic radios = runTime_getLista(rt,L_RADIOS);

    svgObject origin;

    void* objeto;

    char* nomeObj = calloc(32, sizeof(char));

    //Ache o elemento
    objeto = listaStatic_find(hidrantes, Hidrante_comparadorID, id);
    origin = Hidrante_getElemento(objeto);
    //printf("Hidrante!\n");
    sprintf(nomeObj, "Hidrante");

    if(objeto == NULL){
        objeto = listaStatic_find(semaforos, Semaforo_comparadorID, id);
        origin = Semaforo_getElemento(objeto);
        //printf("Semaforo!\n");
        sprintf(nomeObj, "Sefamoro");

        if(objeto == NULL){
            objeto = listaStatic_find(radios, Radio_comparadorID, id);
            origin = Radio_getElemento(objeto);
            //printf("Radio!\n");
            sprintf(nomeObj, "Radio");
        }
    }


    if(objeto == NULL){
        reportError(__func__, "Objeto de referência não encontrado");
        return;
    }        

    
    bool metricaL1 = true;

    if(stringEquals(l1l2, "L2")){
        metricaL1 = false;
    }

    struct argumento_dq_t{
        Point p;
        listaStatic quadras;
        double raio;
        bool metricaL1;
        FILE* respostaFile;
    };

    fprintf(respostaFile, "%sDeletando quadras em um raio de %lf do %s com ID %s\n", comando, r, nomeObj, svgObject_getId(origin));

    Point crcd = Circle_getCoordenada(svgObject_getElemento(origin));

    struct argumento_dq_t *arg = malloc(sizeof(struct argumento_dq_t));
    arg->p = crcd;
    arg->quadras = quadras;
    arg->raio = r;
    arg->metricaL1 = metricaL1;
    arg->respostaFile = respostaFile;

    listaStatic_forEach(quadras, deleteQuadras, arg);
    fprintf(respostaFile, "\n");

    listaStatic_add(runTime_getLista(rt, L_QUERY), svgObject_new(CIRCLE, DEFAULT, NULL, Circle_new(Point_getX(crcd), Point_getY(crcd), 7), "orange", "none", "stroke-width:3px;"));
    listaStatic_add(runTime_getLista(rt, L_QUERY), svgObject_new(CIRCLE, DEFAULT, NULL, Circle_new(Point_getX(crcd), Point_getY(crcd), 9), "gold", "none", "stroke-width:3px;"));
}

void qry_Del(char* comando, Runtime rt, FILE* respostaFile){
    char* id = calloc(64, sizeof(char));

    sscanf(comando, "%*s %s",id);
    listaStatic quadras = runTime_getLista(rt,L_QUADRAS);
    listaStatic hidrantes = runTime_getLista(rt,L_HIDRANTES);
    listaStatic semaforos = runTime_getLista(rt,L_SEMAFOROS);
    listaStatic radios = runTime_getLista(rt,L_RADIOS);

    bool delete = false;
    char* nomeObj = calloc(32, sizeof(char));

    delete = listaStatic_remove(quadras, Quadra_comparadorID, id, NULL);
    sprintf(nomeObj, "Quadra");
    if(!delete){
        delete = listaStatic_remove(hidrantes, Hidrante_comparadorID, id, NULL);
        sprintf(nomeObj, "Hidrante");

        if(!delete){
            delete = listaStatic_remove(semaforos, Semaforo_comparadorID, id, NULL);
            sprintf(nomeObj, "Sefamoro");

            if(!delete){
                delete = listaStatic_remove(radios, Radio_comparadorID, id, NULL);
                sprintf(nomeObj, "Radio");
            }
        }
    }

    if(!delete){
        reportError(__func__, "Objeto não encontrado");
        printf("ID %s\n\n", id);
        return;
    }else{
        fprintf(respostaFile, "%sDeletado o objeto %s\n\n", comando, nomeObj);
    }

}

void pintarQuadras(void* elemento, void* argumento){
  if(elemento != NULL){

    struct argumento_cbq_t{
        Point p;
        double raio;
        char* cor;
        FILE* respostaFile;
    };

    Quadra q = elemento;
    struct argumento_cbq_t *x = argumento;

    svgObject oct = Quadra_getElemento(q);
    Rectangle rer = svgObject_getElemento(oct);

    double mr = 0;
    mr = maiorDistancia(rer, x->p, false);

    if(mr <= x->raio){
        fprintf(x->respostaFile, "--> Quadra ID: %s\n", svgObject_getId(oct));
        svgObject_setCorBorda(oct, x->cor);        
    }

  }
        
}
    
void qry_CBQ(char* comando, listaStatic quadras, FILE* respostaFile){
    char* cor = calloc(64, sizeof(char));
    double x = 0, y = 0, r = 0;

    sscanf(comando, "%*s %lf %lf %lf %s",&x, &y, &r, cor);

    struct argumento_cbq_t{
        Point p;
        double raio;
        char* cor;
        FILE* respostaFile;
    };

    struct argumento_cbq_t *arq = malloc(sizeof(struct argumento_cbq_t));
    arq->p = Point_new(x,y);
    arq->raio = r;
    arq->cor = cor;
    arq->respostaFile = respostaFile;

    fprintf(respostaFile, "%sMudando a cor das quadras em torno de %lf, %lf com raio %lf:\n", comando, x, y, r);
    listaStatic_forEach(quadras, pintarQuadras, arq);

}

void qry_CRD(char* comando, Runtime rt, FILE* respostaFile){
    char* id = calloc(64, sizeof(char));

    sscanf(comando, "%*s %s", id);

    char* resp = calloc(64, sizeof(char));

    void* objeto;
    svgObject origin;
    char* nomeObj = calloc(32, sizeof(char));
    Point cord;

    listaStatic quadras = runTime_getLista(rt,L_QUADRAS);
    listaStatic hidrantes = runTime_getLista(rt,L_HIDRANTES);
    listaStatic semaforos = runTime_getLista(rt,L_SEMAFOROS);
    listaStatic radios = runTime_getLista(rt,L_RADIOS);


    objeto = listaStatic_find(quadras, Quadra_comparadorID, id);
    if(objeto != NULL){
        sprintf(nomeObj, "Quadra");
        origin = Quadra_getElemento(objeto);
        cord = Rectangle_getCoordenada(svgObject_getElemento(origin));
    }else{
        objeto = listaStatic_find(hidrantes, Hidrante_comparadorID, id);
        if(objeto != NULL){
            sprintf(nomeObj, "Hidrante");
            origin = Hidrante_getElemento(objeto);
            cord = Circle_getCoordenada(svgObject_getElemento(origin));
        }else{
            objeto = listaStatic_find(semaforos, Semaforo_comparadorID, id);
            if(objeto != NULL){
                origin = Semaforo_getElemento(objeto);
                sprintf(nomeObj, "Sefamoro");
                cord = Circle_getCoordenada(svgObject_getElemento(origin));
            }else{
                objeto = listaStatic_find(radios, Radio_comparadorID, id);
                if(objeto != NULL){
                    origin = Radio_getElemento(objeto);
                    sprintf(nomeObj, "Radio");
                    cord = Circle_getCoordenada(svgObject_getElemento(origin));
                }else{
                    reportError(__func__, "Objeto não encontrado!");
                    fprintf(respostaFile, "%sNão encontrado\n\n", comando);
                    return;
                }
            }
        }
    }

    fprintf(respostaFile, "%s%s ID: %s | X: %lf Y: %lf\n", comando, nomeObj, id, Point_getX(cord), Point_getY(cord));


}

void moveElement(void* elemento, void* argumento){
  if(elemento != NULL){

    struct argumento_mv_t{
        Rectangle box;
        double dx, dy;
        FILE* respostaFile;
        Classe tp;
    };

    struct argumento_mv_t *x = argumento;
    svgObject svo;

    switch(x->tp){
        case QUADRA:
            svo = Quadra_getElemento(elemento);
            break;
        case HIDRANTE:
            svo = Hidrante_getElemento(elemento);
            break;
        case SEMAFORO:
            svo = Semaforo_getElemento(elemento);
            break;
        case RADIO:
            svo = Radio_getElemento(elemento);
            break;
        default:
            reportError(__func__, "Unhandled Type");
            return;
            break;
    }

    geometryType tipoSVO = svgObject_getTipo(svo);

    if(isContido(svo, x->box)){
        if(tipoSVO == CIRCLE){
            Point cdc = Circle_getCoordenada(svgObject_getElemento(svo));

            fprintf(x->respostaFile, "Movendo %s\n", svgObject_getId(svo));
            fprintf(x->respostaFile, "(%lf, %lf) -->", Point_getX(cdc), Point_getY(cdc));
            Point_change(cdc, x->dx, x->dy);
            fprintf(x->respostaFile, "(%lf, %lf)\n", Point_getX(cdc), Point_getY(cdc));
        }else if(tipoSVO == RECTANGLE){
            Point cdc = Rectangle_getCoordenada(svgObject_getElemento(svo));

            fprintf(x->respostaFile, "Movendo %s\n", svgObject_getId(svo));
            fprintf(x->respostaFile, "(%lf, %lf) -->", Point_getX(cdc), Point_getY(cdc));
            Point_change(cdc, x->dx, x->dy);
            fprintf(x->respostaFile, "(%lf, %lf)\n", Point_getX(cdc), Point_getY(cdc));
        }else{
            reportError(__func__, "Unhandled Type");
        }

    }
  }
        
}

void qry_Trns(char* comando, Runtime rt, FILE* respostaFile){
    double x = 0, y = 0, w = 0, h = 0, dx = 0, dy = 0;
    sscanf(comando, "%*s %lf %lf %lf %lf %lf %lf", &x, &y, &w, &h, &dx, &dy);

    struct argumento_mv_t{
        Rectangle box;
        double dx, dy;
        FILE* respostaFile;
        Classe tp;
    };

    listaStatic quadras = runTime_getLista(rt,L_QUADRAS);
    listaStatic hidrantes = runTime_getLista(rt,L_HIDRANTES);
    listaStatic semaforos = runTime_getLista(rt,L_SEMAFOROS);
    listaStatic radios = runTime_getLista(rt,L_RADIOS);

    struct argumento_mv_t *arg = malloc(sizeof(struct argumento_mv_t));
    arg->box = Rectangle_new(x,y,h,w);
    arg->dx = dx;
    arg->dy = dy;
    arg->respostaFile = respostaFile;

    fprintf(respostaFile,"%s", comando);

    arg->tp = QUADRA;
    listaStatic_forEach(quadras, moveElement, arg);
    arg->tp = HIDRANTE;
    listaStatic_forEach(hidrantes, moveElement, arg);
    arg->tp = SEMAFORO;
    listaStatic_forEach(semaforos, moveElement, arg);
    arg->tp = RADIO;
    listaStatic_forEach(radios, moveElement, arg);


}