#include "gameboard.h"

using namespace std;



/*
Gameboard::Gameboard(int xmax, int xmin, int ymax, int ymin,  int winx, int winy)
{
	setGridDimensions(xmax, xmin, ymax, ymin, winx, winy);
}
*/
Gameboard::Gameboard()
{
	/*
		This is the default constructor. It sets stuff to default. Shocking. 
	*/
	xmax_dim = xmin_dim = 0;
	ymax_dim = ymin_dim = 0;
	winx_dim = winy_dim = 0;
}

Gameboard::Gameboard(int xmax, int xmin, int ymax, int ymin, 
	int winx, int winy)
{

	/*
		1) Set grid dimensions at creation. 
			-Likely to be overwritten by file
			-Doesn't make board because reasons
	*/
	xmax_dim = xmax;
	xmin_dim = xmin;
	ymax_dim = ymax;
	ymin_dim = ymin;
	winx_dim = winx;
	winy_dim = winy;
}

Gameboard::~Gameboard()
{
	//delete grid[][];
}

void Gameboard::setGridDimensions( int  xmax, int   xmin, int   ymax,
		int  ymin,  bool wroteX, bool wroteY, int  winx, int  winy)
{
		if(wroteX)
		{
			xmax_dim = xmax;
			xmin_dim = xmin;
		}
		
		if(wroteY)
		{
			ymax_dim = ymax;
			ymin_dim = ymin;
		}
		
		//alive = '1';
		//dead = ' ';
		setGridDimensions( xmax_dim, xmin_dim, ymax_dim, ymin_dim, winx, winy);
}

bool Gameboard::setGridDimensions( int  xmax, int   xmin, int   ymax,
		int  ymin,  int  winx, int  winy)
{
	/*
		1) Set dimensions specified
		2) Convert min/max ==> actual useful ranges
			- NOTE: A range of -10 to 10 is actually 21 spaces. Remember the 0th!
		3) Create boards and temp stuff based on converted ranges
	*/
	xmax_dim = xmax;
	xmin_dim = xmin;
	ymax_dim = ymax;
	ymin_dim = ymin;
	winx_dim = winx;
	winy_dim = winy;

	xrange = xmax_dim + abs(xmin_dim)+1;
	yrange = ymax_dim + abs(ymin_dim)+1;
	//std::cout << xmax_dim<<endl;
	
	if(0 == xrange || 0 == yrange)
		return false;
		
	generateBoard(xrange, yrange);
	return true;
}

std::string Gameboard::printDimensions()
{
	/*
		1) Make string
		2) Print string
	*/
	std::stringstream ss;
	ss << "xrange: "<< xmax_dim << " - " << xmin_dim;
	return ss.str();

}

void Gameboard::setCellState(int x, int y, bool isLiving)
{
	/*
		1) Convert from human-readable index (+/- #) to computer index (0 -> N)
		2) Set spot described by coordinates to living;
	*/
	int loc_x = (int) floor((xrange/2.0)) + x;
	int loc_y = (int) floor((yrange/2.0)) - y;
	grid[loc_y][loc_x] = isLiving;
}

void Gameboard::setCellState(std::vector<string> &coords)
{
	/*
		1) Split string based on space
		2) Store x and y
		3) Use function that does the human readable < == > index coordinate
		4) Continue for all strings in the vector
			-pitches things with bad formatting 
	*/
	std::string y, x;
	for(std::vector<string>::iterator iter = coords.begin();
			iter != coords.end();
			iter++)
	{
		std::stringstream ss(*iter);
	
		getline(ss, y, ' ');
		getline(ss, x);

		setCellState( atoi(y.c_str()), atoi(x.c_str()), true);
		
	}

}

void Gameboard::setSimName(std::string &sn)
{
	simname = sn;
}

void Gameboard::setSimChars( char living, char notliving)
{
	alive = living;
	dead = notliving;
}

void Gameboard::generateBoard(int xrange, int yrange)
{
	/*
		1) malloc arrays of pointers for the 'y' of the board and the gen storage
		2) malloc each of the 'x' rows 
		3) 'zero' the boards to 'clear' the memory
		
	*/
	grid = ( bool ** ) malloc (yrange * sizeof(bool * ));
	output = ( bool ** ) malloc (yrange * sizeof(bool * ));
		
	for( int i  = 0; i < yrange ; i++)
	{
		grid[i] = (bool *) malloc ( xrange * sizeof(bool));
		output[i] = (bool *) malloc ( xrange * sizeof(bool));
	}

	for(int i = 0; i < yrange ; i++)
		for( int j = 0; j < xrange ; j++)
		{
			grid[i][j]=false;	
			output[i][j]=false;
		}
		
}

