#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAXIMO 100

void gerenciadorProcessos(char* comandos, int n){
	Lista* lista = NULL;
	int i, pid;
	pid=fork();

	if(pid = 0){
		printf("Sou o gerenciador");
		for(i=0; i<n; i++){
			pid = fork();
			if(pid != 0){
				lista = insereLista(lista, pid, comando[i]);
			} else {
				//sou o filho executo algo em background
			}

		}
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