#ifndef TADESTRUTURALISTA_H_
#define TADESTRUTURALISTA_H_

/* define tipo lista */
struct listinha
{
	int pid;
	char* comando;
	struct listinha* proximo;
};

typedef struct listinha Lista;


/* cria um item da lista
 * input: um inteiro correspondende ao pid do processo e uma string correspondente ao comando
 * output: uma lista contendo o item criado */
Lista* criaItem(int pid, char* comando);

/* insere um item na lista 
 * input: uma lista, um inteiro com o pid do processo e uma string com o comando
 * output: uma lista com o item inserido*/
Lista* insereLista(Lista* primeiro, int pid, char* comando);

/* busca um processo na lista de processos 
 * input: uma lista contendo os processos e um inteiro contendo o numero do pid do processo a ser procurado 
 * output: uma lista com o item caso seja achado ou NULL */
Lista* buscaProcesso(Lista* l, int pid);

/* remove um item da lista
 * input: uma lista de processos e um numero inteiro com o pid que deseja remove
 * output: uma lista sem o item removido */
Lista* remover(Lista* primeiro, int pid);

#endif 

