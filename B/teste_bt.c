#include <stdio.h>
#include <stdlib.h>
#include "bt.h"

int main(int argc, char * argv[]) {
	bt bt;
	int opcao;
	abrir_arquivo(&bt, "arvore.dat");

	while(1) {
		scanf("%d", &opcao);

		switch(opcao) {
				int valor, rrn, posicao;
				case 1:
						imprimir(&bt, bt.raiz);
						break;
				case 2: 
					scanf("%d", &valor);
					inserir(&bt, valor);
					break;
				case 3: 
					exibir_arquivo(&bt);
					break;
				case 4:
					scanf("%d", &valor);
					if(procurar(&bt, valor, bt.raiz, &rrn, &posicao))
						printf("Elemento encontrado na página %d, posição %d.\n", rrn, posicao);
					else
						printf("Elemento não encontrado\n");
					break;					
				case 99:
						fechar_arquivo(&bt);
						exit(0);
		}
	}
}
