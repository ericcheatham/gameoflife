CC = g++
LFLAGS = -lm 
CFLAGS = -Wall -g

make: gameboard golutilities showgen  sim-tui clean

gameboard: gameboard.h gameboard.cpp 
	$(CC) gameboard.cpp -c $(CFLAGS)

golutilities: golutilities.h golutilities.cpp gameboard.o
	$(CC) golutilities.cpp  -c  $(CFLAGS)

showgen: showgen.cpp gameboard.o golutilities.o 
	$(CC) golutilities.o gameboard.o -o showgen showgen.cpp  $(CFLAGS) $(LFLAGS)

sim-tui: sim-tui.cpp gameboard.o golutilities.o 
	$(CC) golutilities.o gameboard.o -o sim-tui sim-tui.cpp $(CFLAGS) $(LFLAGS) -lncurses
#Utilities
clean: 
	/bin/rm -f *.o
