#include <stdio.h>
#include <stdlib.h>
#include "bst.h"


int main(int argc, char*  argv[]) {
	no * arvore;
    int opcao;

	arvore = NULL;

    while(1==1) {
        scanf("%d", &opcao);
        int valor;
        switch(opcao) {
            case 1:
                scanf("%d", &valor);
                arvore = inserir(valor, arvore);            
                break;
            case 2:
                pre_order(arvore);
                break;
            case 999:
                exit(0);

        }


    }
	
	return 0;
}
