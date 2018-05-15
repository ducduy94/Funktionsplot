#ifndef FLIGHTPLAN_H
#define FLIGHTPLAN_H

#include "flightleg.h"



struct _FLIGHTPLAN 
{
	// this defines an object template for a flight plan
	// attributes
	int flights; // gives the number of flights to be stored
	char flightplanname[256];
	_FLIGHTLEG * FLIGHTLEG;
	
	// constructor
	_FLIGHTPLAN(void);
	_FLIGHTPLAN(char flightplanname[256], int _number_of_flightlegs);
	
	// destructor
	~_FLIGHTPLAN();
	
	// other methods
	void add_flight(_FLIGHTLEG *FLIGHT); // using given number of flights
	void print();

	int apstrToIndex(const string& name);
	const string& indexToApstr(int index);
	void printNetworkData(const int matrix[7][7], string title = "Unknown Network Data");
};

#endif // !FLIGHTPLAN_H
