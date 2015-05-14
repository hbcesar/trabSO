#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "TADleitura.h"
#include "TADestruturaLista.h"
#include "TADgerente.h"

void tratadorMain(int sig){
	
	if(sig == SIGINT){
		// bloqueia o sinal ctrl-C via terminal
		printf("Vou sair só prq sou bonzinho, queridinha\n");
		//signal(sig, SIG_IGN);
		exit(0);
	}
	
	if(sig == SIGTSTP){
		printf("Sou o processo %d\n", getpid());

		//suspende os filhos
		suspenderTodosProcessos();

		// ignora o sinal ctrl-Z via terminal.
		signal(sig, SIG_IGN);
	}

	if(sig == SIGCHLD){ 
		//matarTodosProcessos(); 
	}
}


int main(){
	int i;
	char* linha_de_comando;
	char** comandos = (char**)malloc(10*sizeof(char*));

	// ignora o sinal ctrl-Z  e ctrl-C via terminal.
	signal(SIGINT, tratadorMain);
	signal(SIGTSTP, tratadorMain);

	for(;;){
		linha_de_comando = leLinhaDeComando();
		i = quebraLinhaDeComando(linha_de_comando, comandos, "@");
		executaComandos(comandos, i);
	}
	scanf("%d", &i);

	return 0;
}