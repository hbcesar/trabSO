#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TADleitura.h"

//Referencia: http://stackoverflow.com/questions/3919009/how-to-read-from-stdin-with-fgets
char* leLinhaDeComando(){
	char* comando = (char*)malloc(MAXIMO*sizeof(char));

	printf("fsh>");
	fgets(comando, MAXIMO ,stdin);

	return comando;
}

//Referencia: https://support.microsoft.com/en-us/kb/51327/pt-br
int quebraLinhaDeComando(char* linha_de_comando, char** comandos, char* divisor){
		signal(SIGTSTP, tratadorSinal);

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