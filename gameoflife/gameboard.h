
#ifndef INCLUDES
#define INCLUDES

//== C headers ==//
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>

//== C++ headers ==//
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#endif

#ifndef GOLBUILDER_H
#define GOLBUILDER_H

class Gameboard
{
	int xmax_dim ,xmin_dim;
	int ymax_dim ,ymin_dim; 
	int winx_dim ,winy_dim;

	int xrange, yrange;

	bool ** grid; //malloc when needed;
	bool ** output;
	
	public:
		Gameboard();
		~Gameboard();
		
		void setGridDimensions( int xmax, int xmin, int ymax, 
		int ymin, int winx, int winy );
		
	
		void setCellState(int x, int y, bool isLiving);
		void setCellState(std::vector<std::string> &coords);
		void runSimulation(int numGenerations);
		std::string printDimensions();
	
	private:
		void generateBoard(int xrange, int yrange);
		int countAdjLiving(int px , int py);	
};

#endif
