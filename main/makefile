COMPILE = gcc -c
LINK = gcc -o
FLAGS =  -ansi

simcpu : simcpu.o parser.o  CPUSim.o Queue.o Thread.o BurstQueue.o BurstNode.o errors.o stats.o
	$(LINK) simcpu simcpu.o parser.o Queue.o CPUSim.o  Thread.o BurstQueue.o BurstNode.o errors.o stats.o

CPU : CPUSim.o Queue.o Thread.o BurstQueue.o BurstNode.o errors.o
	$(LINK) CPU CPUSim.o Queue.o Thread.o BurstQueue.o BurstNode.o errros.o

Queue : Queue.o Thread.o BurstQueue.o BurstNode.o errors.o
	$(LINK) Queue Queue.o Thread.o BurstQueue.o BurstNode.o errors.o

Thread : Thread.o BurstQueue.o BurstNode.o errors.o
	$(LINK) Thread Thread.o BurstQueue.o BurstNode.o errors.o

BurstQueue : BurstQueue.o BurstNode.o errors.o
	$(LINK) BurstQueue BurstQueue.o BurstNode.o errors.o

stats.o : ../stats/stats.c ../stats/stats.h
	$(COMPILE) $(FLAGS) ../stats/stats.c

parser.o : ../parser/parser.c ../parser/parser.h
	$(COMPILE) $(FLAGS) ../parser/parser.c

simcpu.o : simcpu.c simcpu.h
	$(COMPILE) $(FLAGS) simcpu.c

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
