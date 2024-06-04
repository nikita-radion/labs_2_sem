all: compile

compile:
	@gcc -o generate generate.c io.c -Wall -pedantic -std=c99
	@gcc -o print print.c io.c -Wall -pedantic -std=c99
	@gcc -o execute execute.c io.c -Wall -pedantic -std=c99