#include <stdio.h>
#include <stdlib.h>
#include "bt.h"

/* 	Função inserir:	Chamada principal do inserir.
		Parâmetros:
						arvore:	ponteiro para árvore que receberá a nova chave
					 		 key:	chave a ser inserida 
		Retorno:
							void */
void inserir(bt * arvore, int key) {
	int promoted, promo_key, promo_rrn;
	promoted = inserir_aux(arvore, key, arvore->raiz, &promo_rrn, &promo_key);
	/* Resolve os casos de inserir em uma árvore vazia 
     ou quando ocorre o split na antiga raiz*/	
	if(promoted)
	  criar_raiz(arvore, promo_key, arvore->raiz, promo_rrn);
}


/* 	Função inserir_aux:	Função recursiva que realiza de fato a inserção
												Partindo da raiz, inicialmente a função "desce" a árvore 
												pelo caminho apropriado até achar o nó vazio (NIL).
												No retorno da recursão, os valores(promo_key e promo_rrn) serão
												inseridos nas páginas seguindo o caminho reverso das chamadas 
												recursivas, se houver necessidade (falta de espaço para inserir no nó) 
												a função split será chamada e o nó do meio será passado para ser 
												inserido no seu pai através das variáveis promo_key e promo_rrn
		Parâmetros:
								arvore:	ponteiro para árvore que receberá a nova chave
					 				 key:	chave a ser inserida
									 rrn: página na qual a chave será inserida (começa com a raiz e a cada 
												chamada recursiva o valor é atualizado de acordo com o caminho tomado)
						 promo_rrn: ponteiro utilizado para retornar o valor do filho promovido
						 promo_key: ponteiro utilizado para retornar o valor da chave promovida
		Retorno:
									 int:	indica se houve ou não promoção na chamada recursiva do seu filho	 */

int inserir_aux(bt *arvore, int key,  int rrn, int *promo_rrn, int * promo_key) {
	pagina_bt *page,		   /*página atual */
			  *new_page;       /*página que será criada se ocorrer um split */

	int promoted; /* valor booleano */
	int posicao;  /* posicao dentro da página onde a chave deve ser inserida */

	if(rrn == NIL) {     /* passou da folha, utilizaremos os atributos promo_key para realizar a 														inserção da chave no retorno da função recursiva */
		*promo_key = key;
		*promo_rrn = NIL;  /* ao inserir na folha, o filho direito será nulo */
		return SIM; /* sinaliza que houve promoção */
	}
	
	page = ler_pagina(arvore, rrn);
	posicao = procurar_pagina(page, key);
	
	if(posicao < page->quantidade && page->chaves[posicao] == key) {
			//Erro!!! Tentativa de inserir uma chave duplicada
			free(page); //Não houve modificação na página, podemos liberar a memória sem salvar
			return NAO; //Não haverá promoção
	}
	
	//Empilhamento das chamadas recursivas, seguindo o caminho da posição encontrada
	promoted = inserir_aux(arvore, key, page->filhos[posicao], promo_rrn, promo_key);

	//Se não houve promoção do seu filho para cá, não haverá promoção deste nó para seus pais
	if(!promoted) {
			free(page);
			return NAO;
	}

	//Se existe espaço para uma nova chave
	if(page->quantidade < ORDEM -1) {
			inserir_pagina(*promo_key, *promo_rrn, page);
		  atualizar_pagina(arvore, page, rrn);
			free(page);
			return NAO; 
	} else {
			//split
			split(arvore, page, rrn, promo_key, promo_rrn);
			free(page);
			return SIM;
	}
}

/* 	Função inserir_pagina: Insere uma chave em uma página que possui espaço livre.
		Parâmetros:
							 			  key: Valor da chave que será inserida
											 fd: RRN do filho direito da chave que será inserida
									 pagina: Ponteiro para página na qual a chave será inserida
		Retorno:
											void */
void inserir_pagina(int key, int fd, pagina_bt * pagina) {
	int indice; 
	//Desloca os arrays (chaves e filhos) em uma posição à direita 
	//para abrir espaço de modo que se possa inserir a chave/filho mantendo 
	//o array de chaves ordenado
	for(indice = pagina->quantidade - 1; indice >= 0 
							&& key < pagina->chaves[indice] ; indice --) {

			pagina->chaves[indice+1] = pagina->chaves[indice];
			pagina->filhos[indice+2] = pagina->filhos[indice+1];
	}
	//Insere a chave/filho na posição correta
	pagina->chaves[indice+1] = key;
	pagina->filhos[indice+2] = fd;
	pagina->quantidade ++;
}


