#ifndef HEAPSORT_H
#define HEAPSORT_H

/*
    Heapsort crescente e decrescente
    *Trabalha apenas com um veteor padrão do C
*/

void heapMenor(void* a[], int tamanhoLista, int k, double (*getValue)(void* value));

void heapMaior(void* a[], int tamanhoLista, int k, double (*getValue)(void* value));

#endif