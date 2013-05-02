#include "gameboard.h"
#include "gcell.h"

using namespace std;



/*
Gameboard::Gameboard(int xmax, int xmin, int ymax, int ymin,  int winx, int winy)
{
	setGridDimensions(xmax, xmin, ymax, ymin, winx, winy);
}

Gameboard::Gameboard()
{
	
		//This is the default constructor. It sets stuff to default. Shocking. 
	
	xmax_dim = xmin_dim = 0;
	ymax_dim = ymin_dim = 0;
	winx_dim = winy_dim = 0;
}
*/
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
	cgrid[loc_y][loc_x].setCWState( isLiving );
	cgrid[loc_y][loc_x].setBBState( isLiving );
	cgrid[loc_y][loc_x].setWWState( isLiving );


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

void Gameboard::setSimRule(std::string &sr)
{
	simrule = sr;
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
	
	cgrid 	= ( Gcell ** ) malloc (yrange * sizeof(Gcell * ));
	coutput = ( Gcell ** ) malloc (yrange * sizeof(Gcell * ));
		
	for( int i  = 0; i < yrange ; i++)
	{
		grid[i] = (bool *) malloc ( xrange * sizeof(bool));
		output[i] = (bool *) malloc ( xrange * sizeof(bool));
	}

	for( int i  = 0; i < yrange ; i++)
	{
		cgrid[i]   = (Gcell *) malloc ( xrange * sizeof(Gcell));
		coutput[i] = (Gcell *) malloc ( xrange * sizeof(Gcell));
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


void Gameboard::runSimulationAnt(int numGenerations){}

void Gameboard::runSimulationBrian(int numGenerations)
{
	/*
		1) For each cell in the current grid
			- Count neighbors
			- Set to alive or dead pursant to Conway's Game of Life rules
		2) Copy results back to main grid
		3) Repeat 1 & 2 for as many generations as needed
	*/
	int i = 0; 
	int liveCount = 0;
	int px = 0,py = 0;
		
	while(i != numGenerations && numGenerations != 0)
	{
		for(int y = 0; y < yrange ; y++)
		{
			for(int x = 0; x < xrange; x++)
			{	
				//set bounds
				px = x-1;
				py = y -1;
				if(py  < 0) py = 0;
				if(px  < 0) px = 0;

				
				int xsave = x -1;
				for(int a = py -1; a < py + 2 && a < yrange; a++)
				{
					for(int b = xsave  ; b<  px + 2 && b < xrange; b++)
					{			
						if(px == x && py == y){	}	
						else
							if(cgrid[y][x].getBBState()==2) { liveCount++;}
					}
				}
				cout<<liveCount<<" \n"; 
				if(coutput[y][x].getBBState()==0)//Dead
				{
					if( 2 == liveCount) coutput[y][x].setBBState(2); //2 heads are better than, everything
					else 
						coutput[y][x].setBBState(0);
				}
				
				else if(coutput[y][x].getBBState()== 1)//Dying -> Dead 
						coutput[y][x].setBBState(0);
				else if(coutput[y][x].getBBState()== 2)//Alive -> Dying 
						coutput[y][x].setBBState(1);

				liveCount = 0;
			}
		}	
		
		for(int y = 0; y < yrange ; y++)
		{
			for(int x = 0; x < xrange; x++)
			{	
				cgrid[y][x] = coutput[y][x];
			}
		}		
		i++;
	}
}


void Gameboard::runSimulationConway(int numGenerations)
{
	/*
		1) For each cell in the current grid
			- Count neighbors
			- Set to alive or dead pursant to Conway's Game of Life rules
		2) Copy results back to main grid
		3) Repeat 1 & 2 for as many generations as needed
	*/
	int i = 0; 
	int liveCount = 0;
	int px = 0,py = 0;
		
	while(i != numGenerations && numGenerations != 0)
	{
		for(int y = 0; y < yrange ; y++)
		{
			for(int x = 0; x < xrange; x++)
			{	
				//set bounds
				px = x-1;
				py = y -1;
				if(py  < 0) py = 0;
				if(px  < 0) px = 0;

				
				int xsave = x -1;
				for(int a = py -1; a < py + 2 && a < yrange; a++)
				{
					for(int b = xsave  ; b<  px + 2 && b < xrange; b++)
					{			
						if(px == x && py == y){	}	
						else
							if(cgrid[y][x].getCWState()) { liveCount++;}
					}
				}
				
				cout<<liveCount<<" ";
				if(coutput[y][x].getCWState())//alive
				{
					if( 2 > liveCount) coutput[y][x].setCWState(false); //under-population: death
					if( 2 == liveCount || 3 == liveCount){ coutput[y][x].setCWState(true); }//Lives on
					if( 3 < liveCount) coutput[y][x].setCWState(false); //overcrowding: death
				}
				else//Soylent grid is pixels
					if(3 == liveCount) {  coutput[y][x].setCWState(true); }//3 people to give birth? 					

				liveCount = 0;
			}
			cout<<endl;
		}	

		for(int y = 0; y < yrange ; y++)
		{
			for(int x = 0; x < xrange; x++)
			{	
				cgrid[y][x] = coutput[y][x];
			}
		}		
		i++;
	}
}


void Gameboard::runSimulationWire(int numGenerations)
{
	/*
		1) For each cell in the current grid
			- Count neighbors
			- Set to alive or dead pursant to Conway's Game of Life rules
		2) Copy results back to main grid
		3) Repeat 1 & 2 for as many generations as needed
	*/
	int i = 0; 
	int liveCount = 0;
	int px = 0,py = 0;
		
	while(i != numGenerations && numGenerations != 0)
	{
		for(int y = 0; y < yrange ; y++)
		{
			for(int x = 0; x < xrange; x++)
			{	
				//set bounds
				px = x-1;
				py = y -1;
				if(py  < 0) py = 0;
				if(px  < 0) px = 0;

				
				int xsave = x -1;
				for(int a = py -1; a < py + 2 && a < yrange; a++)
				{
					for(int b = xsave  ; b<  px + 2 && b < xrange; b++)
					{			
						if(px == x && py == y){	}	
						else
							if(cgrid[y][x].getCWState()) { liveCount++;}
					}
				}

				
				if(coutput[y][x].getWWState()==0)//empty
					coutput[y][x].setWWState(0); 
				else if(coutput[y][x].getWWState()== 1)//head -> tail
						coutput[y][x].setWWState(2);
				else if(coutput[y][x].getWWState()== 2)//tail -> wire
						coutput[y][x].setBBState(3);	
				else if(coutput[y][x].getWWState()== 3)//is wire
				{
					if( 2 == liveCount || 1 == liveCount) coutput[y][x].setWWState(1); //2 Or 1, lets have fun
					else 
						coutput[y][x].setBBState(3); //iWire
				}				
				
				liveCount = 0;
			}
		}	
		
		for(int y = 0; y < yrange ; y++)
		{
			for(int x = 0; x < xrange; x++)
			{	
				cgrid[y][x] = coutput[y][x];
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

const std::string Gameboard::getSimRule()
{
	return simrule;
}


//Printers
void Gameboard::printAscii(int wxl =0, int  wxh = 0, int wyl = 0 ,int wyh = 0)
{
	/*
		1) Step through each row and print what we see
	*/
	int xmax = (int) floor((xrange/2.0)) + wxh;
	int xmin=  (int) floor((xrange/2.0)) + wxl;
	int ymax = (int) floor((yrange/2.0)) - wyh;
	int ymin = (int) floor((yrange/2.0)) - wyl;

	
	for(int y = 0; y < yrange  ; y++)
	{
		for(int x = 0; x < xrange ; x++)
		{	
			/*
			if(x >= xmin && x <= xmax && y <= ymin && y >= ymax)
			{
				if(!getSimRule().compare("ConwaysLife"))
				{
					if(!cgrid[y][x].getCWState())
						cout<<"~";
					else
						cout<<cgrid[y][x].getCWState();
				}
				else if(!getSimRule().compare("BriansBrain"))
				{
					cout<<cgrid[y][x].getBBState();
				}
				else if(!getSimRule().compare("WireWorld"))
				{
					cout<<cgrid[y][x].getWWState();
				}
				else if(!getSimRule().compare("LangstonsAnt"))
				{
					cout<<"Not yet implemented\n";
					return;
				}
			}
			*/
			if(!grid[y][x])
				cout<<"~";
			else
				cout<<grid[y][x];
		}
		if((y <= ymin && y >= ymax))
			cout<<endl;
	}
}

void Gameboard::printToFile(int wxl =0, int wxh = 0, int wyl = 0 ,int wyh = 0)
{
	/*
		1) Open file stream
		2) Build range data strings and write to file
		3) Iterate through simulation data
			-Write only if line actually has at least one active cell
		
		Note: All the important crap on its own line. Like a normal human would prefer.
		
	*/
	
	int xmax = (int) floor((xrange/2.0)) + wxh;
	int xmin=  (int) floor((xrange/2.0)) + wxl;
	int ymax = (int) floor((yrange/2.0)) - wyh;
	int ymin = (int) floor((yrange/2.0)) - wyl;
	
	std::ofstream outfile;
	outfile.open("out.aut");
	outfile<<"#Generated by showgen\n\n";
	outfile<<"Xrange "<<wxl << " " <<wxh<<";\n";
	outfile<<"Yrange "<<wyl<< " " <<wyh<<";\n";
	outfile<<"Initial{\n";
	
	
	int pos;
	std::stringstream  ss;
	for(int y = 0; y < yrange ; y++)
	{
		if((y <= ymin && y >= ymax))
		{
			pos = (int)(y - floor(yrange/2.0));
			ss << "Y = " << pos << ":";
	
			for(int x = 0; x < xrange; x++)
			{	
				if(x >= xmin && x <= xmax && y <= ymin && y >= ymax)
					if(grid[y][x])
						ss<< (int)(x - floor(xrange/2.0)) <<", ";
			}
		
			if(!ss.str().empty())
				if(*ss.str().rbegin() != ':')
					outfile<<ss.str()<<";"<<endl;
			ss.clear();
			ss.str("");
		}
	}
	
	outfile<<"};\n";

}
