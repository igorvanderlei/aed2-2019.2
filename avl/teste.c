#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
        int opcao;

       no * arvore=NULL;


        while(1==1) {
            scanf("%d", &opcao);
            switch(opcao) {
                int n;
                int cresceu;
                case 1:
                    
                     scanf("%d", &n);
                     arvore = inserir(arvore, n, &cresceu);        
                     break;
                case 2:
                    preorder(arvore);
                    printf("\n");                    
                    break;
                case 3:
                    inorder(arvore);                    
                    printf("\n");
                    break;
                case 4:
                    posorder(arvore);                    
                    printf("\n");
                    break;
                case 5:
                    scanf("%d", &n);
                    arvore = remover(arvore, n);
                    break;
                case 999:
                    exit(0);
            }

        }

}
