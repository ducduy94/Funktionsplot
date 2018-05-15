#ifndef SCENARIO_H
#define SCENARIO_H

#include "bookingrequest.h"

// this file contains the definition of a class object that describes
// and configures the simulation environment

// All methods that belong to this object are specified with there source
// code in the file scenario.cpp

class _SCENARIO {
public:

	// the following attributes are used to describe the initial setup
	int SimStartTime;			// the starting time of the simulation experiment (in minutes)
	int SimEndTime;				// defines the time point when the simulation terminates (in minutes)

	// the following attributes are used to describe the dynamic part
	int IntMedArrTime;			// the intermediate arrival time between to consecutive booking requests (in minutes)						
	int CurrentSimTime;			// used to save the current time during a running simulation
	int requests;				// number of processed requests
	int immediate_acceptance;	// number of requests accepted due to large wtp
	int bargained_accepance;	// number of tickets sold after bargaining
	int bargainned_decline;		// number of tickets not sold after bargaining
	double revenues;			// sum of revenues collectd by sold tickets

								
	// potenzielle neue Datenelemente:



	
	// in the following the methods are defined that manipulate the data stored in the _SCENARIO object
	// this is the constructor that initiates a realization of the _SCENARIO object
	_SCENARIO(int _SimStartTime, int _SimEndTime, int _IntMedArrTime);
	~_SCENARIO();

	// this function initiates the execution of the simulation minicing time progress
	void _SCENARIO::ExecuteSim(_FLIGHTPLAN *FLIGHTPLAN);

	// vorbereite Methoden für die Klausuren
	void clear();
	void print(int status);


	// this function returns the time until the next booking request arrival. It randomly draws this  time offset value
	static int DrawTimeUntilNextRequest(int _MeanValue);
};

#endif // SCENARIO_H