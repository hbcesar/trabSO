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

//tratador de sinal para os processos gerentes
void tratadorSinal(int sig){
	
	if(sig == SIGTSTP){
		//imprime a mensagem
		printf("Não adianta tentar suspender... minha família de processos está protegida!\n");

		// ignora o sinal ctrl-Z via terminal.
		signal(sig, SIG_IGN);
	}
}

/*
 * A função a seguir recebe um vetor com comandos a serem executados
 * Neste caso, cada vetor contem os comando e seus parâmetros.
 * Por exemplo, no caso de: firefox www.google.com @ gedit novo.txt
 * o vetor conterá:
 * comandos[0] = "firefox www.google.com " e comandos[1] = "gedit novo.txt"
 * Por isso, é criado um novo vetor argumentos, que cada casa de comandos em subvetores.
 */
Lista* executaComandos(Lista* lista, char** comandos, int n){
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
		return lista;
	}

	//retira o \n do ultimo argumento
	retiraQuebra(argumentos[k-1]);

	//verifica se as funções sao padrões da shell
	//caso contrário, executa o comando externamente
	if(strcmp(argumentos[0], "pwd") == 0){
		pwd();
		return lista;
	}else if(strcmp(argumentos[0], "cd") == 0){
		cd(argumentos[1]);
		return lista;
	}else if(strcmp(argumentos[0], "waita") == 0){
		waita();
		return lista;
	}else if(strcmp(argumentos[0], "exit") == 0){
		exitar();
		return lista;
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

 			for(i=0; i<n; i++){	
 				//seta o todas as posições do vetor para null
				for(j=0; j<5; j++)
					argumentos[j] = NULL;

				//separa os argumentos de um comando 
				k = quebraLinhaDeComando(comandos[i], argumentos, " ");

				//verifica se o numero de argumentos é permitido
				if(k > 5){
					printf("Número máximo de argumentos excedido para o comando %s.\n", argumentos[0]);
					return lista;
				}

				//retira o \n do ultimo argumento
				retiraQuebra(argumentos[k-1]);

				//funcao que executa novos processos externamente
				// essa fucnao está no arquivo gerente.c
				gerenciadorProcessos(argumentos);

			}

			//assim que qualquer filho morrer, o gerente envia um sinal para matar todos do grupo
			// o grupo tem o mesmo ID do pai (gerente)
			wait(0);
			grupo = getpid();
			//esse comando serve para que o processo gerente, que irá morrer logo em seguida
			//seja retirado da lista de processos da fsh
			raise(SIGCHLD);
			kill(-(grupo), SIGKILL);
			exit(0);


		}else{
			//a fsh insere todos os gerentes na lista
			lista = insereLista(lista, pid);
		}	
	}

	return lista;
}



