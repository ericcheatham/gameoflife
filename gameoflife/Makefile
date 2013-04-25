CC = g++
LFLAGS = -lm 
CFLAGS = -Wall -g

make: gameboard golutilities showgen  clean

gameboard: gameboard.h gameboard.cpp 
	$(CC) gameboard.cpp -c $(CFLAGS)

golutilities: golutilities.h golutilities.cpp gameboard.o
	$(CC) golutilities.cpp  -c  $(CFLAGS)

showgen: showgen.cpp gameboard.o golutilities.o 
	$(CC) golutilities.o gameboard.o -o showgen showgen.cpp  $(CFLAGS) $(LFLAGS)

#Utilities
clean: 
	/bin/rm -f *.o
