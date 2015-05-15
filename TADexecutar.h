#ifndef TADEXECUTAR_H_
#define TADEXECUTAR_H_

/* executa todos os comandos 
 * input: um vetor de string contendo os comandos e um numero inteiro para informar a quantidade de comandos
 * output: nada */
void executaComandos(char** comandos, int n);

void matarTodosProcessos();

void esperar(int status);

void suspenderTodosProcessos();

#endif  