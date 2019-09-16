#include "rb.h"
#include <stdio.h>
#include <stdlib.h>

void inserir (no** raiz, tipo_dado elemento) {
    no *temp, *pai, *novo;
    temp = *raiz;
    pai = NULL;
    
    while(temp != NULL) {
        pai = temp;
        if(elemento > temp->valor) {
            temp = temp->dir;
        } else {
            temp = temp->esq;
        }
    }

    novo = (no *) malloc(sizeof(struct no));
    novo->valor = elemento;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->c = VERMELHO;
    novo->pai = pai;

    if(*raiz != NULL) {
        if(elemento > pai->valor) {
            pai->dir = novo;
        } else {
            pai->esq = novo;
        }
    } else {
        *raiz = novo;
    }
    ajustar(novo, raiz);
}


void preorder(no* raiz){
    if(raiz == NULL) ;
    else {
        if(raiz->c == PRETO)
    		printf("\x1b[30m[%d]\x1b[0m", raiz->valor);
        else
    		printf("\x1b[31m[%d]\x1b[0m", raiz->valor);            
        preorder(raiz->esq);
        preorder(raiz->dir);
    }
}

int cor(no* no) {
    if(no == NULL || no->c == PRETO)
        return PRETO;
    else 
        return VERMELHO;
}


int eh_esquerdo(no* no) {
    if(no == no->pai->esq) 
        return 1;
    else
        return 0;
}



int eh_raiz(no* no) {
    if(no->pai == NULL) 
        return 1;
    else
        return 0;
}

no* tio (no* n) {
    if(eh_esquerdo(n->pai))
        return n->pai->pai->dir;
    else
        return n->pai->pai->esq;   
}


void ajustar(no* n, no **raiz) {
    //Caso 0,5
    if(eh_raiz(n)) {
        n->c = PRETO;
        return;
    }
    
    //
    if(n->pai->c == VERMELHO) {
           //caso 1 o tio é vermelho, baixa o preto do avo
           if(cor(tio(n)) == VERMELHO) {
                n->pai->c = PRETO;
                tio(n)->c = PRETO;
                n->pai->pai->c = VERMELHO;
                ajustar(n->pai->pai, raiz);
                return;
           } else { //Tio preto
                   //Caso 2 - esq
//                if(eh_esquerdo(n) && eh_esquerdo(n->pai)) {
//                    rotacao_simples_direita(n->pai->pai, raiz);
                    //NÃO CHAMA O AJUSTAR => ajustar(n->pai->pai, raiz);
                }


            }
         


    }
}




void remover (no** raiz, tipo_dado elemento) {
    no *temp, *pai, *novo;
    temp = *raiz;
    pai = NULL;
    

    //Percorre a arvore até achar o elemento ou null
    while(temp != NULL && temp->valor != elemento) {
        pai = temp;
        if(elemento > temp->valor) {
            temp = temp->dir;
        } else {
            temp = temp->esq;
        }
    }
    
    //Se o elemento não existe encerra a função
    if(temp == NULL)
          return ;

    //O elemento a ser removido tem dois filhos
    if(temp->esq != NULL && temp->dir != NULL) {
        temp->valor = maior(temp->esq);
        remover(&temp->esq, temp->valor);
        return ;
    }
    
    //Tem somente o filho esquerdo
    if(temp->esq != NULL && temp->dir == NULL) {
       *raiz = temp->esq;
        temp->cor = PRETO;
        return ;
    }
    
    //Fazer o simétrico para direita

    //Não tem filhos e é vermelho
    if(temp->cor == VERMELHO) {
        if(eh_esquerdo(temp)) {
            temp->pai->esq = NULL;
        } else {
            temp->pai->dir = NULL;  
        }
        return ;          
    }

    //Nao tem filhos e é preto - pode ser declarado e inicializado como global. Daqui ...
    no *no_null = (no *) malloc(sizeof(struct no));
    no_null->cor = DUPLO_PRETO;
    no_null->esq = NULL;
    no_null->dir = NULL;
    //... Até aqui 
    
    no_null->pai = temp->pai;
    if(eh_esquerdo(temp)) {
            temp->pai->esq = no_null;
        } else {
            temp->pai->dir = no_null;  
    }
       
    reajustar(raiz, no_null);
}

void   reajustar(no** raiz, no* n) {
    //Verificação do caso que se enquadra e chamar a função
    //Caso 1 - duplo preto é raiz
    if(eh_raiz(n)) {
        if(n == no_null) {
            *raiz = NULL;

        } else {
            n->cor = PRETO;
        }
        return ;
    }

    //Caso 2 - Esq


}




