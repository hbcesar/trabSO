all: METODOS
	./executavel

METODOS:
	gcc -c -std=c11 estruturaLista.c
	gcc -c -std=c11 funcoesShell.c
	gcc -c -std=c11 leitura.c
	gcc -c -std=c11 gerente.c
	gcc -c -std=c11 main.c
	gcc estruturaLista.o funcoesShell.o leitura.o gerente.o main.o -o executavel