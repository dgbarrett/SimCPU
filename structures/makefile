COMPILE = gcc -c
LINK = gcc -o
FLAGS = -Wall -ansi

CPU : CPUSim.o Queue.o Thread.o BurstQueue.o BurstNode.o errors.o
	$(LINK) CPU CPUSim.o Queue.o Thread.o BurstQueue.o BurstNode.o errros.o

Queue : Queue.o Thread.o BurstQueue.o BurstNode.o errors.o
	$(LINK) Queue Queue.o Thread.o BurstQueue.o BurstNode.o errors.o

Thread : Thread.o BurstQueue.o BurstNode.o errors.o
	$(LINK) Thread Thread.o BurstQueue.o BurstNode.o errors.o

BurstQueue : BurstQueue.o BurstNode.o errors.o
	$(LINK) BurstQueue BurstQueue.o BurstNode.o errors.o

CPUSim.o : CPUSim.c CPUSim.h
	$(COMPILE) $(FLAGS) CPUSim.c

Queue.o : Queue.c Queue.h
	$(COMPILE) $(FLAGS) Queue.c

Thread.o : Thread.c Thread.h
	$(COMPILE) $(FLAGS) Thread.c

BurstQueue.o : BurstQueue.c BurstQueue.h
	$(COMPILE) $(FLAGS) BurstQueue.c

BurstNode.o : BurstNode.c BurstNode.h
	$(COMPILE) $(FLAGS) BurstNode.c

errors.o : ../error/errors.c ../error/errors.h
	$(COMPILE) $(FLAGS) ../error/errors.c

clean :
	rm *.o
	rm *.exe
