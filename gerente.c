#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "TADestruturaLista.h"
#define MAXIMO 100

void tratadorSinal(int sig){
	
	if(sig == SIGINT){
		// bloqueia o sinal ctrl-C via terminal
		signal(SIGINT, SIG_IGN);
	}
	
	if(sig == SIGTSTP){
		//imprime a mensagem
		printf("Não adianta tentar suspender... minha família de processos está protegida!\n");

		// ignora o sinal ctrl-Z via terminal.
		signal(SIGTSTP, SIG_IGN);		
	}

	if(sig == SIGCHLD){ 
		matarTodosProcessos(); 
	}
}

int gerenciadorProcessos(char** comandos){
	int i, pid, exec;

	pid=fork();
	
	signal(SIGTSTP, tratadorSinal);


	if(pid < 0){
		perror("Erro na criação de processo:");
		exit(1);
	} else if (pid == 0){
		if(setsid() == -1)
			perror("Erro no setsid");
		//sou o filho
		//TRATAR UNS SINAIS AQUI
		//executa comando (esse if tem efeito colateral)
		exec = execvp(comandos[0],comandos);
		if( exec == -1){ //Referencia: http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
				printf("Não foi possivel iniciar processo, comando inválido.\n");
		}
		//se processo for morto manda um sinal aqui, acho que é esse SIGKILL
		signal(SIGCHLD, tratadorSinal);
		return 0;
	}else{
		//sou o pai, vou inserir o pid do filho  na lista
		return pid;
	}

}