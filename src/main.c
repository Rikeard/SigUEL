#include "header/geometry.h"
#include "header/main.h"
#include "header/tree.h"
#include "header/svg.h"
#include "header/argHandler.h"
#include "header/fileHandler.h"
#include "header/fileMaker.h"

int main(int argc, char *argv[]){
    defaultInit();

    char* diretorioEntrada = NULL, *diretorioSaida = NULL, *arquivoEntrada = NULL, *arquivoConsulta = NULL;

    //Processamento dos argumento e padronização dos diretórios terminando em /
    argumentoInicial(argc, argv, &diretorioEntrada, &diretorioSaida, &arquivoEntrada, &arquivoConsulta);
    treeNode* raiz = inicializarArvore();

    char *nomeEntrada = obterNome(arquivoEntrada), *nomeConsulta = obterNome(arquivoConsulta);
    printf("\nNome de entrada: %s\nNome de consulta: %s\n", nomeEntrada, nomeConsulta);

    char* fullpathGEO = malloc((strlen(diretorioEntrada)+strlen(arquivoEntrada)+10)*sizeof(char));
    sprintf(fullpathGEO,"%s%s", diretorioEntrada, arquivoEntrada);

    char* fullpathSVG = malloc((strlen(diretorioSaida)+strlen(nomeEntrada)+10)*sizeof(char));
    sprintf(fullpathSVG,"%s%s.svg", diretorioSaida, nomeEntrada);

    char* fullpathQRY = malloc((strlen(diretorioEntrada)+strlen(arquivoConsulta)+10)*sizeof(char));
    sprintf(fullpathQRY,"%s%s", diretorioEntrada, arquivoConsulta);

    char* fullpathSVG2 = malloc((strlen(diretorioSaida)+strlen(nomeEntrada)+10+strlen(nomeConsulta))*sizeof(char));
    sprintf(fullpathSVG2,"%s%s-%s.svg", diretorioSaida, nomeEntrada, nomeConsulta);

    char* fullpathTXT = malloc((strlen(diretorioSaida)+strlen(nomeEntrada)+10+strlen(nomeConsulta))*sizeof(char));
    sprintf(fullpathTXT,"%s%s-%s.txt", diretorioSaida, nomeEntrada, nomeConsulta);

    char* fullpathBB = malloc((strlen(diretorioSaida)+strlen(nomeConsulta)+10+strlen(nomeEntrada))*sizeof(char));
    sprintf(fullpathBB,"%s%s-%s", diretorioSaida, nomeEntrada, nomeConsulta);

    FILE* f = abrirArquivo(fullpathGEO, READONLY);
    handleGeo(f, raiz, fullpathSVG);

    FILE* f2 = abrirArquivo(fullpathQRY, READONLY);
    handleQry(f2, raiz, fullpathSVG2, fullpathTXT, fullpathBB);

    fclose(f);
    fclose(f2);


    liberarArvore(raiz);

    free(diretorioEntrada);
    free(diretorioSaida);
    free(arquivoEntrada);
    free(arquivoConsulta);
    free(nomeEntrada);
    free(nomeConsulta);
    free(fullpathGEO);
    free(fullpathSVG);
    free(fullpathQRY);
    free(fullpathSVG2);
    free(fullpathTXT);
    free(fullpathBB);

    reportSucess(__func__);
    return 0;

}


void reportError(const char* funcao, char* error){
    if(error != NULL){
        printf("Erro em %s: %s\n", funcao, error);
    }else{
        printf("Erro em %s: Erro não especificado\n", funcao);
    }
}

void reportSucess(const char* funcao){
    printf("Sucesso em %s\n", funcao);
}

void defaultInit(){
    #if PLATAFORM == PLATFORM_UNIX
        printf("Inicializando código em sistema Linux\n\n");
    #elif PLATAFORM == PLATFORM_WINDOWS
        printf("Inicializando código em sistema Windows\n Não testado\n\n");
    #elif PLATAFORM == PLATAFORM_MAC
        printf("Inicializando código em sistema Mac\n Não testado\n\n");
    #endif   
}