void Gameboard::runSimulation(int numGenerations)
{
	/*
		1) For each cell in the current grid
			- Count neighbors
			- Set to alive or dead pursant to Conway's Game of Life rules
		2) Copy results back to main grid
		3) Repeat 1 & 2 for as many generations as needed
	*/
	int i = 0; 
	int test = 0;
	while(i != numGenerations && numGenerations != 0)
	{
		for(int y = 0; y < yrange ; y++)
		{
			for(int x = 0; x < xrange; x++)
			{	
				test  = countAdjLiving(x, y);
				//cout<<test<<" ";
				if(grid[y][x])//alive
				{
					if( 2 > test) output[y][x] = false; //under-population: death
					if( 2 == test || 3 == test){ output[y][x] = true; }//Lives on
					if( 3 < test) output[y][x] = false; //overcrowding: death
				}
				else//Soylent grid is pixels
					if(3 == test) {  output[y][x] = true; }//3 people to give birth? 			
			}
		}	
		for(int y = 0; y < yrange ; y++)
		{
			for(int x = 0; x < xrange; x++)
			{	
				grid[y][x] = output[y][x];
			}
		}		
		i++;
	}
}

int Gameboard::countAdjLiving(int px , int py)
{
	/*
		1) Bound values to the 8 cell directly touching it
			-clamp inside the GRID size, not the WINDOW size
		2) For each cell that is inside that 3x3 (or less) selection
			- count if it is live
			- ignore if origin or dead
		3) Return count
	*/
	int liveCount = 0;
	int x = px-1, y = py -1;
		
	if(py - 1 < 0) y = 0;
	if(px -1 < 0) x = 0;

	int xsave = x;
	for(; y < py + 2 && y < yrange; y++)
	{
		for(x = xsave  ; x<  px + 2 && x < xrange; x++)
		{			
			if(px == x && py == y){	}	
			else
				if(grid[y][x]) { liveCount++;}
		}
	}
	return liveCount;
}

//Getters
bool ** Gameboard::getBoard()
{
	return grid;
}

int Gameboard::getXrange()
{
	return xrange;
}

int Gameboard::getYrange()
{
	return yrange;
}


const char Gameboard::getliveChar()
{
	return alive;
}

const char Gameboard::getdeadChar()
{
	return dead;
}

const std::string Gameboard::getSimName()
{
	return simname;
}


//Printers
void Gameboard::printAscii()
{
	/*
		1) Step through each row and print what we see
	*/
	for(int y = 0; y < yrange ; y++)
	{
		for(int x = 0; x < xrange; x++)
		{	
			if(!grid[y][x])
				cout<<"~";
			else
				cout<<grid[y][x];
		}
		cout<<endl;
	}
}

void Gameboard::printToFile()
{
	/*
		1) Open file stream
		2) Build range data strings and write to file
		3) Iterate through simulation data
			-Write only if line actually has at least one active cell
		
		Note: All the important crap on its own line. Like a normal human would prefer.
		
	*/
	
	std::ofstream outfile;
	outfile.open("out.aut");
	outfile<<"#Generated by showgen\n\n";
	outfile<<"Xrange "<<xmin_dim << " " <<xmax_dim <<";\n";
	outfile<<"Yrange "<<ymin_dim << " " <<ymax_dim <<";\n";
	outfile<<"Initial{\n";
	
	
	int pos;
	std::stringstream  ss;
	for(int y = 0; y < yrange ; y++)
	{
		pos = (int)(y - floor(yrange/2.0));
		ss << "Y = " << pos << ":";

		
		for(int x = 0; x < xrange; x++)
		{	
			if(grid[y][x])
				ss<< (int)(x - floor(xrange/2.0)) <<", ";
		}
		
		if(!ss.str().empty())
			if(*ss.str().rbegin() != ':')
				outfile<<ss.str()<<endl;
		ss.clear();
		ss.str("");
	}
	
	outfile<<"};\n";

}
