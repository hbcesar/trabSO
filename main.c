#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "TADleitura.h"
#include "TADestruturaLista.h"
#include "TADgerente.h"

int main(){
	int i;
	char* linha_de_comando;
	char** comandos = (char**)malloc(10*sizeof(char*));

	// ignora o sinal ctrl-Z via terminal.
	signal(SIGTSTP, tratadorSinal);

	while(1){
		linha_de_comando = leLinhaDeComando();
		i = quebraLinhaDeComando(linha_de_comando, comandos, "@");
		executaComandos(comandos, i);
	}

	return 0;
}