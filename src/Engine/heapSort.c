#include "header/heapsort.h"

void heapMenor(void* a[], int n, int valor, double (*getValue)(void* value)) {
   int i = n / 2, pai, filho, div;
   div = n/2 + valor;
   int cont = 0;
   void* t;
	
   while(1) {
      if (i > 0) {
          i--;
          t = a[i];
      } else {
          n--;
          if (n == 0) return;
          t = a[n];
          a[n] = a[0];
      }
      pai = i;
      filho = i * 2 + 1;
      while (filho < n) {
          if ((filho + 1 < n)  &&  (getValue(a[filho + 1]) < getValue(a[filho])))
              filho++;
          if (getValue(a[filho]) < getValue(t)) {
             a[pai] = a[filho];
             pai = filho;
             filho = pai * 2 + 1;
          } else {
             break;
          }
      }
      a[pai] = t;
      cont++;
      if(cont == div) return;
   }     
}

void heapMaior(void* a[], int n, int valor, double (*getValue)(void* value)) {
   int i = n / 2, pai, filho, div;
   div = n/2 + valor;
   int cont = 0;
   void* t;
	
   while(1) {
      if (i > 0) {
          i--;
          t = a[i];
      } else {
          n--;
          if (n == 0) return;
          t = a[n];
          a[n] = a[0];
      }
      pai = i;
      filho = i * 2 + 1;
      while (filho < n) {
          if ((filho + 1 < n)  &&  (getValue(a[filho + 1]) > getValue(a[filho])))
              filho++;
          if (getValue(a[filho]) > getValue(t)) {
             a[pai] = a[filho];
             pai = filho;
             filho = pai * 2 + 1;
          } else {
             break;
          }
      }
      a[pai] = t;
      cont++;
      if(cont == div) return;
   }     
}