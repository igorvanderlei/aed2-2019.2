#include "rb.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
        int opcao;

       no * arvore=NULL;


        while(1==1) {
            scanf("%d", &opcao);
            switch(opcao) {
                int n;
                case 1:
                     scanf("%d", &n);
                     inserir(&arvore, n);                    
                     break;
                case 2:
                    preorder(arvore);
                    printf("\n");                    
                    break;

                case 999:
                    exit(0);
            }

        }

}
