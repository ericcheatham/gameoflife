#include "gameboard.h"
#include "golutilities.h"



using namespace std;


std::string trim(const std::string& str, const std::string& delim = " \t")
{

	std::string t_string;
	size_t t_start = str.find_first_not_of(delim);
	if(t_start == std::string::npos)
		return "";
	else
	{
		 t_string = str.substr(t_start, str.find_last_not_of(delim)+1);
	}
	return t_string;
}

void readFile(Gameboard::Gameboard &g_ref , const std::string filename) 
{
	/*
	1) Open file
	2) Read file line by line
		- Tokenize the line? Yeah!
	3) Step through line with following specs
		- ignore the remainder of a line after '#'
		- Look for the keywords: Xrange, Yrange, Initial
		- Bogus keywords: Print some form of error 
		- Absorb crap until ';'
		- Additional information can be stored between {}
	4) Build GOL object
	*/

	std::string line, temp2;
	std::vector<std::string> line_vec;
	std::ifstream file( filename.c_str(), ifstream::in );
	//char * split;
	//LifeObject::LifeObject * newlife = new LifeObject;

	if( file ){	
		while( std::getline( file, line ) )
		{	
			line = line.substr(0, line.find("#"));
			std::replace(line.begin(), line.end(), '{', ';');
			std::replace(line.begin(), line.end(), '	', ' ');
			
		
		
			while(line.find(";") == std::string::npos && line.length() > 0 )
			{
								
				std::getline(file,temp2);
				temp2 = temp2.substr(0, temp2.find("#"));
				line += " " +temp2;
			}
			
			if(line.length() != 0)
			{
				std::stringstream ss(line);
				while(getline(ss, temp2, ';'))
				{
					//cout<<temp2<<endl;
					std::string test = trim(temp2);
					if(test.length() != 0)
						line_vec.push_back( test );
					//line_vec.push_back( temp2 );
				}
				
			}		
		}
	}
	else{
		cout << "File error. Please check your inputs...\n" <<endl;	
	}	
	
	int xrangemin = 0, xrangemax = 0;
	int yrangemin = 0, yrangemax = 0;
	bool wroteXR = false, wroteYR = false;
	std::string y, x;
	std::vector<std::string> coordinates;
	for(std::vector<string>::iterator iter = line_vec.begin();
			iter != line_vec.end();
			iter++)
	{
		std::stringstream ss(*iter), convert;
		//cout<<*iter<<endl;
		
		while(getline(ss, temp2, ' '))
		{
			//cout<<temp2<<endl;
			if(!temp2.compare("Xrange"))
			{	
				getline(ss,temp2, ' ');
				xrangemin = atoi(temp2.c_str());
				
				getline(ss,temp2, ' ');
				xrangemax = atoi(temp2.c_str());
				
				wroteXR = true;
			}
			if(!temp2.compare("Yrange"))
			{	
				getline(ss,temp2, ' ');
				yrangemin = atoi(temp2.c_str());
				
				getline(ss,temp2, ' ');
				yrangemax = atoi(temp2.c_str());
				
				wroteYR = true;
			}
			else if(!temp2.compare("Initial"))
			{
				
				iter++; //next string since it should be the next thing in the vector
				std::stringstream ss2(*iter);
					
				while(ss2.str().compare("}") != 0)
				{	
					std::string thing, thing2;
					std::string y, x;
					int side = 0;		
					while(getline(ss2, thing , ':'))
					{
						thing.erase(std::remove_if(thing.begin(), thing.end(), (int(*)(int))isspace), thing.end());
						if(side == 0)
						{
							size_t last_index = thing.find_last_not_of("-0123456789");
							y = thing.substr(last_index + 1);
							//cout<<y<<endl;
							side++;
						}
						else
						{
							std::stringstream ssi(thing);
							while(getline(ssi, thing2 , ','))
							{
								x = thing2;
								std::string push(x + " " + y);
								coordinates.push_back(push);
							}
						}
					}

					iter++; //next string since it should be the next thing in the vector
					ss2.clear();
					ss2.str(*iter);
				}		
			}
			else
				break;
		}		
	}

	file.close(); 
	
	g_ref.setGridDimensions( xrangemax, xrangemin, yrangemax, yrangemin, wroteXR, wroteYR, 5,6);
	g_ref.setCellState(coordinates);
}
