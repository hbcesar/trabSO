all: METODOS

METODOS:
	gcc -c estruturaLista.c
	gcc -c funcoesShell.c
	gcc -c leitura.c
	gcc -c executar.c
	gcc -c gerente.c
	gcc -c main.c
	gcc estruturaLista.o funcoesShell.o leitura.o executar.o gerente.o main.o -o executavel