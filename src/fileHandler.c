#include "header/fileHandler.h"

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

void processaComandoGeo(char* comando, Runtime rt){
    char *tipo = calloc(10, sizeof(char));
    sscanf(comando, "%s", tipo);
    
    if(stringEquals(tipo, "nx")){
        geo_Nx(comando, rt);
    }

    if(stringEquals(tipo, "c")){
        geo_C(comando, runTime_getLista(rt, L_GERAL), rt);
    }

    if(stringEquals(tipo, "r")){
        geo_R(comando, runTime_getLista(rt, L_GERAL), rt);
    }

    if(stringEquals(tipo, "t")){
        geo_T(comando, runTime_getLista(rt, L_GERAL));
    }

    if(stringEquals(tipo, "q")){
        geo_Q(comando, runTime_getLista(rt, L_QUADRAS), rt);
    }

    if(stringEquals(tipo, "h")){
        geo_H(comando, runTime_getLista(rt, L_HIDRANTES), rt);
    }

    if(stringEquals(tipo, "s")){
        geo_S(comando, runTime_getLista(rt, L_SEMAFOROS), rt);
    }

    if(stringEquals(tipo, "rb")){
        geo_RB(comando, runTime_getLista(rt, L_RADIOS), rt);
    }

    if(stringEquals(tipo, "cq")){
        geo_CQ(comando, rt);
    }

    if(stringEquals(tipo, "cr")){
        geo_CR(comando, rt);
    }

    if(stringEquals(tipo, "cs")){
        geo_CS(comando, rt);
    }

    if(stringEquals(tipo, "ch")){
        geo_CH(comando, rt);
    }

    if(stringEquals(tipo, "sw")){
        geo_SW(comando, rt);
    }

    free(tipo);

}

void processaComandoQry(char* comando, Runtime rt, FILE *svgFile, FILE *respostaFile, char* DirComNomeBase){
    char *tipo = calloc(10, sizeof(char));
    sscanf(comando, "%s", tipo);

    if(safeStrlen(comando)<2 || comando[0] == '\0' || safeStrlen(comando)<2 || tipo[0] == '\0'){
        reportError(__func__, "Erro no comando/string recebida");
        return;
    }
        
    //As formas geométricas J e K se sobrepõe? Faça um quadrado em volta delas e se elas não se sobreporem, faça a linha tracejada
    if(stringEquals(tipo, "o?")){
        qry_O(comando, runTime_getLista(rt, L_GERAL), respostaFile, runTime_getLista(rt, L_QUERY));
    }

    //O ponto X Y é interno a forma de ID j? Note que com interno, não pode ser == por que a borda não é inclusa
    // Coloque um pequeno círculo no XY; Pinte de verde se for interno, vermelho se for externo; Coloque uma linha ligando o ponto ao centro de J;
    if(stringEquals(tipo, "i?")){
       qry_I(comando, runTime_getLista(rt, L_GERAL), respostaFile, runTime_getLista(rt, L_QUERY));
    }


    //Qual a distância entre os centros de massas? Escrever a distância proximo da metade da linha
    if(stringEquals(tipo, "d?")){
        qry_D(comando, runTime_getLista(rt, L_GERAL), respostaFile, runTime_getLista(rt, L_QUERY));
    }

    //Criar novo SVG, para cada circulo crie um retangulo que envolva-o, para cada retangulo crie uma elipse dentro dele;
    //o fill deve ser o param cor, o nome do arquivo deve ser nomebase-sufixo.svg
    if(stringEquals(tipo, "bb")){
        qry_BB(comando, runTime_getLista(rt, L_GERAL), DirComNomeBase);
    }

    if(stringEquals(tipo, "dq")){
        qry_DQ(comando, rt, respostaFile);    
    }

    if(stringEquals(tipo, "del")){
        qry_Del(comando, rt, respostaFile);
    }

    if(stringEquals(tipo, "cbq")){
        qry_CBQ(comando, runTime_getLista(rt, L_QUADRAS), respostaFile);
    }

    if(stringEquals(tipo, "crd?")){
        qry_CRD(comando, rt, respostaFile);
    }

    if(stringEquals(tipo, "trns")){
        qry_Trns(comando, rt, respostaFile);
    }


}

void listGeoToFile(void* elemento, void* argumento){
    if(elemento != NULL)
        adicionarSVGFile(elemento, argumento);
}

void listQuadraGeoToFile(void* elemento, void* argumento){
  if(elemento != NULL)
        adicionarSVGFile(Quadra_getElemento(elemento), argumento);
}

void listHidranteGeoToFile(void* elemento, void* argumento){
  if(elemento != NULL)
        adicionarSVGFile(Hidrante_getElemento(elemento), argumento);
}

void listSemaforoGeoToFile(void* elemento, void* argumento){
  if(elemento != NULL)
        adicionarSVGFile(Semaforo_getElemento(elemento), argumento);
}

void listRadioGeoToFile(void* elemento, void* argumento){
  if(elemento != NULL)
        adicionarSVGFile(Radio_getElemento(elemento), argumento);
}

void handleGeo(FILE* file, Runtime rt, char* path){
    char *buffer = calloc(256, sizeof(char));

    while(true){
        fgets(buffer, 255, file);
        if(feof(file))
            break;
        processaComandoGeo(buffer, rt);
        //listaStatic_stats(runTime_getLista(rt, L_GERAL));
    }

    iniciarSVGFILE(path);
    FILE *fl = abrirArquivo(path, APPEND);
    listaStatic_forEach(runTime_getLista(rt, L_GERAL), listGeoToFile, fl);
    listaStatic_forEach(runTime_getLista(rt, L_QUADRAS), listQuadraGeoToFile, fl);
    listaStatic_forEach(runTime_getLista(rt, L_HIDRANTES), listHidranteGeoToFile, fl);
    listaStatic_forEach(runTime_getLista(rt, L_SEMAFOROS), listSemaforoGeoToFile, fl);
    listaStatic_forEach(runTime_getLista(rt, L_RADIOS), listRadioGeoToFile, fl);


    fclose(fl);
    finalizarSVGFILE(path);  



    free(buffer);
}



void handleQry(FILE* file, Runtime rt, char* pathSVG, char* pathTXT, char* nomeBase){
    char *buffer = calloc(256, sizeof(char));

    iniciarSVGFILE(pathSVG);
    FILE *fl = abrirArquivo(pathSVG, APPEND);
    FILE *txt = abrirArquivo(pathTXT, WRITE);


    while(true){
        fgets(buffer, 256, file);
        if(feof(file))
            break;
        processaComandoQry(buffer, rt, fl, txt, nomeBase);
    }

    listaStatic_forEach(runTime_getLista(rt, L_GERAL), listGeoToFile, fl);
    listaStatic_forEach(runTime_getLista(rt, L_QUADRAS), listQuadraGeoToFile, fl);
    listaStatic_forEach(runTime_getLista(rt, L_HIDRANTES), listHidranteGeoToFile, fl);
    listaStatic_forEach(runTime_getLista(rt, L_SEMAFOROS), listSemaforoGeoToFile, fl);
    listaStatic_forEach(runTime_getLista(rt, L_RADIOS), listRadioGeoToFile, fl);

    listaStatic_forEach(runTime_getLista(rt, L_QUERY), listGeoToFile, fl);


    fclose(fl);
    fclose(txt);
    finalizarSVGFILE(pathSVG);  
    free(buffer);

}

