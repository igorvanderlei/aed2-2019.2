#include <stdio.h>
#include <stdlib.h>


int main(int argc, char * argv) {
    //Arquivo lógico
    FILE *fp;
    int a;
    int b;    
    //Abertura de um arquivo para escrita no modo binário
    //se o arquivo não existe ele será criado
    //se o arquivo já existe, seu conteúdo será apagado
    //modo w
    fp = fopen("teste.txt", "rb");

    if(fp != NULL) {
        //escrever no modo texto
        fseek(fp, sizeof(int), SEEK_SET);
        fread(&a, sizeof(int), 1, fp);


        //fechar o arquivo
        fclose(fp);


        printf("%d\n", a);

    }


}

