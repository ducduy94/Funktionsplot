#include "flightleg.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

_FLIGHTLEG::_FLIGHTLEG()
	: _FLIGHTLEG(-17, string(""), -1, string(""), -1, -1, -1, -1)
{
}

_FLIGHTLEG::_FLIGHTLEG(int _flight_num,const string & _ORIG_APT, int _dept_minute,const string& _DEST_APT, int _arr_minute, int _fare, int _seats, int _ac_id)
	: flight_num(_flight_num), ORIG_APT(_ORIG_APT), dept_minute(_dept_minute), DEST_APT(_DEST_APT), arr_minute(_arr_minute), fare(_fare), seats(_seats), ac_id(_ac_id), increased(false)
{

}

int _FLIGHTLEG::getFare()
{
	return this->fare;
}

int _FLIGHTLEG::getDeptTime()
{
	return this->dept_minute;
}

int _FLIGHTLEG::getArrTime()
{
	return this->arr_minute;
}

int _FLIGHTLEG::getSeats()
{
	return this->seats;
}

int _FLIGHTLEG::getFlightNum()
{
	return this->flight_num;
}

void _FLIGHTLEG::print() const
{
	printf("\n+---------Flightleg %d---------+", this->flight_num);
	printf("\n|Origin:\t%s\t\t|", this->ORIG_APT.c_str());
	printf("\n|Departure:\t%s\t|", convert_int_to_TimeStr(dept_minute).c_str() );
	printf("\n|Destination:\t%s\t\t|", this->DEST_APT.c_str());
	printf("\n|Arrival:\t%s\t|", convert_int_to_TimeStr(arr_minute).c_str());
	printf("\n|AC-ID:\t\t%d\t\t|", this->ac_id);
	printf("\n|Seats:\t\t%d\t\t|", this->seats);
	printf("\n|Fare:\t\t%d\t\t|", this->fare);
	printf("\n|Increased:\t%s\t\t|", increased ? "true" : "false");
	printf("\n+-------------------------------+\n");
}



