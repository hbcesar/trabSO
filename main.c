#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "TADleitura.h"
#include "TADestruturaLista.h"
#include "TADgerente.h"
#include "TADexecutar.h"

Lista* lista = NULL;
sigset_t masc_default;

void bloquearSIGINT(){
	sigset_t mascara;

	/*
	 * Inicializa a mascara de sinais, onde:
	 * sigemptyset: inicializa a mascara de sinais
	 * sigaddset: preenche a mascara com SIGINT (Ctrl+C)
	 */
	if (sigemptyset(&mascara)== -1){  
			perror("Erro ao criar mascara de sinais");
			abort();
	} else if(sigaddset(&mascara, SIGINT)==-1){
			perror("Erro ao setar mascara de sinais");
			abort();
	} else if (sigprocmask(SIG_BLOCK, &mascara , &masc_default) == -1) {
			perror("Erro ao bloquear o sinal ctrl-C");
			abort();
	}
}

/*
 * Essa funcao acessa a lista de processos gerentes da fsh
 * e entao envia um sinal para que todos eles sejam suspensos
 */
void suspenderTodosProcessos(){ 
	Lista* aux = lista; 

	while(aux != NULL){ 
		kill(aux->pid, SIGTSTP);
		aux = aux->proximo;
	}
}

/*
 * Essa função trata tres tipos de sinais diferentes:
 * SIGINT (ctrl-C): caso a shell nao tenha nenhum gerente rodando, sai
 * 					caso contrário, bloqueia o sinal
 * SIGTSTP (ctrl-Z): reenvia o sinal a todos os gerentes e o ignora
 * SIGCHLD: para o caso de um gerente ter morrido, recebe o sinal e o remove da lista
 */
void tratadorMain(int sig){
	
	if(sig == SIGINT){
		if(lista == NULL){
			sigprocmask(SIG_SETMASK, &masc_default, NULL);
			exit(0);
		} else{
			bloquearSIGINT();
		}
	} else if(sig == SIGTSTP){
		suspenderTodosProcessos();

		//ignora o sinal ctrl-Z via terminal.
		signal(sig, SIG_IGN);
	}else if(sig == SIGCHLD){ 
		int pid;
		pid = waitpid(-1, NULL, WNOHANG);
		lista = remover(lista, pid);
	}
}

int main(){
	int i;
	char* linha_de_comando;
	char** comandos = (char**)malloc(10*sizeof(char*));

	// trata o sinal ctrl-Z  e ctrl-C via terminal.
	signal(SIGINT, tratadorMain);
	signal(SIGTSTP, tratadorMain);
	signal(SIGCHLD, tratadorMain);


	for(;;){
		linha_de_comando = leLinhaDeComando();
		i = quebraLinhaDeComando(linha_de_comando, comandos, "@");
		lista = executaComandos(lista, comandos, i);
	}

	return 0;
}