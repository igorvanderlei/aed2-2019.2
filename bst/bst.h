#ifndef BST_H
#define BST_H

typedef struct no {
	int valor;
	struct no* esq;
	struct no* dir;
} no;

no* criar_no(int valor);

no* inserir(int valor, no* raiz);

void pre_order(no* arvore);


#endif
