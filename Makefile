all: problem1.o problem2a.o shubert.o problem2b.o
	gcc -pthread -o  problem1 problem1.o -lm
	gcc -pthread -o  problem2a problem2a.o -lm -lrt
	gcc -pthread -o  problem2b problem2b.o -lm -lrt
	gcc -pthread -o  shubert shubert.o -lm

problem1.o: problem1.c
	gcc -Wall -Werror -g -c problem1.c 
	
shubert.o: shubert.c
	gcc -Wall -Werror -g -c shubert.c 
	
problem2a.o: problem2a.c
	gcc -Wall -Werror -g -c problem2a.c 
	
problem2b.o: problem2b.c
	gcc -Wall -Werror -g -c problem2b.c 
	
clean:
	rm -f *.o
	rm -f problem1
	rm -f shubert
	rm -f problem2a
	rm -f problem2b
	rm -f problem2c
	rm -f problem2d