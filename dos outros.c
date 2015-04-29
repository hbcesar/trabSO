#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shel.h"
#include <signal.h>

#define FOREGROUND 1
#define BACKGROUND 0
#define MAXIMO 200
#define MAX_PALAVRAS_COMANDO 20

struct processo{
	pid_t pid;
	int ground;
	char* nome ;
	struct processo* prox ;
} ;

struct lista{
	Processo* primeiro ;
	Processo* ultimo ;
} ;



/* função retira: retira Processo da lista */
void retira (int pid) 
{
	Processo* a = NULL; /* ponteiro para Processo anterior */
	Processo* p = lista->primeiro; /* ponteiro para percorrer a lista */
	/* procura Processo na lista, guardando anterior */
	while (p != NULL && p->pid != pid) { 
 		a = p;
 		p = p->prox; 
	}
	/* verifica se achou Processo */
	if (p == NULL){
 		return ; /* não achou: retorna lista original */
	}
	/* retira Processo */
	if (a == NULL) 
	{ /* retira Processo do inicio */
		lista->primeiro = p->prox; 
	}else { 
		/* retira Processo do fim lista */
		if (p->pid == lista->ultimo->pid){
			lista->ultimo = a ; 
		}		
		a->prox = p->prox; 
	}
	free(p->nome) ;
	free(p) ;
}

//Funcao que trarnsforma a linha de comando em uma matriz de strings(array)
char** string_to_array(char* comando, int *ground){

	char *word, **array;
	int i=0;	
	*ground=FOREGROUND;

	word = strtok(comando," \n");
	array = calloc(MAX_PALAVRAS_COMANDO,sizeof(char*) );
	while(word !=NULL){
		if( strcmp(word,"&")==0 ){
			*ground=BACKGROUND;
			return array;
		}
		int length = strlen(word);
		array[i]=calloc(length+1,sizeof(char) );
		strcpy(array[i++],word);
		word = strtok (NULL, " \n");
	}
	return array;
}

//Funcao utilizada para liberar um array que tenha sido alocado
void libera_array(char** array){

	int i;

	for(i=0; array[i]!=NULL ;i++) {
		free(array[i]);
	}
	free(array);
}

//Funcao que executa comandos externos
void externos(char** comando, int ground){
	int pid;
	sigset_t mascara;

	pid = fork();
	if(pid==-1){
		perror("Erro criação processo filho\n");
		exit(1);
	}else{ 
		if( pid == 0 ){
			if ( (sigemptyset(&mascara) == -1) || (sigfillset( &mascara)==-1) ) {  
				perror("Erro no bloqueio de sinais");
			}else{
				if (sigprocmask(SIG_BLOCK, &mascara , NULL) == -1) { //Bloqueia todos os outros sinais
					perror("Erro no bloqueio de sinais");
				}else{
					if( execvp(comando[0],comando) == -1){
						printf("Comando invalido\n");
						raise(SIGKILL);
					}
				}
			}
		}else{
			Processo* p = cria_processo(pid,ground, comando[0]); //Guarda processo filho no vetor 
			lista = insere_processo(lista,p);
			if( ground== FOREGROUND ){				//Se processo for Foreground, processo pai(shell) espera
				waitpid(pid, NULL, WUNTRACED);
			}
			
		}
	}
}

Lista* cria_lista(void){
	Lista* lista = (Lista*)malloc(sizeof(Lista));
	lista->primeiro = NULL ;
	lista->ultimo = NULL ;
	return lista ;
}

Processo* cria_processo(pid_t pid, int ground, char* nome_proc){
	Processo* proc = (Processo*) malloc(sizeof(Processo)) ;
	int tam = 1 + strlen(nome_proc) ;
	proc->nome = (char*) malloc(tam*(sizeof(char))) ;
	strcpy(proc->nome,nome_proc) ;
	proc->pid = pid;
	proc->ground = ground;
	return proc ;
}

Lista* insere_processo(Lista* lista, Processo* proc){

	Processo* aux ;

	if (lista->primeiro == NULL){
		lista->ultimo = proc ;
		lista->primeiro = proc ;
	}
	lista->ultimo->prox = proc ;
	proc->prox = NULL ;
	lista->ultimo = proc ;
	return lista;
}

void funcao_pwd(){
	char path_atual[200];
	if (getcwd(path_atual, sizeof(path_atual)) != NULL){
		printf("%s\n", path_atual);
 	}else{
	 	perror("Erro");
	}
}

void funcao_cd(char* cd){

	if(cd != NULL && chdir(cd) == 0) {
	}else{
		perror("Erro") ;
	}
}

void funcao_exit(){
	Processo* aux;
	int pid, causa;	
	for(aux=lista->primeiro; aux != NULL;  aux = aux->prox){
		kill(aux->pid, SIGKILL);
	}
	while(pid = waitpid( -1, &causa , WNOHANG)) 
		if ( pid == -1 ) {
			break;
		}	
	
	exit(0);
}

void funcao_clean() {

	int pid, causa; // Funcao que libera os filhos no estado zombie

	while( pid = waitpid( -1, &causa , WNOHANG) ){
		if ( pid == -1 ) {
			break;
		
		}	
		
		if ( pid == 0 ) {
			printf("%d NAO MORREU", pid);
		
		}
		else {
			if( WIFEXITED(causa) ) {
				printf("%d MORREU: %d\n", pid, WEXITSTATUS(causa) ); 
			}
			else if ( WIFSIGNALED(causa) ) { 
				printf("%d MORREU: %d\n", pid, WTERMSIG(causa) );
			}		
		}		
	}



}

