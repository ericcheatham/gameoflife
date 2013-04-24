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
	xmax_dim = xmin_dim = 0;
	ymax_dim = ymin_dim = 0;
	winx_dim = winy_dim = 0;
}

Gameboard::~Gameboard()
{
	//delete grid[][];
}

void Gameboard::setGridDimensions( int xmax, int xmin, int ymax,
		int ymin,  int winx, int winy)
{
	xmax_dim = xmax;
	xmin_dim = xmin;
	ymax_dim = ymax;
	ymin_dim = ymin;
	winx_dim = winx;
	winy_dim = winy;

	xrange = xmax_dim + abs(xmin_dim)+1;
	yrange = ymax_dim + abs(ymin_dim)+1;
	//std::cout << xmax_dim<<endl;
	generateBoard(xrange, yrange);
}

std::string Gameboard::printDimensions()
{
	
	std::stringstream ss;
	ss << "xrange: "<< xmax_dim << " - " << xmin_dim;
	return ss.str();

}

void Gameboard::setCellState(int x, int y, bool isLiving)
{
	//grid[yrange][xrange]
	int loc_x = (int) floor((xrange/2.0)) + x;
	int loc_y = (int) floor((yrange/2.0)) - y;
	grid[loc_y][loc_x] = isLiving;
}

void Gameboard::setCellState(std::vector<string> &coords)
{
	//cout<<endl;
	std::string y, x;
	for(std::vector<string>::iterator iter = coords.begin();
			iter != coords.end();
			iter++)
	{
		std::stringstream ss(*iter);
		
		//cout<<*iter<<endl;
		
	
		getline(ss, y, ' ');
		getline(ss, x);
	    //cout<< y << " " << x<< endl;
		setCellState( atoi(y.c_str()), atoi(x.c_str()), true);
		
	}
	
	//int yrange =ymax_dim + abs(ymin_dim);
	//int xrange =xmax_dim + abs(xmin_dim);
	
	
}


void Gameboard::generateBoard(int xrange, int yrange)
{
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
/*

	for( int i  = 0; i < yrange ; i++)
		output[i] = (bool *) malloc ( xrange * sizeof(bool));
	
	for(int i = 0; i < yrange ; i++)
		for( int j = 0; j < xrange ; j++)
			output[i][j]=false;*/
}

void Gameboard::runSimulation(int numGenerations)
{
			
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
				else 
					if(3 == test) {  output[y][x] = true; }//3 people to give birth? 			
			}
			//cout<<endl;
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
									
	for(int y = 0; y < yrange ; y++)
	{
		for(int x = 0; x < xrange; x++)
		{	
			//if(i != 0 )
				//grid[y][x] = output[y][x];
			if(!grid[y][x])
				cout<<"~";
			else
				cout<<grid[y][x];
		}
		cout<<endl;
	}
}

int Gameboard::countAdjLiving(int px , int py)
{
	int liveCount = 0;
	int x = px-1, y = py -1;
		
	if(py - 1 < 0) y = 0;
	//if(py + 1 > yrange+1) y = yrange-1;
	if(px -1 < 0) x = 0;
	//if(px + 1 > xrange+1) x = xrange-1;


	int xsave = x;
	//cout<<" Position ( "<< px << " " << py << ")\n";
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
