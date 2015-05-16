#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "TADestruturaLista.h"
#include "TADexecutar.h"
#define MAXIMO 100


void gerenciadorProcessos(char** comandos){
	int i, pid, exec;

	pid=fork();
	
	if(pid < 0){
		perror("Erro na criação de processo:");
		exit(1);
	} else if (pid == 0){

		//Referencia: http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
		exec = execvp(comandos[0],comandos);
		if( exec == -1){ 
			printf("Não foi possivel iniciar processo, comando inválido.\n");
			abort();
		}
	}

}