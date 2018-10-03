all: problem1.o shubert.o
	gcc -pthread -o  problem1 problem1.o -lm
	gcc -pthread -o  shubert shubert.o -lm

problem1.o: problem1.c
	gcc -Wall -Werror -g -c problem1.c 
	
shubert.o: shubert.c
	gcc -Wall -Werror -g -c shubert.c 
	
clean:
	rm -f *.o
	rm -f problem1