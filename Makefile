all: METODOS
	./executavel

METODOS:
	gcc -c estruturaLista.c
	gcc -c funcoesShell.c
	gcc -c leitura.c
	gcc -c gerente.c
	gcc -c main.c
	gcc estruturaLista.o funcoesShell.o leitura.o gerente.o main.o -o executavel