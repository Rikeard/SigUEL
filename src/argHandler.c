#include "header/argHandler.h"


char* obterNome(char* arquivoEntrada){
    if(arquivoEntrada == NULL)
        return NULL;

    int endpoint, startpoint = 0;
    for(int i = safeStrlen(arquivoEntrada)-1; i >= 0; i--){
        if(arquivoEntrada[i]=='.')
            endpoint = i;

         if(arquivoEntrada[i]=='/'){
             startpoint = i+1;
             break;
         }
    }

    char *batata = calloc(100, sizeof(char));
    strncpy(batata, arquivoEntrada+startpoint, endpoint-startpoint);
    char *def = malloc((sizeof(char)*(safeStrlen(batata)+3)));
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
            if(current[safeStrlen(current)-1]=='/'){
                *diretorioEntrada = malloc(sizeof(char)*(safeStrlen(current)+1));
                sprintf(*diretorioEntrada, "%s", current);
                //strcpy(*diretorioEntrada, current);
            }else{
                *diretorioEntrada = malloc((sizeof(char)*(safeStrlen(current)+1)));
                sprintf(*diretorioEntrada, "%s/", current);
            }

            continue;
        }

        //Arquivo de entrada
        if(strcmp(current, "-f")==0){
            i++;
            *arquivoEntrada = malloc(sizeof(char)*(safeStrlen(argv[i])+1));
            sprintf(*arquivoEntrada, "%s", argv[i]);
            //strcpy(*arquivoEntrada, argv[i]);
            continue;
        }

        // //Arquivo de consulta
        if(strcmp(current, "-q")==0){
            i++;
            *arquivoConsulta = malloc(sizeof(char)*(safeStrlen(argv[i])+1));
            sprintf(*arquivoConsulta, "%s", argv[i]);
            //strcpy(*arquivoConsulta, argv[i]);
            continue;
        }

        // //Diretório de saida (opcional)
        if(strcmp(current, "-o")==0){
             i++;

            current = argv[i];

            //Tratamento de /
            if(current[safeStrlen(current)-1]=='/'){
                *diretorioSaida = malloc(sizeof(char)*(1+safeStrlen(current)));
                //strcpy(*diretorioSaida, current);
                sprintf(*diretorioSaida, "%s", current);
            }else{
                *diretorioSaida = malloc((sizeof(char)*(safeStrlen(current)+1)));
                sprintf(*diretorioSaida, "%s/", current);
            }

            continue;
        }


    }

    printf("Diretório de entrada (-e): %s\nArquivo de entrada (-f): %s\nArquivo de consulta (-q): %s\nDiretório de saída (-o): %s\n", *diretorioEntrada, *arquivoEntrada, *arquivoConsulta, *diretorioSaida);
}

void montagemDePathGEO(char* diretorioEntrada, char* diretorioSaida, char* arquivoEntrada, char *nomeEntrada, char** fullpathGEO, char **fullpathSVG){
        printf("    Processamento de GEO:\n");

        *fullpathGEO = malloc((safeStrlen(diretorioEntrada)+safeStrlen(arquivoEntrada)+10)*sizeof(char));
        sprintf(*fullpathGEO,"%s%s", nullToSpace(diretorioEntrada), nullToSpace(arquivoEntrada));
        printf("        Caminho de arquivo .geo: %s\n", *fullpathGEO);

        *fullpathSVG = malloc((safeStrlen(diretorioSaida)+safeStrlen(nomeEntrada)+10)*sizeof(char));
        sprintf(*fullpathSVG,"%s%s.svg", nullToSpace(diretorioSaida), nullToSpace(nomeEntrada));
        printf("        Caminho de arquivo .svg: %s\n", *fullpathSVG);    
}

void montagemDePathQRY(char* diretorioEntrada, char* diretorioSaida, char* arquivoConsulta, char* nomeEntrada, char* nomeConsulta, char** fullpathQRY, char** fullpathSVG2, char** fullpathTXT, char** fullpathBB){
     printf("\n    Processamento QRY:\n");

        *fullpathQRY = malloc((safeStrlen(diretorioEntrada)+safeStrlen(arquivoConsulta)+10)*sizeof(char));
        sprintf(*fullpathQRY,"%s%s", nullToSpace(diretorioEntrada), nullToSpace(arquivoConsulta));
        printf("        Caminho de arquivo .qry: %s\n", *fullpathQRY);

        *fullpathSVG2 = malloc((safeStrlen(diretorioSaida)+safeStrlen(nomeEntrada)+10+safeStrlen(nomeConsulta))*sizeof(char));
        sprintf(*fullpathSVG2,"%s%s-%s.svg", nullToSpace(diretorioSaida), nullToSpace(nomeEntrada), nullToSpace(nomeConsulta));
        printf("        Caminho de arquivo .svg - qry: %s\n", *fullpathSVG2);

        *fullpathTXT = malloc((safeStrlen(diretorioSaida)+safeStrlen(nomeEntrada)+10+safeStrlen(nomeConsulta))*sizeof(char));
        sprintf(*fullpathTXT,"%s%s-%s.txt", nullToSpace(diretorioSaida), nullToSpace(nomeEntrada), nullToSpace(nomeConsulta));
        printf("        Caminho de arquivo .txt - qry: %s\n", *fullpathTXT);

        *fullpathBB = malloc((safeStrlen(diretorioSaida)+safeStrlen(nomeConsulta)+10+safeStrlen(nomeEntrada))*sizeof(char));
        sprintf(*fullpathBB,"%s%s-%s", nullToSpace(diretorioSaida), nullToSpace(nomeEntrada), nullToSpace(nomeConsulta));
}