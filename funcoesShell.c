#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "TADestruturaLista.h"
#define MAXIMO 100

//Referencia: http://manpages.ubuntu.com/manpages/precise/pt/man3/getcwd.3.html
void pwd(){
	char diretorio[100];

	if(getcwd(diretorio, MAXIMO) != NULL)
		printf("%s\n", diretorio);
	else
		perror("Não foi possível obter o diretorio atual:");
}

//Referência: http://stackoverflow.com/questions/16376892/c-using-chdir-function
void cd(char* dir){
	int ret;

	if (dir[strlen (dir) - 1] == '\n')
        dir[strlen (dir) - 1] = '\0';

	ret = chdir(dir);

	if (ret != 0){
		perror("Nao foi possivel alterar o diretorio:");
	}
}

 void waita(){
	int pid, causa;

	while((pid = waitpid( -1, &causa , WNOHANG))){
		if (pid == -1) { //erro no wait
			break;
		} else {
			if(WIFEXITED(causa)) { //se processo terminou espontaneamente
				printf("O processo %d foi terminado, causa: %d\n", pid, WEXITSTATUS(causa));
				kill(pid, SIGKILL);
			}
			else if (WIFSIGNALED(causa)) { //se processo recebeu sinal (teoricamente esse if nunca acontece para a fsh)
				printf("O processo %d foi terminado, causa: %d\n", pid, WTERMSIG(causa));
				kill(pid, SIGKILL);
			}		
		}		
	}
}

/*
 * envia comando kill para todos os gerentes
 * os filhos deles sao herdados por init e entao também terminados
 * ao fina, sai da fsh
 */
void exitar(Lista* lista){
	Lista* aux = lista;

	while(aux != NULL){ 
		kill(aux->pid, SIGKILL);
		aux = aux->proximo;
	}

	exit(0);
}