#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "TADleitura.h"
#include "TADestruturaLista.h"
#include "TADgerente.h"

void tratadorSinal(int sig){
	
	if(sig == SIGINT){
		// bloqueia o sinal ctrl-C via terminal
		printf("Vou sair só prq sou bonzinho, queridinha\n");
		//signal(sig, SIG_IGN);
		exit(0);
	}
	
	if(sig == SIGTSTP){
		//imprime a mensagem
		printf("Não adianta tentar suspender... minha família de processos está protegida!\n");

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
	signal(SIGINT, tratadorSinal);
	signal(SIGTSTP, tratadorSinal);

	for(;;){
		linha_de_comando = leLinhaDeComando();
		i = quebraLinhaDeComando(linha_de_comando, comandos, "@");
		executaComandos(comandos, i);
	}
	scanf("%d", &i);

	return 0;
}