void funcao_wait(){
	int* status ;

	pid_t pid = wait(status) ; //Funcao que deve bloquear esperando que todos os seus filhos tenham morrido


	while ( pid > 0) {
		if ( pid == -1 ) {
			break;
		}		
		printf("%d RETORNOU: ", pid) ;
		if( WIFEXITED(status) ){
				printf("%d\n", WEXITSTATUS(status) );
			}
	
	pid = wait(status) ;
	}
}


void le_comando(char* comando){
	int ground ;
	char* f_b_ground = strchr(comando,'&') ; //Funcao que le o comando passado pelo teclado ou arquivo
	if (f_b_ground != NULL){  		// Se tiver o & faz o processo ser background
		ground = FOREGROUND ;
	}else{
		ground = BACKGROUND ;
	}
	executa_comando(comando,ground) ;
	free(comando) ;
}


void executa_comando(char* comando, int ground){
	char **array;	
	ground=0;
	if(strcmp(comando,"\n")!=0){			//Funcao que escolhe qual comando executar
		if( strcmp(comando,"clean") == 0 ){
			funcao_clean();
			return ;
		}
		if( strcmp(comando,"pwd") == 0 ){
			funcao_pwd();
			return ;
		}
		if( strcmp(comando,"wait")==0 ){
			funcao_wait();
			return ;			
		}
		if (strstr(comando, "echo") != NULL ){ 	
			char* novo = strtok(comando," ") ;
			if( strcmp(novo,"echo") == 0 ){
				novo = strtok(NULL," ") ;
				while (novo != NULL) {
					printf("%s ", novo) ;
					novo = strtok(NULL," ") ;
				}
				printf("\n") ;
			}
			return ;
		}	
		if( strcmp(comando,"exit") == 0 ){
			funcao_exit();
			return ;
		}
		if (strstr(comando, "cd") != NULL ){ 	
			char* novo = strtok(comando," ") ;
			if( strcmp(novo,"cd") == 0 ){
				novo = strtok(NULL," ") ;
				funcao_cd( novo ) ;
			}
			return ;
		}
		array = string_to_array(comando, &ground);
		externos(array,ground) ;
		libera_array(array);
		return ;
	}
	return ;
}

void tratamento_SIGSTP(){
	Processo *aux;
	printf("\n<Ctrl-Z pressionado>\n");			//Funcao de Tratamento do sinal Ctrl + z
	printf("Processos suspensos:\n") ;
	for(aux=lista->primeiro; aux != NULL;  aux = aux->prox){
		printf("%d - %s\n", aux->pid, aux->nome) ;
		kill( aux->pid, SIGSTOP);
		retira(aux->pid) ;
	}
	setbuf(stdout, NULL);
}

void tratamento_SIGINT(){
	Processo *aux;
	printf("\n<Ctrl-C pressionado>\n");
	printf("Processos finalizados:\n") ;
	for(aux=lista->primeiro; aux != NULL;  aux = aux->prox){  //Funcao de Tratamento do sinal Ctrl + C. So funciona em processos foreground
		if (aux->ground == FOREGROUND){
			printf("%d - %s\n", aux->pid, aux->nome) ;
			kill( aux->pid, SIGKILL);
			retira(aux->pid) ;
		}
	}
	setbuf(stdout, NULL);
}

//Função que permite, por exemplo, que "pwd" e "   pwd   " sejam comandos válidos 
char * Tira_Espacos_Antes_e_Depois(char *string)
{
	char *fim;

	while(isspace(*string)){ 
		string++;
	}

  	if(*string == 0){  // All spaces?
    		return string;
	}
	fim = string + strlen(string) - 1;
	while(fim > string && isspace(*fim)) {
		fim--;
	}
	*(fim+1) = 0;

	return string;
}

//Função que tira o \n da string
void strip(char *s) {
    char *p2 = s;
    while(*s != '\0') {
    	if(*s != '\t' && *s != '\n') {
    		*p2++ = *s++;
    	} else {
    		++s;
    	}
    }
    *p2 = '\0';
}

void le_arquivo(char* entrada ){

	char comando[MAXIMO];				//Funcao que le o arquivo de entrada
	FILE *arquivo;

	arquivo = fopen ( entrada , "r");
	if(arquivo != NULL){
		while( fscanf( arquivo," %[^\n]s", comando) != EOF ) {
			int tam = strlen(comando)+1 ;
			char* cmd = (char*) malloc(tam*sizeof(char)) ;
			strcpy (cmd, comando) ; 
			le_comando(cmd) ;
		}
		fclose(arquivo);
	}else{
		printf("Erro na abertura do arquivo!") ;
	}
}

void le_console(){
	printf("shell %% ");    		//Funcao que le do teclado
	char string[MAXIMO];
	fgets(string, MAXIMO ,stdin);
	
	int tam = strlen(string)+1 ;
	char* comando = (char*) malloc(tam*sizeof(char)) ;
	strcpy (comando, string) ; 
	
	strip(comando) ;
	Tira_Espacos_Antes_e_Depois(comando) ;
	le_comando(comando) ;
}

int main(int argc, char* argv[]){
	
	signal( SIGCONT, SIG_IGN );
	signal( SIGINT, tratamento_SIGINT); 	
	signal( SIGTSTP, tratamento_SIGSTP);
	lista = cria_lista() ;
	if (argc > 1) {
		le_arquivo(argv[1]) ;
	}
	for(;;){
		le_console() ;
	}
	return 0 ;
}


