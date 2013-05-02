#ifndef GCELL_H
#define GCELL_H


class Gcell{

	bool conway;	
	int bbstate;
	int wwstate;
	int lastate;

	public: 
		Gcell();
		void setCWState(bool state);
		void setBBState(int state);
		void setWWState(int state);
		void setLAState(int state);
		bool getCWState();
		int getBBState();
		int getWWState();
		int getLAState();

		Gcell operator= (bool);
		Gcell operator= (Gcell);
};

#endif 
