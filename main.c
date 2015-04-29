#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TADleitura.h"

int main(){
	int i;
	char* linha_de_comando;
	char** comandos = (char**)malloc(10*sizeof(char*));

	while(1){
		linha_de_comando = leLinhaDeComando();
		i = quebraLinhaDeComando(linha_de_comando, comandos, "@");
		executaComandos(comandos, i);
	}

	return 0;
}