/* 	Função split:	Função auxiliar do inserir que é chamada quando uma chave precisa 
									ser inserida em uma página que não tem espaço. Serão utilizados arrays
									temporários com capacidade de acomodar a nova chave, em seguida a chave 
									é inserida no temporário, por fim será criada uma nova página. A metade 
									dos elementos fica na página original, outra metade vai para a nova página
									e o elemento do meio será promovido para a página pai. O elemento promovido
									é passado pelos ponteiros promo_key e promo_rrn.
		Parâmetros:
					arvore:	ponteiro para árvore que receberá a nova chave
		 			pagina:	página que será dividida
						 rrn: rrn da página que será dividida
			 promo_rrn: ponteiro utilizado para retornar o valor do filho promovido
			 promo_key: ponteiro utilizado para retornar o valor da chave promovida
		Retorno:
					  void */
void split(bt *arvore, pagina_bt *pagina, int rrn, int *promo_key, int *promo_rrn) {
		int chaves_temp[ORDEM];
		int filhos_temp[ORDEM +1];
		int indice, indice2;
		int meio;

		pagina_bt * nova;

		//Copia as chaves e filhos para os arrays de trabalho _temp
		for(indice = 0; indice < ORDEM-1; indice++) {
				chaves_temp[indice] = pagina->chaves[indice];
				filhos_temp[indice] = pagina->filhos[indice];
		}
		filhos_temp[indice] = pagina->filhos[indice];		

		//Insere em ordem o elemento promovido no array temporário
		for(indice = ORDEM - 2 ; indice >=0 
							&& *promo_key < pagina->chaves[indice]; indice --) {
				chaves_temp[indice+1] = pagina->chaves[indice];
				filhos_temp[indice+2] = pagina->filhos[indice+1];
		}
		chaves_temp[indice+1] = *promo_key;
		filhos_temp[indice+2] = *promo_rrn;

		//Atualiza o valor do elemento(chave e rrn) que será devolvido para a função inserir_aux
		//de modo que esta possa passar esses valores para a chamada anterior (aplicada ao pai deste nó)
		meio = (ORDEM -1) /2;
		*promo_key = chaves_temp[meio];
		*promo_rrn = get_pagina(arvore);
	
		//Copia a primeira metade das chaves e filhos de volta do array de trabalho para a página atual
		limpar_pagina(pagina);
		for(indice = 0; indice < meio; indice++) {
			pagina->chaves[indice] = chaves_temp[indice];
			pagina->filhos[indice] = filhos_temp[indice];
		}
		pagina->filhos[indice] = filhos_temp[indice];	
		pagina->quantidade = meio;

		//Copia a segunda metade das chaves e filhos para a nova página
		nova = inicializar_pagina();
		for(indice = meio+1, indice2=0; indice < ORDEM; indice++, indice2++) {
			nova->chaves[indice2] = chaves_temp[indice];
			nova->filhos[indice2] = filhos_temp[indice];
		}
		nova->filhos[indice2] = filhos_temp[indice];		
		nova->quantidade = ORDEM - meio - 1;

		//Atualiza a página atual no arquivo
	  atualizar_pagina(arvore, pagina, rrn);
		//Escreve a nova página no arquivo
		escrever_pagina(arvore, nova);

		//libera da memória a nova página
		free(nova);
}

/* 	Função procurar_pagina:	Procura (local - em apenas uma página) a posição de uma chave 
														no array de chaves da página. Se a chave não existir, será 
														retornada a posição do filho onde possivelmente a chave 
														será localizada.
		Parâmetros:
							 			pagina:	página que será dividida
											 rrn: rrn da página que será dividida
								 promo_rrn: ponteiro utilizado para retornar o valor do filho promovido
								 promo_key: ponteiro utilizado para retornar o valor da chave promovida
		Retorno:
											 int: posição da chave encontrada / posição do filho na qual a 
														chave pode ser encontrada  */
int procurar_pagina(pagina_bt *pagina, int key) {
	int indice;
	for(indice = 0; indice < pagina->quantidade && key > pagina->chaves[indice]; indice ++ )
	;
	return indice;
}

/* 	Função limpar_pagina:	Zera/Restaura os campos (quantidade, chaves e filhos) de 
													uma página para os valores padrão
		Parâmetros:
							 			pagina:	página que terá os valores "zerados" 
		Retorno:
											void */
void limpar_pagina(pagina_bt *pagina) {
	int indice;
	pagina->quantidade = 0;
	for(indice = 0; indice < ORDEM - 1; indice++) {
		pagina->chaves[indice] = NIL;
		pagina->filhos[indice] = NIL;
	}	
	pagina->filhos[indice] = NIL;
}

