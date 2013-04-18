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
	
	//std::cout << xmax_dim<<endl;
}

std::string Gameboard::printDimensions()
{
	
	std::stringstream ss;
	ss << "xrange: "<< xmax_dim << " - " << xmin_dim;
	return ss.str();

}

void Gameboard::setCellLive(int x, int y)
{
	std::cout<< "(" <<x << ", " << y << ")"<< endl;
}


