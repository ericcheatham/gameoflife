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


//#include "gameboard.h"
#include "golutilities.h"

using namespace std;

void printHelp()
{
	cout << "CS229 Conway's Game of Life Simulation\n\n";
	cout << "A simple ascii based version of Conway's game of life. \n";
	cout << "Useage:\n";
	cout << "	./showgen -aghijkl --tx --tw --wx --wy [filename]\n\n";
	cout << "Common options:\n";
	cout << "-a: Forces ASCII output to stdout. If omitted, prints information in an .aut file\n";
	cout << "-g: Specify how many generations to complete. Default is 0 generations\n";
	cout << "-h: Displays this help window\n";
	cout << "-i: Define simulation x-range\n";
	cout << "-j: Define simulation y-range\n";
	cout << "-k: Define sim window x-range\n";
	cout << "-l: Define sim window y-range\n";
	cout << "-tx: Same as '-i'\n";
	cout << "-ty: Same as '-j'\n";
	cout << "-wx: Same as '-k'\n";
	cout << "-wy: Same as '-l'\n\n";
	
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
		{"wx", 1, 0, 'k'},
		{"wy", 1, 0, 'l'},
		{NULL, 0, NULL, 0}	
	};


	int c = 0 , option_index = 0;
	int gen = 0;
	int xmax = 0, xmin = 0, ymax = 0, ymin = 0;
	int wxl =0, wxh = 0, wyl = 0, wyh = 0;
	int wx = 0, wy = 0;
	bool useX = true, useY = true;
	bool printAscii = false;
	
	
	while ( ( c = getopt_long(argc, argv, "ag:hi:j:k:l:", 
			long_options, &option_index)) != -1)
	{
	
		switch (c)
        	{
           		 case 'g':
				gen = atoi(optarg);
				break;
			case 'a':
				printAscii = true;	
				break;
			case 'i':
				readArg(optarg, xmin, xmax);
				break;	
			case 'j':
				readArg(optarg, ymin, ymax);
				break;
			case 'k':	
				useX = false;
				readArg(optarg, wxl, wxh);	
				break;
			case 'l':
				useY =  false;
				readArg(optarg, wyl, wyh);
				break;	
			case 'h':
				printHelp();				
				return 0 ;
		}
	}


	gen = (gen > 0) ? gen : 0;
	std::string file;
  	 if (optind < argc) {
      		file = argv[optind];
   	 }

	
	Gameboard board(xmax, xmin, ymax, ymin,wx, wy);
 	if(!readFile(board, file))
 		return -1;
	
	board.runSimulation(gen);
	

	if(!board.getSimRule().compare("ConwaysLife"));
	//	board.runSimulationConway(gen);
	else if(!board.getSimRule().compare("BriansBrain"));
	//	board.runSimulationBrian(gen); 
	else if(!board.getSimRule().compare("WireWorld"));
	//	board.runSimulationWire(gen);
	else if(!board.getSimRule().compare("LangtonsAnt"));
	//	board.runSimulationAnt(gen);
	else
	{
		cout<<"Invalid simulation rule:" <<board.getSimRule()<<endl;
		return -1;
	}

	//cout<<board.getSimRule()<<endl;

	//wx = (useX) ? board.getXrange() : (abs(wxl) + wxh + 1);
	//wy = (useY) ? board.getYrange() : (abs(wyl) + wyh + 1);
	if(useX)
	{
		wxl = -(int) floor(board.getXrange()/2.0);
		wxh = (int) floor(board.getXrange()/2.0);
	}
	if(useY)
	{
		wyl = -(int) floor(board.getYrange()/2.0); 
		wyh = (int) floor(board.getYrange()/2.0);
	}
	//cout<<wyl;

	if(printAscii)	
		board.printAscii(wxl , wxh,  wyl  , wyh );
	else
		board.printToFile(wxl , wxh,  wyl  , wyh );
}
