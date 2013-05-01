#ifndef INCLUDES
#define INCLUDES

//== C headers ==//
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>

//== C++ headers ==//
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <string>
#include <algorithm>


class Gameboard
{
	char alive, dead;
	int xmax_dim ,xmin_dim;
	int ymax_dim ,ymin_dim; 
	int winx_dim ,winy_dim;
	int xrange, yrange;
	
	
	
	std::string simname;

	bool ** grid; //malloc when needed;
	bool ** output;
	
	public:
		//Gameboard();
		Gameboard(int xmax_dim, int xmin_dim, int ymax_dim, int ymin_dim,int winx_dim, int winy_dim);
		~Gameboard();
		
		void setGridDimensions( int xmax, int xmin, int ymax, int ymin, bool wroteX, bool wroteY, int  winx, int  winy);
			
		bool setGridDimensions( int xmax, int xmin, int ymax, int ymin, int winx, int winy );
		
		void setSimName(std::string & simname);
		void setCellState(int x, int y, bool isLiving);
		void setCellState(std::vector<std::string> &coords);
		void runSimulation(int numGenerations);
		void setSimChars( char living, char notliving);
		
		
		//Getters
		bool ** getBoard();
		int getXrange();
		int getYrange();
		const std::string getSimName();
		const char getliveChar();
		const char getdeadChar();

		
		//printing functions
		void printAscii(int wxl, int  wxh , int wyl  ,int wyh);
		void printToFile(int wxl, int  wxh , int wyl  ,int wyh);
		
		//User debug utilities 
		std::string printDimensions();
	
	private:
		void generateBoard(int xrange, int yrange);
		int countAdjLiving(int px , int py);	
};
#endif

