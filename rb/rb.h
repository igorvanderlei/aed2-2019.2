#ifndef RB_H
#define RB_H

enum cor {VERMELHO, PRETO};


typedef  int  tipo_dado;

typedef struct no {
    enum cor c;
    tipo_dado valor;
    struct no *esq, *dir, *pai;
} no;


void inserir (no** raiz, tipo_dado elemento);
void preorder(no* raiz);

int cor(no* no);
int eh_esquerdo(no* no);
int eh_raiz(no* no);

void ajustar(no* n, no** raiz);
no* tio (no* n);




#endif
