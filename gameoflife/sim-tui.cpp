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


//#include "gameboard.h"
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
void updateResources(WINDOW * window, WINDOW * pad, int pminrow, int pmincol, int sminrow, int smincol, int smaxrow, int smaxcol)
{
	
	refresh();
	wclear(window);
	

	//window= newwin(LINES-3,COLS,2,0);
	wborder(window,'|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(window);
	prefresh(pad, pminrow,  pmincol, sminrow, smincol,  smaxrow,  smaxcol);

}



void displayGeneration(WINDOW * win, int yrange, int xrange, const char calive,  const char cdead, bool ** grid)
{	
	for(int y = 0; y < yrange; y++)
	{
			for(int x = 0; x < xrange; x++)
			{
				if(grid[y][x])
					mvwaddch(win,y+1, x+1, calive);
					//mvaddch(y+2, x, calive);
				else 
					mvwaddch(win,y+1, x+1, cdead);
					//mvaddch(y+2, x, cdead);
					
			}
	}	
}

void printBorder(int gen, int delay, const char * name, bool isPaused)
{
	mvprintw(0,(COLS/2)-(strlen(name)/2),"%s", name);
	mvprintw(1,0,"Delay(+/-):%5d", delay);
	mvprintw(1,COLS-strlen("Generation:     \0"),"Generation:%5d", gen);

	char arrows[] 	= " Arrows: Scroll\0";
	char play[] 	= "(P)lay \0";
	char pause[] 	= "(P)ause\0";
	char quit[] 	= "(Q)uit\0";
	char step[] 	= "(S)tep\0";
	
	if(isPaused)
	{
		mvprintw(LINES-1,0,quit);
		mvprintw(LINES-1,(COLS/2)-strlen(play),play);
		mvprintw(LINES-1,(COLS/2)+1,step);
		mvprintw(LINES-1,COLS-strlen(arrows),arrows);
	}
	else
	{	
		mvprintw(LINES-1,0,quit);
		mvprintw(LINES-1,(COLS/2)-strlen(play),pause);
		mvprintw(LINES-1,(COLS/2)+1,step);
		mvprintw(LINES-1,COLS-strlen(arrows),arrows);
	}
}	


int main(int argc, char ** argv)
{
	/*
		1) Read and construct stuff from file
		2) Draw bordering stuff in NCURSES
		3) Draw generation 0
		4) Listen for key input
			- Execute corresponding key command
			- Refresh resources, lowest first
				+ Border
				+ Window
				+ Pad
		5) GOTO 04
	*/
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

	std::string file;
  	 if (optind < argc) {
      		file = argv[optind];
   	 }


	Gameboard board(xmax, xmin, ymax, ymin, 
			xmax + abs(xmin)+1, ymax + abs(ymin)+1);		
 	if(!readFile(board, file))
 		return -1;
	
	bool ** grid = board.getBoard();
	int xrange = board.getXrange();
	int yrange = board.getYrange();
	
	
	// ==== NCURSES BEGIN ==== //
	WINDOW *gamewin, *gamepad;
	initscr();
	cbreak();			
	keypad(stdscr, TRUE);	
	nodelay(stdscr, TRUE); //non-blocking 	
	noecho();
	

	gamewin = newwin(LINES-3,COLS,2,0);
	wborder(gamewin, '|', '|', '-', '-', '+', '+', '+', '+');

	gamepad = newpad(yrange+1, xrange-2);
	int padx = 0, pady = 0;
	
	
	
	board.runSimulation(gen);
	grid = board.getBoard();
	std::string live(1, board.getliveChar());
	std::string dead(1, board.getdeadChar());
	
	printBorder(gen, delay, board.getSimName().c_str(), true);
	displayGeneration(gamepad,yrange, xrange, board.getliveChar(), board.getdeadChar(), grid);
	updateResources(gamewin, gamepad, pady, padx, 3,1, LINES-3, COLS-2);
	
	
	while(isRendering)
	{
		printBorder(gen, delay, board.getSimName().c_str(), true);
		ch = getch();  
		switch(ch)
		{
			case 'p':
				while(isPlaying)
				{			
					printBorder(++gen, delay, board.getSimName().c_str(),!isPlaying);
					ch = getch();
					if(ch == 'p' || ch == 'P')
						isPlaying = false;
					board.runSimulation(gen);
					grid = board.getBoard();
					displayGeneration(gamepad,yrange, xrange, board.getliveChar(), board.getdeadChar(), grid);
					if(ch == '+')
						delay++;
					if(ch == '-')
						delay--;	
					if(delay < 0) delay = 0;
					usleep(delay);
					updateResources(gamewin, gamepad, pady, padx, 3,1, LINES-3, COLS-2);
				}
				isPlaying = true;
				break;	
				
			case 's':
				board.runSimulation(++gen);
				grid = board.getBoard();
				displayGeneration(gamepad, yrange, xrange, board.getliveChar(), board.getdeadChar(), grid);
				updateResources(gamewin, gamepad, pady, padx, 3,1, LINES-3, COLS-2);
				break;	
			case '+':
				delay++;
				break;
			case '-':
				delay--;
				if(delay < 0) delay = 0;
				break;
			case KEY_RIGHT:
				if(padx-- < 0)	
					padx =0;				
				
				updateResources(gamewin, gamepad, pady, padx, 3,1, LINES-3, COLS-2);
				break;
			case KEY_LEFT:
				updateResources(gamewin, gamepad, pady, padx++, 3,1, LINES-3, COLS-2);
				break;
			case KEY_UP:
				updateResources(gamewin, gamepad, pady++, padx, 3,1, LINES-3, COLS-2);
				break;
			case KEY_DOWN:
				if(pady-- < 0)	
					pady =0;
				
				updateResources(gamewin, gamepad, pady, padx, 3,1, LINES-3, COLS-2);
				break;
			case 'q':
				isRendering = false;
				break;
			case 'Q':
				isRendering = false;
				break;
		}
	
		
	}

	//sleep(4);
	endwin();
	return 0;
}
