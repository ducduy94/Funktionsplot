#ifndef FLIGHTLEG_H
#define FLIGHTLEG_H


#include "global.h"

struct _FLIGHTLEG {
	//this struct-object represents a single flight leg from
	// a departure to a destination airport APT

	// data attributes
	int flight_num;
	string ORIG_APT;
	string DEST_APT;
	int dept_minute;
	int arr_minute;
	int fare;
	int seats;
	int ac_id;
	bool increased;


	_FLIGHTLEG();
	_FLIGHTLEG(int _flight_num,const string& _ORIG_APT, int _dept_minute,const string& _DEST_APT, int _arr_minute,
		int _fare, int _seats, int _ac_id);

	int getFare();
	int getDeptTime();
	int getArrTime();
	int getSeats();
	int getFlightNum();

	void print() const;
};


#endif // !FLIGHTLEG_H
