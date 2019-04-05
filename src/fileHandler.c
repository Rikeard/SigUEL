#include "header/fileHandler.h"
#include "header/geometry.h"
#include "header/svg.h"
#include "header/tree.h"
#include "header/fileMaker.h"

FILE* abrirArquivo(char* path, fileTYPE a){
    FILE *file;
    switch(a){
        case READONLY:
        {
            file = fopen(path, "r");
            break;
        }

        case WRITE:
        {
            file = fopen(path, "w+");
            break;
        }

        case APPEND:
        {
            file = fopen(path, "a+");
        }

    }

    if(file == NULL){
        reportError(__func__, path);
        perror("Reportado ");
    }

    return file;
}

svgObject* processaComandoGEO(char* comando, treeNode* raiz, int* idnegativo){
    char tipo[10];
    sscanf(comando, "%s", tipo);
    
    if(strcmp(tipo, "nx") == 0){
        //Define limite maximo de elementos
        //
        return NULL;
    }

    if(strcmp(tipo, "c") == 0){
        //Cadastra um círculo na árvore
        int id = -1;
        double r = -1,x = -1,y = -1;
        char cor1[32], cor2[32];
        sscanf(comando, "%*s %d %lf %lf %lf %s %s ", &id, &r, &x, &y, cor1, cor2);
        
        Circle* b = criarCirculo(x,y,r);
        svgObject* obj = criarSVGObject(CIRCLE, id, b, cor1, cor2, NULL);
        adicionarElemento(raiz, obj);

        if(id == -1 || r == -1 || x == -1 || y == -1){
            reportError(__func__, "Falha no Scanf - Repassado valores negativos (-1) : Circulo");
        }

        return obj;
    }

    if(strcmp(tipo, "r") == 0){
        //Cadastra um retângulo na árvore
        int id = -1;
        double w = -1, h = -1, x = -1,y = -1;
        char cor1[32], cor2[32];
        sscanf(comando, "%*s %d %lf %lf %lf %lf %s %s ", &id, &w, &h, &x, &y, cor1, cor2);
        
        Rectangle* b = criarRetangulo(x,y,w,h);
        svgObject* obj = criarSVGObject(RECTANGLE, id, b, cor1, cor2, NULL);
        adicionarElemento(raiz, obj);

        if(id == -1 || h == -1 ||  w == -1 || x == -1 || y == -1){
            reportError(__func__, "Falha no Scanf - Repassado valores negativos (-1) : Retangulo");
        }      

        return obj;
    }

    if(strcmp(tipo, "t") == 0 ){
        double x = -1, y = -1;
        char buffer[safeStrlen((comando))];
        sscanf(comando, "%*s %lf %lf %[^\n]s", &x, &y, buffer);
        
        char* texto = malloc(sizeof(char)*(safeStrlen(buffer)+1));
        sprintf(texto, "%s", buffer);
        Text* t = criarTexto(x,y, texto);
        svgObject* obj = criarSVGObject(TEXT, *idnegativo, t, NULL, NULL, NULL);
        adicionarElemento(raiz, obj);
        (*idnegativo)--;
        return obj;
    }

    return NULL;

}

