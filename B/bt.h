#ifndef BT_H
#define BT_H

#define ORDEM 5
#define NIL -1
#define SIM 1
#define NAO 0

typedef struct  pagina_bt{
	int quantidade;
	int chaves[ORDEM -1];
	int filhos[ORDEM]; //Armazena o RRN das páginas filhas ou -1
} pagina_bt;


typedef struct bt {
	int raiz;
	FILE *arquivo;
	FILE *dados;
} bt;

void inserir(bt * arvore, int key);
int inserir_aux(bt *arvore, int key,  int rrn, int *promo_rrn, int * promo_key);
void split(bt *arvore, pagina_bt *pagina, int rrn, int *promo_key, int *promo_rrn);
int procurar_pagina(pagina_bt *pagina, int key);
void criar_raiz(bt *arvore, int key, int fe, int fd);


int procurar(bt *arvore, int key, int rrn, int *rrn_encontrado, 
int *pos_encontrada);

pagina_bt * ler_pagina(bt *arvore, int rrn);
void escrever_pagina(bt *arvore, pagina_bt * pagina );
void abrir_arquivo(bt *arvore, char *arquivo);

void fechar_arquivo(bt *arvore);

pagina_bt * inicializar_pagina();
void limpar_pagina(pagina_bt *pagina);
void inserir_pagina(int key, int fd, pagina_bt * pagina);
void atualizar_pagina(bt * arvore, pagina_bt *pagina, int rrn);
int get_pagina(bt *arvore);

void imprimir(bt *arvore, int rrn);
void exibir_arquivo (bt *arvore);

#endif
