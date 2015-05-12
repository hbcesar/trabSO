#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "TADleitura.h"
#include "TADfuncoesShell.h"
#define MAXIMO 100

//Referencia: http://stackoverflow.com/questions/3919009/how-to-read-from-stdin-with-fgets
char* leLinhaDeComando(){
	char* comando = (char*)malloc(MAXIMO*sizeof(char));

	printf("fsh>");
	fgets(comando, MAXIMO ,stdin);

	return comando;
}

//Referencia: https://support.microsoft.com/en-us/kb/51327/pt-br
int quebraLinhaDeComando(char* linha_de_comando, char** comandos, char* divisor){

	char* partes;
	int i = 0, j;

	partes = strtok(linha_de_comando, divisor);

	while(partes != NULL && i<10){
    	comandos[i] = (char*)malloc((strlen(partes)+1)*sizeof(char));
    	strcpy(comandos[i], partes);
    	i++;
    	partes = (char*)strtok(NULL, divisor);
  	}

  	return i;
}

//Retirada de: http://stackoverflow.com/questions/1515195/how-to-remove-n-or-t-from-a-given-string-in-c
//nenhuma alteração foi feita
void retiraQuebra(char* s){
	char *p2 = s;
    while(*s != '\0') {
    	if(*s != '\t' && *s != '\n') {
    		*p2++ = *s++;
    	} else {
    		++s;
    	}
    }
    *p2 = '\0';
}


void executaComandos(char** comandos, int n){
	int i, j, k, size;
	char** argumentos = (char**)malloc(10*sizeof(char*));

	for(i=0; i<n; i++){
		/*
		 * seta as casas do vetor argumentos para null
		 * isso evita que argumentos do loop anterior sejam passados como parametro do novo argumento
		 */
		for(j=0; j<5; j++){
			argumentos[i] = NULL;
		}
		k = quebraLinhaDeComando(comandos[i], argumentos, " "); //quebra os argumentos entre um @ e outro
		retiraQuebra(argumentos[k-1]); //como o ultimo argumento vem com \n, chama essa funcao pra retira-lo
		
		if(strcmp(argumentos[0], "pwd") == 0){
			pwd();
			return;
		} 
		else if(strcmp(argumentos[0], "cd") == 0){
			cd(argumentos[1]);
			return;
		}
		else{
			gerenciadorProcessos(argumentos);
		}
	}
}