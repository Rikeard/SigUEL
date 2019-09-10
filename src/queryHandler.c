#include "header/queryHandler.h"

void geo_Nx(char* comando, Runtime rt){

    int maxcr = -1;
    int maxq = -1;
    int maxh = -1;
    int maxs = -1;
    int maxr = -1;
    int maxp = -1;
    int maxm = -1;

    sscanf(comando, "%*s %d %d %d %d %d %d %d", &maxcr, &maxq, &maxh, &maxs, &maxr, &maxp, &maxm);

    if(maxcr == -1 || maxq == -1 || maxh == -1 || maxs == -1 || maxs == -1 || maxr == -1 || maxp == -1 || maxm == -1)
        reportError(__func__, "Valor identificado incorretamente");

    listaStatic_setSize(runTime_getLista(rt, L_GERAL), maxcr);
    listaStatic_setSize(runTime_getLista(rt, L_QUADRAS), maxq);
    listaStatic_setSize(runTime_getLista(rt, L_HIDRANTES), maxh);
    listaStatic_setSize(runTime_getLista(rt, L_SEMAFOROS), maxs);
    listaStatic_setSize(runTime_getLista(rt, L_RADIOS), maxr);
    listaStatic_setSize(runTime_getLista(rt, L_PREDIOS), maxp);
    listaStatic_setSize(runTime_getLista(rt, L_MUROS), maxm);

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
        printf("\n%s\n", comando);
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

void geo_PRD(char* comando, Runtime rt){
    char* cep = calloc(64, sizeof(char));
    char* lado = calloc(2, sizeof(char));
    double f = -1, //Largura do prédio
    p = -1, //Profundidade 
    num = -1, //Distancia até metade da largura prédio
    mrg = -1; //Calçada 

    sscanf(comando, "%*s %s %s %lf %lf %lf %lf", cep, lado, &num, &f, &p, &mrg);
    listaStatic quadras = runTime_getLista(rt,L_QUADRAS);

    listaStatic predios = runTime_getLista(rt,L_PREDIOS);

    void *objeto = listaStatic_find(quadras, Quadra_comparadorID, cep);
    if(objeto == NULL){
        reportError(__func__, "Quadra não encontrada");
        //Quadra não encontrada
        return;
    }

    svgObject oct = Quadra_getElemento(objeto);
    Rectangle rer = svgObject_getElemento(oct);
    Point point = Rectangle_getCoordenada(rer);

    listaStatic tex = runTime_getLista(rt, L_QUERY);


    double x = Point_getX(point), y = Point_getY(point), h = Rectangle_getAltura(rer), w = Rectangle_getLargura(rer);

    if(stringEquals(lado, "S")){
        double localX = x + num - (f/2);
        double localY = y + mrg;

        Predio pr = Predio_new(cep, localX, localY, f, p, 
        runTime_getStyle(rt, S_PREDIO, COR_PREENCHIMENTO), 
        runTime_getStyle(rt, S_PREDIO, COR_BORDA), 
        runTime_getStyle(rt, S_PREDIO, STYLE));

        char* nm = calloc(10, sizeof(char));
        sprintf(nm, "%.0lf", num);


        Text t = Text_new(localX, localY + p, nm);

        svgObject textObjet = svgObject_new(TEXT, DEFAULT, NULL, t, NULL, NULL, NULL);

        listaStatic_add(tex, textObjet);

        listaStatic_add(predios, pr);

    }else if(stringEquals(lado, "N")){
        double localX = x + num - (f/2);
        double localY = y + h - mrg - p;

        Predio pr = Predio_new(cep, localX, localY, f, p, 
        runTime_getStyle(rt, S_PREDIO, COR_PREENCHIMENTO), 
        runTime_getStyle(rt, S_PREDIO, COR_BORDA), 
        runTime_getStyle(rt, S_PREDIO, STYLE));

        char* nm = calloc(10, sizeof(char));
        sprintf(nm, "%.0lf", num);


        Text t = Text_new(localX, localY + p, nm);

        svgObject textObjet = svgObject_new(TEXT, DEFAULT, NULL, t, NULL, NULL, NULL);

        listaStatic_add(tex, textObjet);
        

        listaStatic_add(predios, pr);

    }else if(stringEquals(lado, "O")){
        double localX = x + w - mrg -p;
        double localY = y + num - (f/2);

        Predio pr = Predio_new(cep, localX, localY, p, f, 
        runTime_getStyle(rt, S_PREDIO, COR_PREENCHIMENTO), 
        runTime_getStyle(rt, S_PREDIO, COR_BORDA), 
        runTime_getStyle(rt, S_PREDIO, STYLE));

        char* nm = calloc(10, sizeof(char));
        sprintf(nm, "%.0lf", num);


        Text t = Text_new(localX, localY + f, nm);
        svgObject textObjet = svgObject_new(TEXT, DEFAULT, NULL, t, NULL, NULL, NULL);

        listaStatic_add(tex, textObjet);
        

        listaStatic_add(predios, pr);
        
    }else if(stringEquals(lado, "L")){
        double localX = x + mrg;
        double localY = y + num - (f/2);

        Predio pr = Predio_new(cep, localX, localY, p, f, 
        runTime_getStyle(rt, S_PREDIO, COR_PREENCHIMENTO), 
        runTime_getStyle(rt, S_PREDIO, COR_BORDA), 
        runTime_getStyle(rt, S_PREDIO, STYLE));

        char* nm = calloc(10, sizeof(char));
        sprintf(nm, "%.0lf", num);

        Text t = Text_new(localX, localY + f, nm);
        svgObject textObjet = svgObject_new(TEXT, DEFAULT, NULL, t, NULL, NULL, NULL);

        listaStatic_add(tex, textObjet);
        

        listaStatic_add(predios, pr);
    }else{
        reportError(__func__, lado);
    }
}

void geo_MUR(char* comando, Runtime rt){
    double x1 = -1, y1 = -1, x2 = -1, y2 = -1; 

    sscanf(comando, "%*s %lf %lf %lf %lf", &x1, &y1, &x2, &y2);

    listaStatic muros = runTime_getLista(rt,L_MUROS);

    /* printf("MURO EM (%lf %lf) (%lf %lf)\n", x1, y1, x2, y2); */

    Muro mh = Muro_new(x1, y1, x2, y2, 
        runTime_getStyle(rt, S_MURO, COR_PREENCHIMENTO), 
        runTime_getStyle(rt, S_MURO, COR_BORDA), 
        runTime_getStyle(rt, S_MURO, STYLE));

    listaStatic_add(muros, mh);
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

////

void adicionarAoQuery(void* elemento, void* argumento){
  if(elemento != NULL){
      svgObject s = svgObject_new(TRIANGLE, DEFAULT, NULL, elemento, "none", "gold", NULL);
      listaStatic_add(argumento, s);
  }
}

void qry_Brl(char* comando, Runtime rt, FILE* fl){
    double x = 0, y = 0;
    sscanf(comando, "%*s %lf %lf", &x, &y);

    listaStatic muros = runTime_getLista(rt,L_MUROS);
    listaStatic predios = runTime_getLista(rt,L_PREDIOS);
    listaStatic query = runTime_getLista(rt, L_QUERY);

    listaStatic triangles = processarVisibilidade(x, y, muros, predios);
    listaStatic_forEach(triangles, adicionarAoQuery, query);

    listaStatic sPos = runTime_getLista(rt, L_SUPERPOS);
    char* kabum = malloc(sizeof(char)*5000);

    sprintf(kabum, "<g z-index=\"100\" transform=\"translate(%lf %lf) scale(0.2 0.2)\"><path d=\"m396.699219 126.699219c-40.5 42-69 92.699219-84 147.601562-1.199219 5.097657 12.601562 10.199219 11.402343 15.296875-1.203124 4.5-17.101562 9.300782-17.703124 14.101563-1.5 8.101562-2.699219 16.5-3.597657 24.898437-.902343 9.902344-1.5 20.101563-1.800781 30.300782v123.101562h-90v-123.101562c-.300781-10.199219-.902344-20.398438-1.800781-30.296876-.902344-8.402343-2.101563-16.5-3.601563-24.902343-.601562-5.097657-16.796875-9.898438-17.699218-14.699219-1.199219-4.800781 12.300781-9.601562 11.097656-14.398438-14.699219-55.503906-43.496094-106.203124-84.296875-148.5l-24.601563-25.5 165.902344.296876 165.300781.300781zm0 0\" fill=\"#fdbf00\"/><path d=\"m396.699219 126.699219c-40.5 42-69 92.699219-84 147.601562-1.199219 5.097657 12.601562 10.199219 11.402343 15.296875-1.203124 4.5-17.101562 9.300782-17.703124 14.101563-1.5 8.101562-2.699219 16.5-3.597657 24.898437-.902343 9.902344-1.5 20.101563-1.800781 30.300782v123.101562h-45v-381.101562l165.300781.300781zm0 0\" fill=\"#ff9f00\"/><path d=\"m436 512h-360c0-30.601562 24-57 54.300781-59.699219l7.800781-.902343 3.898438-6.597657c7.800781-13.800781 23.398438-22.800781 39.300781-22.800781 3.300781 0 6.597657.597656 9.898438 1.199219l9.902343 2.402343 6-8.101562c11.398438-15.898438 29.699219-25.5 48.898438-25.5s37.5 9.601562 48.898438 25.5l6 8.101562 9.902343-2.402343c3.300781-.601563 6.597657-1.199219 9.898438-1.199219 15.902343 0 31.5 9 39.300781 22.800781l3.898438 6.597657 7.800781.902343c30.300781 2.699219 54.300781 29.097657 54.300781 59.699219zm0 0\" fill=\"#ffd400\"/><path d=\"m391 316c0 28.5-50.097656 39-90 42.898438-20.097656 1.800781-37.5 2.101562-45 2.101562s-24.902344-.300781-45-2.101562c-39.902344-3.898438-90-14.398438-90-42.898438 0-21 26.101562-35.101562 78-41.398438 2.699219 9.597657 5.101562 19.199219 6.601562 29.097657-27.601562 3.300781-43.203124 8.699219-50.402343 12.300781 8.699219 4.5 27 9.601562 54 12.597656 13.5 1.5 29.101562 2.402344 46.800781 2.402344s33.300781-.902344 46.800781-2.402344c27-2.996094 45.300781-8.101562 54-12.597656-7.199219-3.601562-22.800781-9-50.402343-12.300781 1.5-9.898438 3.902343-19.800781 6.300781-29.398438 51.902343 6.597657 78.300781 20.699219 78.300781 41.699219zm0 0\" fill=\"#ffd400\"/><path d=\"m445.601562 90.300781c-12.300781-35.699219-45.601562-60.300781-84.601562-60.300781-9.300781 0-18.601562 1.5-27.597656 4.5-19.804688-21.902344-48.003906-34.5-77.402344-34.5s-57.597656 12.597656-77.402344 34.5c-8.996094-3-18.296875-4.5-27.597656-4.5-39 0-72.300781 24.601562-84.601562 60.300781-35.097657-3.300781-66.398438 25.898438-66.398438 60.699219 0 33 27 60 60 60 15 0 30.097656-5.398438 40.902344-15.300781 29.398437 19.800781 70.496094 20.101562 100.496094-.300781 16.5 10.203124 35.402343 15.300781 54.601562 15.300781s38.101562-5.097657 54.601562-15.300781c29.699219 20.402343 71.097657 20.101562 100.496094.300781 10.804688 9.902343 25.902344 15.300781 40.902344 15.300781 33 0 60-27 60-60 0-34.800781-31.300781-64-66.398438-60.699219zm0 0\" fill=\"#ffd400\"/><g fill=\"#fdbf00\"><path d=\"m301 358.898438c-20.097656 1.800781-37.5 2.101562-45 2.101562v-30c17.699219 0 33.300781-.902344 46.800781-2.402344 27-2.996094 45.300781-8.101562 54-12.597656-7.199219-3.601562-22.800781-9-50.402343-12.300781 1.5-9.898438 3.902343-19.800781 6.300781-29.398438 51.902343 6.597657 78.300781 20.699219 78.300781 41.699219 0 28.5-50.097656 39-90 42.898438zm0 0\"/><path d=\"m436 512h-180v-120c19.199219 0 37.5 9.601562 48.898438 25.5l6 8.101562 9.902343-2.402343c3.300781-.601563 6.597657-1.199219 9.898438-1.199219 15.902343 0 31.5 9 39.300781 22.800781l3.898438 6.597657 7.800781.902343c30.300781 2.699219 54.300781 29.097657 54.300781 59.699219zm0 0\"/><path d=\"m512 151c0 33-27 60-60 60-15 0-30.097656-5.398438-40.902344-15.300781-29.398437 19.800781-70.796875 20.101562-100.496094-.300781-16.5 10.203124-35.402343 15.300781-54.601562 15.300781v-210.699219c29.398438 0 57.597656 12.597656 77.402344 34.5 8.996094-3 18.296875-4.5 27.597656-4.5 39 0 72.300781 24.601562 84.601562 60.300781 35.097657-3.300781 66.398438 25.898438 66.398438 60.699219zm0 0\"/></g></g>\n", x-50, y-70);

    listaStatic_add(sPos, kabum);

}

////

double getValueFromElement(void* elemento){
    struct element{
        void* elemento;
        double distancia;
    };

    struct element *xd = elemento;
    return xd->distancia;
}

void nsSemaforos(void* elemento, void* argumento){
    if(elemento != NULL){

        struct argumento_lista{
            int index;
            void** lista;
            Point localizacao;
        };

        struct element{
            void* elemento;
            double distancia;
        };

        struct argumento_lista *al = argumento;

        svgObject so = Semaforo_getElemento(elemento);
        Circle cir = svgObject_getElemento(so);
        Point px = Circle_getCoordenada(cir);

        double d = distanciaEntrePontos(al->localizacao, px);

        struct element *elm = malloc(sizeof(struct element));

        elm->elemento = elemento;
        elm->distancia = d;

        al->lista[al->index] = elm;
        al->index++;

    }
}

void nsHidrantes(void* elemento, void* argumento){
    if(elemento != NULL){

        struct argumento_lista{
            int index;
            void** lista;
            Point localizacao;
        };

        struct element{
            void* elemento;
            double distancia;
        };

        struct argumento_lista *al = argumento;

        svgObject so = Hidrante_getElemento(elemento);
        Circle cir = svgObject_getElemento(so);
        Point px = Circle_getCoordenada(cir);

        double d = distanciaEntrePontos(al->localizacao, px);

        struct element *elm = malloc(sizeof(struct element));

        elm->elemento = elemento;
        elm->distancia = d;

        al->lista[al->index] = elm;
        al->index++;

    }
}

/*
Foco de incêndio identificado na coordenada (x,y). 
Encontrar os ns semáforos mais próximos do foco e os hidrantes que estejam a uma distância de até r do foco.

TXT: identificação dos semáforos que devem ter programação alterada e dos hidrantes que devem ser ativados

SVG: hidrantes e semafóros circulados, linha ligando foco ao hidrante/semáforo.
*/
void qry_Fi(char* comando, Runtime rt, FILE* svg, FILE* respostaFile){
    double x = 0, y = 0, r = 0;
    int ns = 0;
    sscanf(comando, "%*s %lf %lf %d %lf", &x, &y, &ns, &r);

    listaStatic semaforos = runTime_getLista(rt, L_SEMAFOROS);
    listaStatic hidrantes = runTime_getLista(rt, L_HIDRANTES);
    listaStatic qry = runTime_getLista(rt, L_QUERY);

    Point loc = Point_new(x, y);

    struct argumento_lista{
        int index;
        void** lista;
        Point localizacao;
    };

    struct element{
        void* elemento;
        double distancia;
    };


    void** listSemaforos = malloc(sizeof(void*) * listaStatic_length(semaforos));
    void** listHidrantes = malloc(sizeof(void*) * listaStatic_length(hidrantes));

    struct argumento_lista *al = malloc(sizeof(struct argumento_lista));
    al->index = 0;
    al->lista = listSemaforos;
    al->localizacao = loc;

    struct argumento_lista *ah = malloc(sizeof(struct argumento_lista));
    ah->index = 0;
    ah->lista = listHidrantes;
    ah->localizacao = loc;

    listaStatic_forEach(semaforos, nsSemaforos, al);
    listaStatic_forEach(hidrantes, nsHidrantes, ah);

    heapMenor(listSemaforos, al->index, ns, getValueFromElement);
    heapMaior(listHidrantes, ah->index, ah->index, getValueFromElement);


    fprintf(respostaFile,"%s\n", comando);
    fprintf(respostaFile, "Os seguintes semáforos deverão ter a sua programação alterada\n");


    for(int i = al->index-1; i >= al->index - ns; i--){
        Semaforo rx = ((struct element*) listSemaforos[i])->elemento;
        svgObject svo = Semaforo_getElemento(rx);
        fprintf(respostaFile, "ID: %s -- Distancia: %lf\n", svgObject_getId(svo), getValueFromElement(listSemaforos[i]));

        Circle origem = svgObject_getElemento(svo);
        Point cord = Circle_getCoordenada(origem);

        Line lt = Line_newXY(Point_getX(cord), Point_getY(cord), x, y);
        svgObject ox1 = svgObject_new(LINE, DEFAULT, NULL, lt, "purple", "purple", "\" z-index=\"100");

        Circle c = Circle_new(Point_getX(cord), Point_getY(cord), 7);
        svgObject ox2 = svgObject_new(CIRCLE, DEFAULT, NULL, c, "red", "none", "\" z-index=\"100");

        listaStatic_add(qry, ox1);
        listaStatic_add(qry, ox2);
    }

    fprintf(respostaFile, "\nOs seguintes hidrantes deverão ser estourados\n");
    for(int i = 0; i < ah->index; i++){
        double dis = getValueFromElement(listHidrantes[i]);
        if(dis > r){
            break;
        }
        Hidrante rx = ((struct element*) listHidrantes[i])->elemento;
        svgObject svo = Hidrante_getElemento(rx);
        fprintf(respostaFile, "ID: %s -- Distancia: %lf\n", svgObject_getId(svo), dis);

        Circle origem = svgObject_getElemento(svo);
        Point cord = Circle_getCoordenada(origem);

        Line lt = Line_newXY(Point_getX(cord), Point_getY(cord), x, y);
        svgObject ox1 = svgObject_new(LINE, DEFAULT, NULL, lt, "purple", "purple", "\" z-index=\"100");

        Circle c = Circle_new(Point_getX(cord), Point_getY(cord), 7);
        svgObject ox2 = svgObject_new(CIRCLE, DEFAULT, NULL, c, "red", "none", " \" z-index=\"100");

        listaStatic_add(qry, ox1);
        listaStatic_add(qry, ox2);
    }

    listaStatic sPos = runTime_getLista(rt, L_SUPERPOS);
    char* fogueira = malloc(sizeof(char)*2048);

    sprintf(fogueira, "<g z-index=\"101\" transform=\"translate(%lf %lf) scale(0.2 0.2)\"><path style=\"fill:#FFB446;\" d=\"M97.103,353.103C97.103,440.86,168.244,512,256,512l0,0c87.756,0,158.897-71.14,158.897-158.897c0-88.276-44.138-158.897-14.524-220.69c0,0-47.27,8.828-73.752,79.448c0,0-88.276-88.276-51.394-211.862c0,0-89.847,35.31-80.451,150.069c8.058,98.406-9.396,114.759-9.396,114.759c0-79.448-62.115-114.759-62.115-114.759C141.241,247.172,97.103,273.655,97.103,353.103z\"/><path style=\"fill:#FFDC64;\" d=\"M370.696,390.734c0,66.093-51.033,122.516-117.114,121.241c-62.188-1.198-108.457-48.514-103.512-110.321c2.207-27.586,23.172-72.276,57.379-117.517l22.805,13.793C229.517,242.023,256,167.724,256,67.724C273.396,246.007,370.696,266.298,370.696,390.734z\"/><path style=\"fill:#FFFFFF;\" d=\"M211.862,335.448c-8.828,52.966-26.483,72.249-26.483,105.931C185.379,476.69,216.998,512,256,512	l0,0c39.284,0,70.729-32.097,70.62-71.381c-0.295-105.508-61.792-158.136-61.792-158.136c8.828,52.966-17.655,79.448-17.655,79.448C236.141,345.385,211.862,335.448,211.862,335.448z\"/></g>\n", x-55, y-70);

    listaStatic_add(sPos, fogueira);

}

/*
Determinar os k hidrantes mais próximos (se -k) ou mais distantes (se +k) do endereço.

TXT: Listar identificadores
SVG: Circular o hidrantes. Colocar linhas do endereço aos hidrantes.
*/
void qry_Fh(char* comando, Runtime rt, FILE* respostaFile){
    int k = 0;
    double num = 0;
    char* cep = calloc(64, sizeof(char));
    char* lado = calloc(2, sizeof(char));
    char pos;
    sscanf(comando, "%*s %c%d %s %s %lf", &pos, &k, cep, lado, &num);


    bool maisProximos = false;

    if(pos == '-'){
        maisProximos = true;
    }else if(pos == '+'){
        maisProximos = false;
    }else{
        reportError(__func__, "Parse errado no sinal de +-");
    }


    listaStatic quadras = runTime_getLista(rt,L_QUADRAS);


    void *objeto = listaStatic_find(quadras, Quadra_comparadorID, cep);
    if(objeto == NULL){
        reportError(__func__, "Quadra não encontrada");
        //Quadra não encontrada
        return;
    }

    svgObject oct = Quadra_getElemento(objeto);
    Rectangle rer = svgObject_getElemento(oct);
    Point point = Rectangle_getCoordenada(rer);

    double x = Point_getX(point), y = Point_getY(point), h = Rectangle_getAltura(rer), w = Rectangle_getLargura(rer);
    double localX = 0, localY = 0;

    if(stringEquals(lado, "N")){
        localX = x + num;
        localY = y + h;

    }else if(stringEquals(lado, "S")){
        localX = x + num;
        localY = y;

    }else if(stringEquals(lado, "L")){
        localX = x;
        localY = y;

    }else if(stringEquals(lado, "O")){
        localX = x + w;
        localY = y + num;

    }else{
        reportError(__func__, "Lado não identificado");
    }

    struct argumento_lista{
        int index;
        void** lista;
        Point localizacao;
    };

    struct element{
        void* elemento;
        double distancia;
    };


    listaStatic hidrantes = runTime_getLista(rt, L_HIDRANTES);
    void** listHidrantes = malloc(sizeof(void*) * listaStatic_length(hidrantes));

    Point loc = Point_new(localX, localY);

    struct argumento_lista *ah = malloc(sizeof(struct argumento_lista));
    ah->index = 0;
    ah->lista = listHidrantes;
    ah->localizacao = loc;

    listaStatic_forEach(hidrantes, nsHidrantes, ah);

    fprintf(respostaFile,"%s\n", comando);

    if(maisProximos){
        heapMenor(listHidrantes, ah->index, k, getValueFromElement);
        fprintf(respostaFile, "Os %d hidrantes mais próximos são\n", k);

    }else{
        heapMaior(listHidrantes, ah->index, k, getValueFromElement);
        fprintf(respostaFile, "Os %d hidrantes mais distantes são\n", k);
    }

    listaStatic qry = runTime_getLista(rt, L_QUERY);

    for(int i = ah->index-1; i >= ah->index - k; i--){
        double dis = getValueFromElement(listHidrantes[i]);

        Hidrante rx = ((struct element*) listHidrantes[i])->elemento;
        svgObject svo = Hidrante_getElemento(rx);
        fprintf(respostaFile, "ID: %s -- Distancia: %lf\n", svgObject_getId(svo), dis);

        Circle origem = svgObject_getElemento(svo);
        Point cord = Circle_getCoordenada(origem);

        Line lt = Line_newXY(Point_getX(cord), Point_getY(cord), x, y);
        svgObject ox1 = svgObject_new(LINE, DEFAULT, NULL, lt, "purple", "purple", "\" z-index=\"100");

        Circle c = Circle_new(Point_getX(cord), Point_getY(cord), 7);
        svgObject ox2 = svgObject_new(CIRCLE, DEFAULT, NULL, c, "red", "none", " \" z-index=\"100");

        listaStatic_add(qry, ox1);
        listaStatic_add(qry, ox2);
    }

    fprintf(respostaFile, "\n\n");

}

/*
Determinar os k semáforos mais próximos do endereço cep,face,num.

TXT: Listar identificadores
SVG: Circular semáforos. Colocar linhas do endereço para o semáforo
*/
void qry_Fs(char* comando, Runtime rt, FILE* respostaFile){
    int k = 0;
    double num = 0;
    char* cep = calloc(64, sizeof(char));
    char* lado = calloc(2, sizeof(char));
    sscanf(comando, "%*s %d %s %s %lf", &k, cep, lado, &num);



    listaStatic quadras = runTime_getLista(rt,L_QUADRAS);


    void *objeto = listaStatic_find(quadras, Quadra_comparadorID, cep);
    if(objeto == NULL){
        reportError(__func__, "Quadra não encontrada");
        //Quadra não encontrada
        return;
    }

    svgObject oct = Quadra_getElemento(objeto);
    Rectangle rer = svgObject_getElemento(oct);
    Point point = Rectangle_getCoordenada(rer);

    double x = Point_getX(point), y = Point_getY(point), h = Rectangle_getAltura(rer), w = Rectangle_getLargura(rer);
    double localX = 0, localY = 0;

    if(stringEquals(lado, "N")){
        localX = x + num;
        localY = y + h;

    }else if(stringEquals(lado, "S")){
        localX = x + num;
        localY = y;

    }else if(stringEquals(lado, "L")){
        localX = x;
        localY = y;

    }else if(stringEquals(lado, "O")){
        localX = x + w;
        localY = y + num;

    }else{
        reportError(__func__, "Lado não identificado");
    }

    struct argumento_lista{
        int index;
        void** lista;
        Point localizacao;
    };

    struct element{
        void* elemento;
        double distancia;
    };


    listaStatic semaforos = runTime_getLista(rt, L_SEMAFOROS);
    void** listSemaforos = malloc(sizeof(void*) * listaStatic_length(semaforos));

    Point loc = Point_new(localX, localY);

    struct argumento_lista *ah = malloc(sizeof(struct argumento_lista));
    ah->index = 0;
    ah->lista = listSemaforos;
    ah->localizacao = loc;

    listaStatic_forEach(semaforos, nsHidrantes, ah);

    fprintf(respostaFile,"%s\n", comando);

    heapMenor(listSemaforos, ah->index, k, getValueFromElement);
    fprintf(respostaFile, "Os %d semáforos mais próximos são\n", k);


    listaStatic qry = runTime_getLista(rt, L_QUERY);

    for(int i = ah->index-1; i >= ah->index - k; i--){
        double dis = getValueFromElement(listSemaforos[i]);

        Hidrante rx = ((struct element*) listSemaforos[i])->elemento;
        svgObject svo = Semaforo_getElemento(rx);
        fprintf(respostaFile, "ID: %s -- Distancia: %lf\n", svgObject_getId(svo), dis);

        Circle origem = svgObject_getElemento(svo);
        Point cord = Circle_getCoordenada(origem);

        Line lt = Line_newXY(Point_getX(cord), Point_getY(cord), x, y);
        svgObject ox1 = svgObject_new(LINE, DEFAULT, NULL, lt, "purple", "purple", "\" z-index=\"100");

        Circle c = Circle_new(Point_getX(cord), Point_getY(cord), 7);
        svgObject ox2 = svgObject_new(CIRCLE, DEFAULT, NULL, c, "red", "none", " \" z-index=\"100");

        listaStatic_add(qry, ox1);
        listaStatic_add(qry, ox2);
    }

    fprintf(respostaFile, "\n\n");

}