/* 	Função inicializar_pagina:	Aloca memória para uma nova página e inicializa seu campos com os 									valores padrão;
		Parâmetros:
		Retorno:
									 pagina_bt*: Ponteiro para nova página*/
pagina_bt * inicializar_pagina() {
	pagina_bt * novo = (pagina_bt *) malloc(sizeof(pagina_bt));
	limpar_pagina(novo);
	return novo;
}


/* 	Função atualizar_pagina:	Sobreescreve no arquivo uma página que foi modificada em memória
		Parâmetros:
										 arvore:  Ponteiro para árvore B
										 pagina:	Ponteiro para página que foi modificada na memória
												rrn:	RRN com a posição da página no arquivo
		Retorno:
											 void */
void atualizar_pagina(bt * arvore, pagina_bt *pagina, int rrn) {
	int deslocamento = sizeof(int) + (sizeof(pagina_bt) * rrn);
	fseek(arvore->arquivo, deslocamento, SEEK_SET);
	fwrite(pagina, sizeof(pagina_bt), 1, arvore->arquivo);

}

/* 	Função get_pagina:	Calcula e retorna o valor do RRN da próxima página a ser salva
		Parâmetros:
							 arvore:  Ponteiro para árvore B
		Retorno:
							    int:	Próximo RRN*/
int get_pagina(bt *arvore) {
	int tamanho, rrn;
	fseek(arvore->arquivo, 0L, SEEK_END);
	tamanho = ftell(arvore->arquivo);
    rrn = (tamanho - sizeof(int))/sizeof(pagina_bt);
	return rrn;
}

/* 	Função escrever_pagina:	Acrescenta uma nova página ao arquivo
		Parâmetros:
							 			arvore:	Ponteiro para árvore B
										pagina:	Ponteiro para a página
		Retorno:
							    		void  */
void escrever_pagina(bt *arvore, pagina_bt * pagina ) {
	fseek(arvore->arquivo, 0L, SEEK_END);
	fwrite(pagina, sizeof(pagina_bt), 1, arvore->arquivo);
}


/* 	Função criar_raiz: Cria (e salva no arquivo) uma nova página para armazenar a nova raiz.
		Parâmetros:
					 		 arvore: Ponteiro para árvore B
	 							  key: Chave da nova raiz
									 fe: RRN do filho esquerdo da nova raiz
									 fd: RRN do filho direito da nova raiz
		Retorno:
							   void  */
void criar_raiz(bt *arvore, int key, int fe, int fd) {
	pagina_bt * novo =	inicializar_pagina();
	novo->chaves[0] = key;
	novo->filhos[0] = fe;
	novo->filhos[1] = fd;
	novo->quantidade = 1;

	arvore->raiz = get_pagina(arvore);
	escrever_pagina(arvore, novo);
	//Escreve o RRN da nova raiz no arquivo
	fseek(arvore->arquivo, 0L, SEEK_SET);
	fwrite(&(arvore->raiz), sizeof(int), 1, arvore->arquivo);
    free(novo);

}

/* 	Função abrir_arquivo:	Abre o arquivo da árvore B e lê o valor da raiz. Caso o arquivo não 													exista, ele será criado.
		Parâmetros:
							 			arvore:	Ponteiro para árvore B
	 								 arquivo: Caminho para o arquivo
		Retorno:
							    		void  */
void abrir_arquivo(bt *arvore, char *arquivo){
		arvore->arquivo = fopen(arquivo, "r+b");
		//Se o arquivo existe leia o valor da raiz
		if(arvore->arquivo != NULL) {
				fread(&(arvore->raiz), sizeof(int), 1, arvore->arquivo);
		} else {
				//Se o arquivo não existe, crie o arquivo (com a opção w+b)
				arvore->raiz = NIL;
				arvore->arquivo = fopen(arquivo, "w+b");
				//Escreva o valor raiz (NIL) no novo arquivo
				fwrite(&(arvore->raiz), sizeof(int), 1, arvore->arquivo);

				//Fecha e reabre o arquivo para trabalhar no modo r+b
				fclose(arvore->arquivo);
				arvore->arquivo = fopen(arquivo, "r+b");				
	}			
}
/* 	Função fechar_arquivo:	Fecha o arquivo da árvore B.
		Parâmetros:
							 			arvore:	Ponteiro para árvore B
		Retorno:
							    		void  */
void fechar_arquivo(bt *arvore) {
	if(arvore->arquivo != NULL) {
		fclose(arvore->arquivo);
		arvore->arquivo = NULL;
		arvore->raiz = NIL;
	}
}

