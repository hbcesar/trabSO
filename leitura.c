#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TADleitura.h"
#include "TADfuncoesShell.h"
#define MAXIMO 100

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

void executaComandos(char** comandos, int n){
	int i, k;
	char** argumentos = (char**)malloc(10*sizeof(char*));


	for(i=0; i<n; i++){
		k = quebraLinhaDeComando(comandos[i], argumentos, " "); //quebra os argumentos entre um @ e outro
		if(strcmp(argumentos[0], "pwd\n") == 0){
			pwd();
		} 
		else if(strcmp(argumentos[0], "cd") == 0){
			cd(argumentos[1]);
		}
	}
}