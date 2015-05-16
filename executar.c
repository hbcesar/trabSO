#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include "TADleitura.h"
#include "TADfuncoesShell.h"
#include "TADestruturaLista.h"
#include "TADgerente.h"
#include "TADexecutar.h"

Lista* lista = NULL;

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
}

void executaComandos(char** comandos, int n){
	int j=0, i=0, pid, k, grupo;
	char** argumentos = (char**)malloc(10*sizeof(char*));

	//seta o todas as posições do vetor para null
	for(j=0; j<5; j++)
		argumentos[j] = NULL;

	//separa os argumentos de um comando 
	k = quebraLinhaDeComando(comandos[0], argumentos, " ");

	//verifica se o numero de argumentos é permitido
	if(k > 5){
		printf("Número máximo de argumentos excedido para o comando %s.\n", argumentos[0]);
		return;
	}

	//retira o \n do ultimo argumento
	retiraQuebra(argumentos[k-1]);

	if(strcmp(argumentos[0], "pwd") == 0){
		pwd();
		return;
	}else if(strcmp(argumentos[0], "cd") == 0){
		cd(argumentos[1]);
		return;
	}else if(strcmp(argumentos[0], "waita") == 0){
		waita();
		return;
	}else if(strcmp(argumentos[0], "imprimir") == 0){
		imprimeLista(lista);
		return;
	}else{

		//cria gerente 
		pid = fork();

		//verifica se o processo foi criado
		if(pid < 0){
			perror("Erro na criação do processo!");
			exit(1);
		}else if(pid == 0){

			//cria os processos em backgroud e torna o gerente lider de sessao	
			setsid();

			//trata os sinais
			signal(SIGTSTP, tratadorSinal);
 			signal(SIGINT, tratadorSinal);
 			signal(SIGCHLD, tratadorSinal);

 			for(i=0; i<n; i++){	
 				//seta o todas as posições do vetor para null
				for(j=0; j<5; j++)
					argumentos[j] = NULL;

				//separa os argumentos de um comando 
				k = quebraLinhaDeComando(comandos[i], argumentos, " ");

				//verifica se o numero de argumentos é permitido
				if(k > 5){
					printf("Número máximo de argumentos excedido para o comando %s.\n", argumentos[0]);
					return;
				}

				//retira o \n do ultimo argumento
				retiraQuebra(argumentos[k-1]);

				gerenciadorProcessos(argumentos);

			}

			//assim que um filho morrer ele envia um sinal para matar todos do grupo
			// o grupo tem o mesmo ID do pai (gerente)
			wait(0);
			grupo = getpid();
			raise(SIGCHLD);
			kill(-(grupo), SIGKILL);
			exit(0);


		}else{
			//a fsh insere todos os gerentes na lista
			lista = insereLista(lista, pid);
		}	
	}
}


void suspenderTodosProcessos(){ 
	Lista* aux = lista; 

	while(aux != NULL){ 
		kill(aux->pid, SIGTSTP);
		aux = aux->proximo;
	}
}

void removerLista(int pid){
	lista = remover(lista, pid);
}

