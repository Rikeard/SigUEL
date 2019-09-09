#include "header/geometry.h"
#include "header/main.h"
#include "header/argHandler.h"
#include "header/fileHandler.h"
#include "header/fileMaker.h"
#include "sys/time.h"

long long current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    //printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

int main(int argc, char *argv[]){
    defaultInit();

    long long base, last;

    char* diretorioEntrada = NULL, *diretorioSaida = NULL, *arquivoEntrada = NULL, *arquivoConsulta = NULL;
    bool processarQry = false, processarGeo = true;

    //Processamento dos argumento e padronização dos diretórios terminando em /
    argumentoInicial(argc, argv, &diretorioEntrada, &diretorioSaida, &arquivoEntrada, &arquivoConsulta);
    if(arquivoConsulta != NULL)
        processarQry = true;

    if(arquivoEntrada == NULL)
        processarGeo = false;
    
    Runtime raiz = runTime_new();
    
    {
        char* fill = calloc(10, sizeof(char));
        char* borda = calloc(10, sizeof(char));
        double espessura = -1;
        sprintf(fill, "blue"); 
        sprintf(borda, "white");
        runTime_setStyle(raiz, S_PREDIO, fill, borda, NULL, 1);
        borda = calloc(10, sizeof(char));
        fill = calloc(10, sizeof(char));
        sprintf(fill, "red");
        sprintf(borda, "red"); 
        runTime_setStyle(raiz, S_MURO, fill, borda, NULL, 3);
    }

    //Obtem os nomes dos arquivos passados como .qry e .geo (Sem paths e extensão);
    char *nomeEntrada = obterNome(arquivoEntrada), *nomeConsulta = obterNome(arquivoConsulta);
    printf("\nNome de entrada: %s\nNome de consulta: %s\n", nomeEntrada, nomeConsulta);

    printf("\nProcessamento interno:\n");

    //Monta os caminhos de diretório e arquivos usados para processar o .geo
    char* fullpathGEO = NULL, *fullpathSVG = NULL;
    if(processarGeo)
        montagemDePathGEO(diretorioEntrada, diretorioSaida, arquivoEntrada, nomeEntrada, &fullpathGEO, &fullpathSVG); 

    //Monta os caminhos de diretório e arquivos usados para processar o .qry
     char* fullpathQRY = NULL, *fullpathSVG2 = NULL, *fullpathTXT = NULL, *fullpathBB = NULL;   
    if(processarQry)
        montagemDePathQRY(diretorioEntrada, diretorioSaida, arquivoConsulta, nomeEntrada, nomeConsulta, &fullpathQRY, &fullpathSVG2, &fullpathTXT, &fullpathBB);
    else
        printf("\n    Processamento de .qry não iniciado/solicitado\n\n");
    


    printf("\nIniciando processamento:\n");

    base = current_timestamp();
    //Inicia o processamento .geo
    FILE* f = abrirArquivo(fullpathGEO, READONLY);
    handleGeo(f, raiz, fullpathSVG);
    fclose(f);

    printf("    Processamento .geo concluído\n");
    last = current_timestamp();
    printf("    Concluído em %lldms\n\n", last-base);
    base = current_timestamp();

    //Verifica se foi o programa possui um arquivo .qry válido para iniciar o processamento do mesmo.
    if(processarQry){
        FILE* f2 = abrirArquivo(fullpathQRY, READONLY);
        handleQry(f2, raiz, fullpathSVG2, fullpathTXT, fullpathBB);
        fclose(f2);
        printf("    Processamento .qry concluído\n");
        last = current_timestamp();
        printf("    Concluído em %lldms\n\n", last-base);
    }
   
    printf("    Processando desalocação de memória\n\n");

    //Desalocação de memória do programa

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





