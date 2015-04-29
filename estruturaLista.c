#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TADestruturaLista.h"

Lista* criaItem(int pid, char* comando){
	Lista* novo = (Lista*)malloc(sizeof(Lista));

	novo->pid = pid;
	novo->comando = (char*)malloc((strlen(comando)+1)*sizeof(char));
	strcpy(novo->comando, comando);
	novo->proximo = NULL;

	return novo;
}

Lista* insereLista(Lista* primeiro, int pid, char* comando){
	Lista* aux = primeiro;

	if(primeiro != NULL){
		while(aux->proximo != NULL)
			aux = aux->proximo;
		aux->proximo = criaItem(pid, comando);
	} else{
		primeiro = criaItem(pid, comando);
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
		return NULL;
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