void processaComandoQRY(char* comando, treeNode* raiz, FILE *svgFile, FILE *respostaFile, char* DirComNomeBase){
    char tipo[10];
    sscanf(comando, "%s", tipo);

    if(safeStrlen(comando)<2 || comando[0] == '\0' || safeStrlen(comando)<2 || tipo[0] == '\0'){
        reportError(__func__, "Erro no comando/string recebida");
        return;
    }
        

    //As formas geométricas J e K se sobrepõe? Faça um quadrado em volta delas e se elas não se sobreporem, faça a linha tracejada
    if(strcmp(tipo, "o?") == 0){
        int j = -1, k = -1;
        sscanf(comando, "%*s %d %d", &j, &k);

        if(j == -1 || k == -1) 
            reportError(__func__, "Erro ao obter os ID");

        svgObject* jb = (buscarElemento(raiz, j)->objeto);
        svgObject* kb = (buscarElemento(raiz, k)->objeto);

        bool col = colide(jb, kb);
        Rectangle *rxt = envolveObjeto(jb, kb);
        svgObject* obj;

        if(col){
            //Linha cheia
            obj = criarSVGObject(RECTANGLE, 0, rxt, "green", "none", NULL);
            fprintf(respostaFile, "%sSIM\n\n", comando);

        }else{
            obj = criarSVGObject(RECTANGLE, 0, rxt, "green", "none", "stroke-dasharray: 2.5;");
            fprintf(respostaFile, "%sNÃO\n\n", comando);
        }

        adicionarSVGFile(obj, svgFile);
        liberarSVGobject(obj);

        return;
    }

    //O ponto X Y é interno a forma de ID j? Note que com interno, não pode ser == por que a borda não é inclusa
    // Coloque um pequeno círculo no XY; Pinte de verde se for interno, vermelho se for externo; Coloque uma linha ligando o ponto ao centro de J;
    if(strcmp(tipo, "i?") == 0){
        int id = -1; double x = -1, y = -1;
        sscanf(comando, "%*s %d %lf %lf", &id, &x, &y);

        if(id == -1 || x == -1 || y == -1) 
            reportError(__func__, "Erro ao obter os ID ou valor XY");

            svgObject* teste = (buscarElemento(raiz, id)->objeto);

            if(teste == NULL)
            reportError(__func__, "Incapaz de proceder: Busca retornou NULL");
        

            Point *cord = criarPonto(x,y);
            Circle *cr = criarCirculo(cord->x, cord->y, 1);


        if(teste->tipo == CIRCLE){
            Circle *c1 = teste->elemento;
            svgObject *respt;

            if(distanciaEntrePontos(cord, c1->coordenada) < c1->raio){
                //É interno
                respt = criarSVGObject(CIRCLE, 0, cr, "green", "green", NULL);
                fprintf(respostaFile, "%sSIM\n\n", comando);
            }else{
                respt = criarSVGObject(CIRCLE, 0, cr, "red", "red", NULL);
                fprintf(respostaFile, "%sNÃO\n\n", comando);
            }

            Line *lt = criarLinha(x, y, c1->coordenada->x, c1->coordenada->y);
            svgObject *lnrt = criarSVGObject(LINE, 0, lt, "black", "black", NULL);

            adicionarSVGFile(lnrt, svgFile);
            adicionarSVGFile(respt, svgFile);

            liberarSVGobject(lnrt);
            liberarSVGobject(respt);

        }else if(teste->tipo == RECTANGLE){
            Rectangle *rt1 = teste->elemento;

            bool intern = contidoNoIntervalo(x,rt1->coordenada->x, rt1->coordenada->x + rt1->width, false) 
                            && contidoNoIntervalo(y, rt1->coordenada->y, rt1->coordenada->y + rt1->height, false);

            svgObject *respt;

            if(intern){
                respt = criarSVGObject(CIRCLE, 0, cr, "green", "green", NULL);
                fprintf(respostaFile, "%sSIM\n\n", comando);
            }else{
                respt = criarSVGObject(CIRCLE, 0, cr, "red", "red", NULL);
                 fprintf(respostaFile, "%sNÃO\n\n", comando);
            }
            
            Point *centroDeMassa = obterCentroDeMassa(teste);
            Line *lt = criarLinha(cord->x, cord->y, centroDeMassa->x, centroDeMassa->y);
            svgObject *lnrt = criarSVGObject(LINE, 0, lt, "black", "black", NULL);

            adicionarSVGFile(lnrt, svgFile);
            adicionarSVGFile(respt, svgFile);

            liberarSVGobject(lnrt);
            liberarSVGobject(respt);
            liberarPoint(centroDeMassa);

            return;


        }else{
            reportError(__func__, "Caso de teste `i?`: Tipo não compatível");
        }

        liberarPoint(cord);


    }


    //Qual a distância entre os centros de massas? Escrever a distância proximo da metade da linha
    if(strcmp(tipo, "d?") == 0){
        int id1 = -1, id2 = -1;
        sscanf(comando, "%*s %d %d", &id1, &id2);

        if(id1 == -1 || id2 == -1) 
            reportError(__func__, "Erro ao obter os ID");

        svgObject* jb = (buscarElemento(raiz, id1)->objeto);
        svgObject* kb = (buscarElemento(raiz, id2)->objeto);

        if(kb == NULL || kb == NULL){
            reportError(__func__, "Incapaz de proceder: Busca retornou NULL");
        }

        Point *p1 = obterCentroDeMassa(jb);
        Point *p2 = obterCentroDeMassa(kb);

        double dist = distanciaEntrePontos(p1, p2);

        fprintf(respostaFile, "%s%lf\n\n", comando, dist);

        //Traçar uma reta entre os centros e escrever o valor na metade da linha
        Line *lt = criarLinha(p1->x, p1->y, p2->x, p2->y);

        char* textdist = malloc(sizeof(char)*30);
        sprintf(textdist, "%lf", dist);
        double dp1 = ((p2->x - p1->x)) /2;
        double dp2 = ((p2->y - p1->y)) /2;

        Text *tt = criarTexto(p1->x + dp1, p1->y + dp2, textdist);

        svgObject *ox1, *ox2;

        ox1 = criarSVGObject(LINE, 0, lt, "black", "black", NULL);
        ox2 = criarSVGObject(TEXT, 0, tt, "black", "black", NULL);

        adicionarSVGFile(ox1, svgFile);
        adicionarSVGFile(ox2, svgFile);     

        liberarPoint(p1);
        liberarPoint(p2);
        liberarSVGobject(ox1);
        liberarSVGobject(ox2);

        return;
    }

    //Criar novo SVG, para cada circulo crie um retangulo que envolva-o, para cada retangulo crie uma elipse dentro dele;
    //o fill deve ser o param cor, o nome do arquivo deve ser nomebase-sufixo.svg
    if(strcmp(tipo, "bb") == 0){
        char sufixo[32], cor[32];
        sscanf(comando, "%*s %s %s", sufixo, cor);


        char stpath[64];
        sprintf(stpath, "%s-%s.svg", DirComNomeBase, sufixo);

        iniciarSVGFILE(stpath);
        FILE *sfxsvg = abrirArquivo(stpath, APPEND);

        processamentoExterno(raiz, sfxsvg, cor, processamentoComandoBB);
        fclose(sfxsvg);
        finalizarSVGFILE(stpath);
        return;
    }




}

