#ifndef BOOKINGREQUEST_H
#define BOOKINGREQUEST_H

#include "flightplan.h"



struct _BOOKINGREQUEST {

	string dept_airp;			// customer specified departure airport
	string dest_airp;			// customer specified destination airport

	int release_time;			// determines the times when put into the scenario
	int earliest_dept_time;		// earliest departure time specified by customer
	int willingness_to_pay;		// maximal willingness to pay of customer


	// ToDo: eventuelle neue Datenelemente


	// constructors
	_BOOKINGREQUEST();
	_BOOKINGREQUEST(_FLIGHTPLAN *FP, int _Release_time);

	void print();
};

#endif