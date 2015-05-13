#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "TADleitura.h"
#include "TADestruturaLista.h"

void tratarSIGTSTP(){
	printf("Não adianta tentar suspender... minha família de processos está protegida!");
}

int main(){
	signal( SIGINT, tratamento_SIGINT); 
	signal(SIGTSTP, tratarSIGTSTP);

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