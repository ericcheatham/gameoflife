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
#include <algorithm>
#include <cctype>
#incldue <string>


#endif

#ifndef GOLUTILITIES_H
#define GOLUTILITIES_H

#include <algorithm>
#include "gameboard.h"
std::string trim(const std::string& str, const std::string& delim);

void readFile(Gameboard::Gameboard &g_ref , const std::string filename);
#endif
