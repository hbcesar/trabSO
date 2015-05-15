#ifndef LEITURA_H_
#define LEITURA_H_

/* leitura da linha de comando 
 * input: nada
 * output: uma string contendo o cmando  */
char* leLinhaDeComando();

/* separa a linha em varios comandos
 * input: uma string contendo varios comandos e uma vetor de string para armazena os comandos 
 * output: um inteiro positivo para informar quantos comandos foram lidos */
int quebraLinhaDeComando(char* linha_de_comando, char** comandos, char* divisor);

void retiraQuebra(char* s);

#endif