/* 	Função ler_pagina:	Carrega para a memória uma página específica da árvore, que está 
												armazenada no arquivo da árvore B.
		Parâmetros:
 				 			 arvore:	Ponteiro para árvore B
									rrn:  RRN da página que será carregada
									
		Retorno:
					 pagina_bt*:  Ponteiro para a página carregada em memória 
												ou NULL se o RRN não existir no arquivo  */
pagina_bt * ler_pagina(bt *arvore, int rrn) {
	int end_pagina;
	if(arvore->arquivo != NULL) {
		pagina_bt * pagina = (pagina_bt *) malloc (sizeof(pagina_bt));
		end_pagina = sizeof(int) + (rrn * sizeof(pagina_bt));
		fseek(arvore->arquivo, end_pagina, SEEK_SET);
		
		if(fread(pagina, sizeof(pagina_bt), 1, arvore->arquivo))
			return pagina;
		else {
			free(pagina);
			return NULL;
		}
	}
}

/* 	Função procurar:	Função recursiva que procura uma chave na árvore.
		Parâmetros:
 				 		 arvore:	Ponteiro para árvore B
							  key:	Chave procurada
								rrn:  RRN da página na qual a chave está sendo procurada (começa com a 
											raiz e a cada chamada recursiva o valor é atualizado de acordo 
											com o caminho tomado)									
		 rrn_encontrado:	Ponteiro utilizado para retornar o RRN da página onde a chave foi encontrada
		 pos_encontrada:	Ponteiro utilizado para retornar a posição no array de chaves onde a 
											chave foi encontrada
		Retorno:
					 			int:  Assume o valor SIM ou NAO, indicando se a chave foi encontrada*/
int  procurar(bt *arvore, int key, int rrn, int *rrn_encontrado, int *pos_encontrada) {
	int indice;
	int proximo_rrn;

	if(rrn == NIL) {
		return NAO; // não encontrado
	} else {
		//carrega a página rrn para a memória
		pagina_bt * page = ler_pagina(arvore, rrn);

		//procura a chave dentro da página
		for(indice = 0; indice < page->quantidade ; indice ++) {
			if(key == page->chaves[indice]) {
					*rrn_encontrado = rrn;
					*pos_encontrada = indice;
					return SIM;
			}
			if(key < page->chaves[indice])
					break;
		}
	
		proximo_rrn = page->filhos[indice];
		//*rrn_encontrado = rrn;

		free(page);

		return procurar(arvore, key, proximo_rrn, rrn_encontrado, pos_encontrada); 
	}
}

/* 	Função imprimir:	Imprime todas as páginas do arquivo impondo a organização 
											hierárquica da árvore
		Parâmetros:
 				 		 arvore:	Ponteiro para árvore B
								rrn:  RRN da página que está sendo impressa (começa com a 
											raiz e em seguida passa por cada um dos seus filhos, 
											e os filhos dos filhos, recursivamente)									
		Retorno:
					 		void */
void imprimir(bt *arvore, int rrn) {
	int i;
	if(rrn != NIL) {
		pagina_bt * page = ler_pagina(arvore, rrn);

		printf("[RRN:%2d, K:{", rrn);
		for(i = 0; i < ORDEM - 2; i++)
			printf("%3d, ", page->chaves[i]);
		printf("%3d  } F:{", page->chaves[i]);		
		for(i = 0; i < ORDEM - 1; i++)
			printf("%3d, ", page->filhos[i]);
		printf("%3d  }]\n", page->filhos[i]);		

		for(i = 0; i < ORDEM; i++)
			imprimir(arvore, page->filhos[i]);

		free(page);
	}
}


/* 	Função exibir_arquivo:	Imprime todas as páginas do arquivo sequencialmente, ou seja, 
														sem impor a organização hierárquica da árvore
		Parâmetros:
 				 		 arvore:	Ponteiro para árvore B
		Retorno:
					 		void */
void exibir_arquivo (bt *arvore) {
		int raiz;
		int pagina = 0;
		int i;

		pagina_bt * temp = inicializar_pagina();
		fseek(arvore->arquivo, 0L, SEEK_SET);
		fread(&raiz, sizeof(int), 1, arvore->arquivo);		

		printf("\nRaiz: %d\n", raiz);

		while(fread(temp, sizeof(pagina_bt), 1, arvore->arquivo)) {
			printf("[RRN:%2d, K:{", pagina++);
			for(i = 0; i < ORDEM - 2; i++)
				printf("%3d, ", temp->chaves[i]);
			printf("%3d  } F:{", temp->chaves[i]);		
			
			for(i = 0; i < ORDEM - 1; i++)
				printf("%3d, ", temp->filhos[i]);
			printf("%3d  }]\n", temp->filhos[i]);		
		}			
		free(temp);
}
