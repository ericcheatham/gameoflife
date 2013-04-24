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


 
int main(int argc, char ** argv)
{
	static struct option long_options[] = {
		{"tx", 1, 0, 'i'},
		{"ty", 1, 0, 'j'},
		{"wx", 1, 0, 'k'},
		{"wy", 1, 0, 'l'},
		{NULL, 0, NULL, 0}	
	};


	int c = 0 , option_index = 0;
	int gen = 0;
	while ( ( c = getopt_long(argc, argv, "ag:hi:j:k:l:", 
			long_options, &option_index)) != -1)
	{
		switch (c)
        {
            case 'g':
				gen = atoi(optarg);
		}
	}


	gen = (gen > 0) ? gen : 0;
	std::string file("test.aut");
	
	Gameboard::Gameboard board;
 	readFile(board, file);
 	//std::cout << board.printDimensions() << std::endl;
	board.runSimulation(gen);
}
