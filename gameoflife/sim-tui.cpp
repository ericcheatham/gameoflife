/*
 *showgen.cc
 * -reads a single .aut file and generates the Nth Generation of the corresponding Game of Life board  
 * 
 * Inputs:
 * 	./showgen -agh --tx --tw --wx --wy [filename]
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
	box(win, 0 , 0);	
	//wborder(win, '|', '|', '-', '-', '+', '+', '+', '+'); 
	//win = newwin(20, 40,0,0);
	
	wrefresh(win);
	
	return win;
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
	
	
	
	// ==== NCURSES BEGIN ==== //

	
	initscr();
	cbreak();
	//printw("--- Test ---");
	WINDOW *game_win = newwin(20, 20, 0, 0);
	
	wrefresh(game_win);
	//game_win = newwin( 20, 40, 10, 20);
	//box( game_win, ACS_VLINE, ACS_HLINE );
	//refresh();
	//board.runSimulation(0);
	
	getchar();
	endwin();
}
