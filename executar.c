#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
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

void executaComandos(char** comandos, int n){
	//Lista* lista = NULL;
	int i, j, k, size, pid=0, pid_filhos=0;
	char** argumentos = (char**)malloc(10*sizeof(char*));

	//inicia gerente
	pid = fork();

	if(pid < 0){
		perror("Erro na criação de processo:");
		exit(1);
	} else if (pid == 0){
		printf("Sou o gerente, %d\n", getpid());

		//seta novos tratadores de sinal
		signal(SIGTSTP, tratadorSinal);
		signal(SIGINT, tratadorSinal);


		for(i=0; i<n; i++){
			/*
			 * seta as casas do vetor argumentos para null
			 * isso evita que argumentos do loop anterior sejam passados como parametro do novo argumento
			 */
			for(j=0; j<5; j++){
				argumentos[i] = NULL;
			}

			//quebra os argumentos entre um @ e outro
			k = quebraLinhaDeComando(comandos[i], argumentos, " ");
			
			//verifica o numero máximo de argumentos permitido
			if (k>5){
				printf("Número máximo de argumentos excedido para o comando %s.\n", argumentos[0]);
				continue;
			}

			//como o ultimo argumento vem com \n, chama essa funcao pra retira-lo
			retiraQuebra(argumentos[k-1]); 
			
			//a partir daqui, verifica se o comando é uma funcao de shell ou externo (valido ou nao)
			if(strcmp(argumentos[0], "pwd") == 0){
				pwd();
				return;
			} 
			else if(strcmp(argumentos[0], "cd") == 0){
				cd(argumentos[1]);
				return;
			}
			else{
				pid = gerenciadorProcessos(argumentos);
				if(pid > 0){
					lista = insereLista(lista, pid);
				}
			}
		}

		printf("imprimindo os filhos do gerente\n");
		imprimeLista(lista);

		//espera o termino dos filhos
		wait(0);

		//se um filho for killed, mata o resto todo

		//matarTodosProcessos();
	} else{
		printf("Sou a bash, %d\n", getpid());
		//aqui a bash mantem uma lista de todos os seus processos gerentes
		lista = insereLista(lista, pid);
		printf("imprimindo os filhos da bash\n");
		imprimeLista(lista);
	}
}

void esperar(){
	Lista* aux = lista;

	while(aux != NULL){ 
		wait(&aux->pid); 
		aux = aux->proximo; 
	}
}

void matarTodosProcessos(){ 
	Lista* aux = lista; 

	while(aux != NULL){ 
		kill(aux->pid, SIGKILL);
		lista = remover(lista, aux->pid);
		aux = aux->proximo; 
	}

	printf("Morreram todos os filhos de %d", getpid());

	exit(1);
}

void suspenderTodosProcessos(){ 
	Lista* aux = lista; 

	while(aux != NULL){ 
		kill(aux->pid, SIGTSTP);
		aux = aux->proximo;
	}

	printf("Suspenderam todos os filhos de %d\n", getpid());
}

