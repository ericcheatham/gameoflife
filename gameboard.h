
#ifndef INCLUDES
#define INCLUDES

//== C headers ==//
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <getopt.h>

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

	public:
		//Gameboard(int xmax, int xmin, int ymax, int ymin,  int winx, int winy);
		Gameboard();
		~Gameboard();
		
		void setGridDimensions( int xmax, int xmin, int ymax, 
		int ymin, int winx, int winy );
		
		void setCellLive(int x, int y);
		void generateGeneration();
		std::string printDimensions();
		//void buildLifeFromFile(const std::string& str, const std::string& delim);
};



#endif