void processamentoComandoBB(treeNode *nd, FILE *svg, char* cor){
    svgObject *obj = nd->objeto;    
    adicionarSVGFile(obj, svg);
    
    if(obj->tipo == CIRCLE){
        Circle *rx = obj->elemento;
        Rectangle *rxt = boundingBoxCIRCLE(rx);
        svgObject *res = criarSVGObject(RECTANGLE, 0, rxt, cor, "none", NULL);
        adicionarSVGFile(res, svg);
        liberarSVGobject(res);

    }else if(obj->tipo == RECTANGLE){
        Ellipse *elpt = moldarRetangulo(obj);
        svgObject *res = criarSVGObject(ELLIPSE, 0, elpt, cor, "none", NULL);
        adicionarSVGFile(res, svg);
        liberarSVGobject(res);
    }
    
}

void processamentoComandoPRINT(treeNode* nd, FILE *svg){
    adicionarSVGFile(nd->objeto, svg);
}

void handleGeo(FILE* file, treeNode* raiz, char* path){
    char buffer[256];
    int* idnegativo = intToPointer(-1);

    iniciarSVGFILE(path);
    FILE *fl = abrirArquivo(path, APPEND);

    while(!feof(file)){
        fgets(buffer, 256, file);
        svgObject *f = processaComandoGEO(buffer, raiz, idnegativo);
        if(f != NULL)
            adicionarSVGFile(f, fl);
    }

    fclose(fl);
    finalizarSVGFILE(path);  
    free(idnegativo);

}

void handleQry(FILE* file, treeNode* raiz, char* pathSVG, char* pathTXT, char* nomeBase){
    char buffer[256];

    iniciarSVGFILE(pathSVG);
    FILE *fl = abrirArquivo(pathSVG, APPEND);
    FILE *txt = abrirArquivo(pathTXT, WRITE);

    treeToPrint(raiz, fl, processamentoComandoPRINT);

    while(true){
        fgets(buffer, 256, file);
        if(feof(file))
            break;
        processaComandoQRY(buffer, raiz, fl, txt, nomeBase);
    }


    fclose(fl);
    fclose(txt);
    finalizarSVGFILE(pathSVG);  

}

