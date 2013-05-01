CC = g++
LFLAGS = -lm 
CFLAGS = -Wall -g

make: gameboard golutilities   showgen  sim-tui  sim-gui clean

gameboard: gameboard.h gameboard.cpp 
	$(CC) gameboard.cpp -c $(CFLAGS)

golutilities: golutilities.h golutilities.cpp 
	$(CC) golutilities.cpp  -c  $(CFLAGS) -lm

showgen: showgen.cpp gameboard.o golutilities.o 
	$(CC) golutilities.o gameboard.o -o showgen showgen.cpp  $(CFLAGS) $(LFLAGS)

sim-tui: sim-tui.cpp gameboard.o golutilities.o 
	$(CC) golutilities.o gameboard.o -o sim-tui sim-tui.cpp $(CFLAGS) $(LFLAGS) -lncurses

sim-gui: 
	cd sim-gui/; make
#Utilities
clean: 
	/bin/rm -f *.o
