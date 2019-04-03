#include "header/argHandler.h"


char* obterNome(char* arquivoEntrada){
    if(arquivoEntrada == NULL)
        return NULL;

    int endpoint, startpoint = 0;
    for(int i = strlen(arquivoEntrada)-1; i >= 0; i--){
        if(arquivoEntrada[i]=='.')
            endpoint = i;

         if(arquivoEntrada[i]=='/'){
             startpoint = i+1;
             break;
         }
    }

    char *batata = calloc(100, sizeof(char));
    strncpy(batata, arquivoEntrada+startpoint, endpoint-startpoint);
    char *def = malloc((sizeof(char)*(strlen(batata)+3)));
    sprintf(def,"%s", batata);
    free(batata);
    return def;
}

void argumentoInicial(int argc, char *argv[], char** diretorioEntrada, char **diretorioSaida, char **arquivoEntrada, char **arquivoConsulta){
    for(int i = 1; i < argc; i++){
        char* current = argv[i];

        // //Diretório de entrada (Opcional)
        if(strcmp(current, "-e")==0){
            i++;

            current = argv[i];

            //Tratamento de /
            if(current[strlen(current)-1]=='/'){
                *diretorioEntrada = malloc(sizeof(char)*(strlen(current)+1));
                sprintf(*diretorioEntrada, "%s", current);
                //strcpy(*diretorioEntrada, current);
            }else{
                *diretorioEntrada = malloc((sizeof(char)*(strlen(current)+1)));
                sprintf(*diretorioEntrada, "%s/", current);
            }

            continue;
        }

        //Arquivo de entrada
        if(strcmp(current, "-f")==0){
            i++;
            *arquivoEntrada = malloc(sizeof(char)*(strlen(argv[i])+1));
            sprintf(*arquivoEntrada, "%s", argv[i]);
            //strcpy(*arquivoEntrada, argv[i]);
            continue;
        }

        // //Arquivo de consulta
        if(strcmp(current, "-q")==0){
            i++;
            *arquivoConsulta = malloc(sizeof(char)*(strlen(argv[i])+1));
            sprintf(*arquivoConsulta, "%s", argv[i]);
            //strcpy(*arquivoConsulta, argv[i]);
            continue;
        }

        // //Diretório de saida (opcional)
        if(strcmp(current, "-o")==0){
             i++;

            current = argv[i];

            //Tratamento de /
            if(current[strlen(current)-1]=='/'){
                *diretorioSaida = malloc(sizeof(char)*(1+strlen(current)));
                //strcpy(*diretorioSaida, current);
                sprintf(*diretorioSaida, "%s", current);
            }else{
                *diretorioSaida = malloc((sizeof(char)*(strlen(current)+1)));
                sprintf(*diretorioSaida, "%s/", current);
            }

            continue;
        }


    }

    printf("Diretório de entrada: %s\nArquivo de entrada: %s\nArquivo de consulta: %s\nDiretório de saída: %s\n", *diretorioEntrada, *arquivoEntrada, *arquivoConsulta, *diretorioSaida);
}