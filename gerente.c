#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "TADestruturaLista.h"
#define MAXIMO 100


int gerenciadorProcessos(char** comandos){
	int i, pid, exec;
	sigset_t mascara;

	pid=fork();

	if(pid < 0){
		perror("Erro na criação de processo:");
		exit(1);
	} else if (pid == 0){
		
		if ((sigemptyset(&mascara) == -1) || (sigfillset( &mascara)==-1)){  
				perror("Erro no bloqueio de sinais");
		}else{
			if (sigprocmask(SIG_BLOCK, &mascara , NULL) == -1) { //Bloqueia todos os outros sinais
				perror("Erro no bloqueio de sinais");
			}else{
					//sou o filho
					//TRATAR UNS SINAIS AQUI
					//executa comando (esse if tem efeito colateral)
					exec = execvp(comandos[0],comandos);
					if( exec == -1){ //Referencia: http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
						printf("Não foi possivel iniciar processo, comando inválido.\n");
						
					}
					//se processo for morto manda um sinal aqui, acho que é esse SIGKILL
					//raise(SIGKILL);
					return 0;
			}
		}
	}else{
		//sou o pai, vou inserir o pid do filho  na lista
		return pid;
	}

}