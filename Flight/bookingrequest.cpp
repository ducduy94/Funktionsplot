#include <iostream>
#include "bookingrequest.h"


_BOOKINGREQUEST::_BOOKINGREQUEST(_FLIGHTPLAN *_FP, int _Release_Time)
{
	// The following principle is used to determine the requested
	// origin to destination connection:
	// step 1: select two flights at random
	// step 2: select one airport from the first flight
	// step 3: select one airport from the second flight that is different to the first selected flight

	// draw the flights
	int F_A, F_B;

	F_A = rand() % _FP->flights;
	F_B = rand() % _FP->flights;

	// now select the first airport
	if (rand() % 2 == 0)
	{
		this->dept_airp = _FP->FLIGHTLEG[F_A].ORIG_APT;
	}
	else
	{
		this->dept_airp = _FP->FLIGHTLEG[F_A].DEST_APT;
	}

	// now select the second airport
	if (rand() % 2 == 0)
	{
		if (this->dept_airp == _FP->FLIGHTLEG[F_B].ORIG_APT)
		{
			this->dest_airp = _FP->FLIGHTLEG[F_B].DEST_APT;
		}
		else
		{
			this->dest_airp = _FP->FLIGHTLEG[F_B].ORIG_APT;
		}
	}
	else
	{
		if (this->dept_airp == _FP->FLIGHTLEG[F_B].DEST_APT)
		{
			this->dest_airp = _FP->FLIGHTLEG[F_B].ORIG_APT;
		}
		else
		{
			this->dest_airp = _FP->FLIGHTLEG[F_B].DEST_APT;
		}
	}

	// determine the desired departure time
	this->earliest_dept_time = 300 + rand() % 601;

	// determine the willingness to pay
	if ((this->dept_airp == "DRS") || (this->dest_airp == "DRS"))
	{
		// single leg flight demanded
		this->willingness_to_pay = (400 + rand() % 301);
	}
	else
	{
		this->willingness_to_pay = 1.25 * (500 + rand() % 501);
	}

	// save release time
	this->release_time = _Release_Time;


	// ToDo: Initialisierung eventuell neuer Datenelemente

}

void _BOOKINGREQUEST::print()
{
	// ToDo: geschalten Sie eine Ausgabe
}

_BOOKINGREQUEST::_BOOKINGREQUEST()
	: dest_airp(""), dept_airp(""), release_time(-1), willingness_to_pay(0), earliest_dept_time(0)
{
	// ToDo: Initialisierung eventuell neuer Datenelemente
}