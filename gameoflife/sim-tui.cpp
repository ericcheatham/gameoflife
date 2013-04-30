/*
 *showgen.cc
 * -reads a single .aut file and generates the Nth Generation of the corresponding Game of Life board  
 * 
 * Inputs:
 * 	./sim-tui -agh --tx --ty  [filename]
 *
 * Defaults
 *  	output - ASCII
 *  	generation - 0
 *  	x range - spec by .aut file
 *  	y range - spec by .aut file
 *  	window x - x range
 *  	window y - y range
 *
*/


#include "gameboard.h"
#include "golutilities.h"

#include <ncurses.h>

using namespace std;


// ==== File User interface stuff ====// 
void printHelp()
{
	cout << "CS229 Conway's Game of Life Simulation\n\n";
	cout << "Text User Interface (TUI) for Conway's Game of Life Simulator \n";
	cout << "Useage:\n";
	cout << "	./showgen -hij --tx --tw  [filename]\n\n";
	cout << "Common options:\n";
	cout << "-h: Displays this help window\n";
	cout << "-i: Define simulation x-range\n";
	cout << "-j: Define simulation y-range\n";
	cout << "-tx: Same as '-i'\n";
	cout << "-ty: Same as '-j'\n";

	
	cout << "[filename]: a file of type .aut specifing, at the very least, starting coordinates\n";

	cout << "The options specified by [ijlk]|tx|ty|wx|wy will default";
	cout << " to the values defined in [filename]\n";
	
	


}	
 
void readArg(const char * optarg, int &min, int &max)
{
	/*
		1) Read optarg string
		2) Split and store based on ',' 
			- if it can't work, doesn't matter, it's zero'ed anyway. 
	*/
	std::stringstream ss(optarg);
	std::string str(optarg);
	
	getline(ss, str , ',');
	min = atoi(str.c_str());
	getline(ss, str , ',');
	max = atoi(str.c_str());
}


// ==== NCURSES ==== //
WINDOW *draw_window(int winw, int winh, int startx, int starty)
{
	/*
		1) Generate window of size 40 columns x 20 height
		2) Refresh screen to draw window
		3) Return window to manipulate elsewere
	*/
	WINDOW * win;

	win = newwin(winh, winw, starty, startx);//Needed?
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+'); 
	//win = newwin(20, 40,0,0);
	
	wrefresh(win);
	
	return win;
}



void displayGeneration(int yrange, int xrange, const char calive,  const char cdead, bool ** grid)
{
	char alive[2];
	char dead[1];
	alive[0] = calive;
	alive[1] = '\0';
	dead[0] = cdead;
	
	for(int y = 0; y < yrange; y++)
	{
			for(int x = 0; x < xrange; x++)
			{
				if(grid[y][x])
					mvaddch(y+2, x, calive);
					//mvprintw( y+2, x,"%s",  alive);	
				else 
					mvaddch(y+2, x, cdead);
					//mvprintw( y+2, x, " ");
			}
	}	
}

void printBorder(int gen, int delay, const char * name, bool isPaused)
{
	mvprintw(0,20-(strlen(name)/2),"%s", name);
	mvprintw(1,0,"Delay(+/-):%5d", delay);
	mvprintw(1,25,"Generation:%5d", gen);

	
	if(isPaused)
		mvprintw(41,0,"(Q)uit    (P)lay     (S)tep     Arrows: Scroll");
	else
		mvprintw(41,0,"(Q)uit    (P)ause     (S)tep     Arrows: Scroll");
}	


int main(int argc, char ** argv)
{
	static struct option long_options[] = {
		{"tx", 1, 0, 'i'},
		{"ty", 1, 0, 'j'},
		{NULL, 0, NULL, 0}	
	};

	int c = 0 , option_index = 0;
	int xmax = 0, xmin = 0, ymax = 0, ymin = 0;
	
	int ch = 0, gen = 0, delay = 100;
	bool isRendering = true, isPlaying = true;
	
	while ( ( c = getopt_long(argc, argv, "ag:hi:j:k:l:", 
			long_options, &option_index)) != -1)
	{
	
		switch (c)
        {
			case 'i':
				readArg(optarg, xmin, xmax);
				break;	
			case 'j':
				readArg(optarg, ymin, ymax);
				break;
			case 'h':
				printHelp();				
				return 0 ;
		}
	}

	std::string file("test.aut");

	Gameboard::Gameboard board(xmax, xmin, ymax, ymin, 
			xmax + abs(xmin)+1, ymax + abs(ymin)+1);		
 	readFile(board, file);
	
	bool ** grid = board.getBoard();
	int xrange = board.getXrange();
	int yrange = board.getYrange();
	
	
	// ==== NCURSES BEGIN ==== //
	
	initscr();
	cbreak();			
	keypad(stdscr, TRUE);	
	nodelay(stdscr, TRUE); //non-blocking 	
	noecho();
	
	WINDOW * gamewin = draw_window(20, 40, 0, 2);
	wrefresh(gamewin);
	
	
	board.runSimulation(gen++);
	grid = board.getBoard();
	std::string live(1, board.getliveChar());
	std::string dead(1, board.getdeadChar());
	displayGeneration(yrange, xrange, board.getliveChar(), board.getdeadChar(), grid);
	
	while(isRendering)
	{
		printBorder(gen, delay, board.getSimName().c_str(), true);
		ch = getch();  
		switch(ch)
		{
			case 'p':
				while(isPlaying)
				{			
					printBorder(gen, delay, board.getSimName().c_str(),!isPlaying);
					ch = getch();
					if(ch == 'p' || ch == 'P')
						isPlaying = false;
					board.runSimulation(gen++);
					grid = board.getBoard();
					displayGeneration(yrange, xrange, board.getliveChar(), board.getdeadChar(), grid);
					if(ch == '+')
						delay++;
					if(ch == '-')
						delay--;	
					if(delay < 0) delay = 0;
					usleep(delay);
					refresh();	
				}
				isPlaying = true;
				break;	
				
			case 's':
				board.runSimulation(gen++);
				grid = board.getBoard();
				displayGeneration(yrange, xrange, board.getliveChar(), board.getdeadChar(), grid);
				refresh();	
				break;	
			case '+':
				delay++;
				break;
			case '-':
				delay--;
				if(delay < 0) delay = 0;
				break;
			case 'q':
				isRendering = false;
				break;
			case 'Q':
				isRendering = false;
				break;
		}
	
		
	}
	endwin();
	return 0;
}
