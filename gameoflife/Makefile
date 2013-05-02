CC = g++
LFLAGS = -lm 
CFLAGS = -Wall -g

 

make:  gcell gameboard golutilities   showgen  sim-tui  sim-gui cleanup 



gcell: gcell.h gcell.cpp
	$(CC) gcell.cpp -c $(CFLAGS)

gameboard: gameboard.h gameboard.cpp 
	$(CC) gameboard.cpp -c $(CFLAGS)

golutilities: golutilities.h golutilities.cpp 
	$(CC) golutilities.cpp  -c  $(CFLAGS)

showgen: showgen.cpp gameboard.o golutilities.o gcell.o
	$(CC) golutilities.o gameboard.o gcell.o -o showgen showgen.cpp  $(CFLAGS) $(LFLAGS)

sim-tui: sim-tui.cpp gameboard.o golutilities.o gcell.o
	$(CC) golutilities.o gameboard.o gcell.o -o sim-tui sim-tui.cpp $(CFLAGS) $(LFLAGS) -lncurses

sim-gui: 
	cd sim-gui/; make
#Utilities
cleanup: 
	/bin/rm -f *.o *~ out.aut

clean:
	/bin/rm -f *.o *~ out.aut sim-tui showgen cheatham-proj2.tar.gz
	
package: 
	tar -zcvf cheatham-proj2.tar.gz *
