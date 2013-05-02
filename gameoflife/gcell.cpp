#include "gcell.h"


Gcell::Gcell(){}
void Gcell::setCWState(bool state){ conway  = state;}
void Gcell::setBBState(int state) { bbstate = state;}
void Gcell::setWWState(int state) { wwstate = state;}
void Gcell::setLAState(int state) { lastate = state;}

bool Gcell::getCWState() { return conway;}
int Gcell::getBBState() { return bbstate;}
int Gcell::getWWState() { return wwstate;}
int Gcell::getLAState() { return lastate;}

Gcell Gcell::operator=(bool value)
{
	Gcell temp;
	temp.setCWState(value);
	return temp;
}

Gcell Gcell::operator=(Gcell  value)
{
	Gcell temp;
	temp.setCWState(value.getCWState());
	temp.setCWState(value.getBBState());
	temp.setCWState(value.getWWState());
	temp.setCWState(value.getLAState());
	
	return temp;
}
