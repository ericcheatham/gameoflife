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
	char * split;
	//LifeObject::LifeObject * newlife = new LifeObject;

	if( file ){	
		while( std::getline( file, line ) )
		{	
			line = line.substr(0, line.find("#"));
			std::replace(line.begin(), line.end(), '{', ';');
			//std::replace(line.begin(), line.end(), '}', ';');
		
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
			}
			if(!temp2.compare("Yrange"))
			{	
				getline(ss,temp2, ' ');
				yrangemin = atoi(temp2.c_str());
				
				getline(ss,temp2, ' ');
				yrangemax = atoi(temp2.c_str());
			}
			else if(!temp2.compare("Initial"))
			{
				
				iter++; //next string since it should be the next thing in the vector
				std::stringstream ss2(*iter);
			
				while(ss2.str().compare("}") != 0)
				{
					char * y = 0;
					split = strtok((char *)ss2.str().c_str(), " ,:="); //FUCK IT, TO C!
					if(0 != strcasecmp(split, "y"))
						break;
						
					//cout<< ss2.str() <<endl;	
					y= split = strtok (NULL, " ,:=");
					while ( (split = strtok (NULL, " ,:="))!= NULL)
					{
						//cout<< split <<" ";
						g_ref.setCellLive(atoi(split), atoi(y));
						//split = strtok (NULL, " ,:=");
					}
			
					iter++; //next string since it should be the next thing in the vector
					ss2.str(*iter);
				}
				
			}
			else
				break;
			
			
		}	
	
	}


	file.close(); 
	//cout<<xrangemin<<endl;
	g_ref.setGridDimensions( xrangemax, xrangemin, yrangemax, yrangemin,5,6);
}
