#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "TADestruturaLista.h"
#define MAXIMO 100

int gerenciadorProcessos(char** comandos){
	int i, pid, exec;

	pid=fork();
	
	if(pid < 0){
		perror("Erro na criação de processo:");
		exit(1);
	} else if (pid == 0){
		printf("Sou o filho do gerente, %d\n", getpid());
		
		//sou o filho
		//TRATAR UNS SINAIS AQUI
		//executa comando (esse if tem efeito colateral)
		//Referencia: http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
		exec = execvp(comandos[0],comandos);
		if( exec == -1){ 
				printf("Não foi possivel iniciar processo, comando inválido.\n");
				abort();
		}
		//se processo for morto manda um sinal aqui, acho que é esse SIGKILL
		//signal(SIGCHLD, tratadorSinal);
		return 0;
	}else{
		//sou o pai, vou inserir o pid do filho  na lista
		return pid;
	}

}