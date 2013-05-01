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

#include <QApplication>
#include <QLabel>

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

	Gameboard board(xmax, xmin, ymax, ymin, 
			xmax + abs(xmin)+1, ymax + abs(ymin)+1);		
 	readFile(board, file);
	
	bool ** grid = board.getBoard();
	int xrange = board.getXrange();
	int yrange = board.getYrange();
	
	
	
	board.runSimulation(gen);
	grid = board.getBoard();
	std::string live(1, board.getliveChar());
	std::string dead(1, board.getdeadChar());
	
	QApplication app(argc, argv);
	QLabel *label = new QLabel("Hola, Qt!");
	label -> show();
	return app.exec();
	
	
	//return 0;
}
