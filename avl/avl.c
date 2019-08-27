#include "avl.h"
#include <stdio.h>
#include <stdlib.h>




no * inserir (no* raiz, tipo_dado elemento, int *cresceu){
    if(raiz == NULL) {
        no *novo = (no *) malloc(sizeof(struct no));
        novo->valor = elemento;
        novo->esq = NULL;
        novo->dir = NULL;
        novo->fb = 0;
        *cresceu = 1;
        return novo;
    } else {
           if(elemento > raiz->valor) {
                raiz->dir = inserir(raiz->dir, elemento, cresceu);
                if(*cresceu) {
                       switch(raiz->fb) {
                            case -1:
                                 raiz->fb = 0;
                                 *cresceu = 0;
                                 break;
                            case 0:
                                raiz->fb = 1;
                                 *cresceu = 1;
                                 break;
                            case 1:
                                //return rotacionar(raiz);
                                raiz->fb = 2;
                                *cresceu = 0;
                                break;
                       }

                }
                
           } else {
                raiz->esq = inserir(raiz->esq, elemento, cresceu);
                if(*cresceu) {
                       switch(raiz->fb) {
                            case -1:
                                 raiz->fb = -2;
                                 *cresceu = 0;
                                 return rotacionar(raiz);
                            case 0:
                                raiz->fb = -1;
                                 *cresceu = 1;
                                 break;
                            case 1:
                                //return rotacionar(raiz);
                                raiz->fb = 0;
                                *cresceu = 0;
                                break;
                       }

                }


           }
           return raiz;
    }
}


no* rotacionar(no* pivo) {
        if(pivo->fb == -2 && pivo->esq->fb == -1) 
            return rotacao_simples_direita(pivo);
}

no* rotacao_simples_direita(no* pivo) {
       no* u = pivo->esq;
       u->fb = 0;
       pivo->fb = 0;
       pivo->esq = u->dir;
       u->dir = pivo;
       return u;
}


void preorder(no* raiz){
    if(raiz == NULL) ;
    else {
        printf("[%d, %d]", raiz->valor, raiz->fb);
        preorder(raiz->esq);
        preorder(raiz->dir);
    }
}


void inorder(no* raiz){
    if(raiz == NULL) ;
    else {
        inorder(raiz->esq);
        printf("[%d]", raiz->valor);
        inorder(raiz->dir);
    }
}

void posorder(no* raiz){
    if(raiz == NULL) ;
    else {
        posorder(raiz->esq);
        posorder(raiz->dir);
        printf("[%d]", raiz->valor);
    }
}


no* remover(no* raiz, tipo_dado elemento) {
	return raiz;
}

