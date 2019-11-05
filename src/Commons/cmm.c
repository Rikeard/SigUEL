#include "header/cmm.h"

void reportError(const char* funcao, char* error){
    if(error != NULL){
        printf("Erro em %s: %s\n", funcao, error);
    }else{
        printf("Erro em %s: Erro não especificado\n", funcao);
    }
}

char* safeString(char* str, int size){
    if(str == NULL){
        return NULL;
    }

    char* saf = calloc(size, sizeof(char));
    sprintf(saf, "%s", str);
    free(str);
    return saf;
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

int safeStrlen(char* str){
    if(str != NULL){
        return strlen(str);
    }else{
        return 0;
    }
}

char* nullToSpace(char* ss){
    if(ss == NULL){
        return "";
    }else{
        return ss;
    }
}

bool stringEquals(char* s1, char* s2){
    if(s1 == NULL || s2 == NULL){
        return false;
    }
    if(strcmp(s1, s2) == 0){
        return true;
    }else{
        return false;
    }
}

void safeFree(void* elemen){
    if(elemen != NULL){
        free(elemen);
    }
}


