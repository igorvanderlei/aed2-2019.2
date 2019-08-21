#include <stdlib.h>
#include <stdio.h>
#include "bst.h"


no* criar_no(int valor){
	no* novo = (no *) malloc(sizeof(no));
	novo->valor = valor;
	novo->esq = NULL;
	novo->dir = NULL;
	return novo;
}

no* inserir(int valor, no* raiz) {
	if(raiz == NULL) {
		return criar_no(valor);
	} else {
		if(valor > raiz->valor) {
			raiz->dir = inserir(valor, raiz->dir);
		} else {
			raiz->esq = inserir(valor, raiz->esq);	
		}
		return raiz;
	}
}

void pre_order(no* arvore) {
	if(arvore != NULL) {
		printf("[%d]", arvore->valor);
		pre_order(arvore->esq);
		pre_order(arvore->dir);
	}
}
