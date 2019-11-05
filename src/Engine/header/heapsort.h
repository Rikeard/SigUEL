#ifndef HEAPSORT_H
#define HEAPSORT_H

void heapMenor(void* a[], int tamanhoLista, int k, double (*getValue)(void* value));

void heapMaior(void* a[], int tamanhoLista, int k, double (*getValue)(void* value));

#endif