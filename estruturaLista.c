#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TADestruturaLista.h"

//Este bloco do programa contém apenas funções padrões de controle de lista

Lista* criaItem(int pid){
	int i;
	Lista* novo = (Lista*)malloc(sizeof(Lista));

	novo->pid = pid;
	novo->proximo = NULL;

	return novo;
}

Lista* insereLista(Lista* primeiro, int pid){
	Lista* aux = primeiro;

	if(primeiro != NULL){
		while(aux->proximo != NULL){
			aux = aux->proximo;
		}
		aux->proximo = criaItem(pid);
	} else{
		primeiro = criaItem(pid);
	}

	return primeiro;
}

Lista* buscaProcesso(Lista* l, int pid){
	Lista* aux;
	for(aux = l; aux != NULL; aux = aux->proximo){
		if(aux->pid == pid){
			return aux;
		}
	}
	return NULL;
}

Lista* remover(Lista* primeiro, int pid){
	Lista* aux = primeiro;
	Lista* anterior = NULL;

	if(buscaProcesso(primeiro, pid) == NULL)
		return primeiro;
	else{
		while(aux->pid != pid){
			anterior = aux;
			aux = aux->proximo;
		}
		if(anterior != NULL){
			anterior->proximo = aux->proximo;
		} else{
			primeiro = aux->proximo;
		}
		free(aux);
	}

	return primeiro;
}

void imprimeLista(Lista* lista){
	Lista* aux = lista;

	while(aux != NULL){
		printf("PID: %d\n", aux->pid);
		aux = aux->proximo;
	}
}
