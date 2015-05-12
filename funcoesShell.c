#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "TADestruturaLista.h"
#define MAXIMO 100


void gerenciadorProcessos(char** comandos){
	Lista* lista = NULL;
	int i, pid;

	pid=fork();

	if(pid < 0){
		perror("Erro na criação de processo:");
		exit(1);
	} else if (pid == 0){
		//sou o filho
		//TRATAR UNS SINAIS AQUI
		//executa comando (esse if tem efeito colateral)
		if( execvp(comandos[0],comandos) == -1){ //Referencia: http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
			printf("Não foi possivel iniciar processo, comando inválido.\n");
			//raise(SIGKILL);
		}
	} else{
		//sou o pai, vou inserir o pid do filho  na lista
		lista = insereLista(lista, pid, comandos[0]);
	}
}

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