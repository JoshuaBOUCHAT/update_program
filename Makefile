my_program:main.o
	gcc -o my_program main.o

main.o:main.c
	gcc -c main.c -o main.o

run:my_program
	./my_program