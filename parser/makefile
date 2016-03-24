COMPILE = gcc -c
LINK = gcc -o
FLAGS = -Wall -ansi

parser : parser.o CPUSim.o Queue.o Thread.o BurstQueue.o BurstNode.o errors.o
	$(LINK) parser parser.o CPUSim.o Queue.o Thread.o BurstQueue.o BurstNode.o errors.o

CPU : CPUSim.o Queue.o Thread.o BurstQueue.o BurstNode.o errors.o
	$(LINK) CPU CPUSim.o Queue.o Thread.o BurstQueue.o BurstNode.o errros.o

Queue : Queue.o Thread.o BurstQueue.o BurstNode.o errors.o
	$(LINK) Queue Queue.o Thread.o BurstQueue.o BurstNode.o errors.o

Thread : Thread.o BurstQueue.o BurstNode.o errors.o
	$(LINK) Thread Thread.o BurstQueue.o BurstNode.o errors.o

BurstQueue : BurstQueue.o BurstNode.o errors.o
	$(LINK) BurstQueue BurstQueue.o BurstNode.o errors.o

parser.o : parser.c parser.h
	$(COMPILE) $(FLAGS) parser.c

CPUSim.o : ../structures/CPUSim.c ../structures/CPUSim.h
	$(COMPILE) $(FLAGS) ../structures/CPUSim.c

Queue.o : ../structures/Queue.c ../structures/Queue.h
	$(COMPILE) $(FLAGS) ../structures/Queue.c

Thread.o : ../structures/Thread.c ../structures/Thread.h
	$(COMPILE) $(FLAGS) ../structures/Thread.c

BurstQueue.o : ../structures/BurstQueue.c ../structures/BurstQueue.h
	$(COMPILE) $(FLAGS) ../structures/BurstQueue.c

BurstNode.o : ../structures/BurstNode.c ../structures/BurstNode.h
	$(COMPILE) $(FLAGS) ../structures/BurstNode.c

errors.o : ../error/errors.c ../error/errors.h
	$(COMPILE) $(FLAGS) ../error/errors.c

clean :
	rm *.o
	rm *.exe

