// needed to use own functions defined in global.cpp
#include "scenario.h"
#include <iostream>


_SCENARIO::_SCENARIO(int _SimStartTime, int _SimEndTime, int _IntMedArrTime)
{
	// this is a constructor. It is automatically executed whenever a _SCENARIO variable is
	// declared and the three input parameters _SimStartTime, _SimEndTime, _intMedArrTime are
	// specified

	// initializes the simulation starting time variable SimStartTime
	this->SimStartTime = _SimStartTime;

	// initializes the simulation terminating time variable EndStartTime
	this->SimEndTime = _SimEndTime;

	// initializes the intermediate arrival time used in the simulation
	this->IntMedArrTime = _IntMedArrTime;

	// set the current simulation time to the simulation starting time
	this->CurrentSimTime = this->SimStartTime;

	// initializing the summary values
	this->requests = 0;
	this->immediate_acceptance = 0;
	this->bargained_accepance = 0;
	this->bargainned_decline = 0;
	this->revenues = 0.0;


	// ToDo: Initialisierung eventuell neuer Datenelemente
}

_SCENARIO::~_SCENARIO()
{
	// bisher nichts zu tun
}




// The following procedure returns the time offset until the next booking request
// arrives. Here, the assumed probability distribution of the intermediate arrival
// times between two consecutive requests is applied
int _SCENARIO::DrawTimeUntilNextRequest(int _MeanValue)
{
	// step 1: define probabilities for events k=0, k=1, ...
	int Considered_Events;
	Considered_Events = 3 * _MeanValue;

	int k;
	double *prob, sum = 0.0;;
	prob = new double[Considered_Events];
	prob[0] = pow((double)(_MeanValue), (double)(0)) / (double)(fakultaet)(0)*exp((double)(-_MeanValue));
	for (k = 1; k < Considered_Events; k++)
	{
		prob[k] = (double)(_MeanValue) / ((double)(k))* prob[k - 1];
	}

	// step 2: define returned intermediate arrival time by a roulette wheel selection
	sum = 0;
	int k_start;
	k_start = rand() % (3 * _MeanValue);

	sum = sum + prob[k_start];
	while (sum < 0.5)
	{
		sum = sum + prob[k_start];
		k_start++;
		k_start = k_start % (3 * _MeanValue);
	}

	// release allocated memory
	delete[] prob;

	// step 3: return new intermediate arrival time value

	return(k_start - 1);

}


// The following procedure executes the simulation by successively adding
// time to the variable _SCENARIO.CurrentSimTime. The increase of this variable
// is interrupted as soon as _SCENARIO.CurrentSimTime exceeds the specified
// threshold value this->SimEndTime
void _SCENARIO::ExecuteSim(_FLIGHTPLAN *FLIGHTPLAN)
{
	int OldSimTime;
	printf("Simulation time %d: Event = the simulation starts\n", this->CurrentSimTime);

	cout << "Hallo";
	// ToDo: (eventuell) dynamischen belegten Speicher freigeben + neu reservieren 



	while (this->CurrentSimTime < this->SimEndTime)
	{
		// determine time progress
		OldSimTime = this->CurrentSimTime;
		this->CurrentSimTime += this->DrawTimeUntilNextRequest(this->IntMedArrTime);
		cout << "Simulation Time: " << convert_int_to_TimeStr(this->CurrentSimTime);
		
		// get the next booking request
		_BOOKINGREQUEST BR = _BOOKINGREQUEST(FLIGHTPLAN, this->CurrentSimTime);
		cout << " " << BR.dept_airp << "->" << BR.dest_airp;
		
		// increase request counter
		this->requests++;


		// the next line represents (A) in slides of lecture 5: analysis of booking request
		if ((BR.dept_airp != "DRS") && (BR.dest_airp != "DRS"))
		{
			// here we start with part (B) for ONE-STOP-CONNECTIONS

			// identify available connections
			int **CONNECTION;
			CONNECTION = new int*[FLIGHTPLAN->flights];
			int i, j;
			for (i = 0; i < FLIGHTPLAN->flights; i++)
			{
				CONNECTION[i] = new int[FLIGHTPLAN->flights];
				for (j = 0; j < FLIGHTPLAN->flights; j++)
				{
					CONNECTION[i][j] = -1;
					// step 1: identify all possible flights connections (offering)
					if ((FLIGHTPLAN->FLIGHTLEG[i].ORIG_APT == BR.dept_airp) && (FLIGHTPLAN->FLIGHTLEG[j].DEST_APT == BR.dest_airp))
					{
						// NOW CHECK DEPARTURE TIME
						if (BR.earliest_dept_time <= FLIGHTPLAN->FLIGHTLEG[i].dept_minute)
						{
							// departure time fits
							// now check transfer possibility in DRS
							if (FLIGHTPLAN->FLIGHTLEG[i].arr_minute + 20 <= FLIGHTPLAN->FLIGHTLEG[j].dept_minute)
							{
								// transfer in DRS from i to j is possible
								// now check if a seat is available is both flight legs
								if ((FLIGHTPLAN->FLIGHTLEG[i].seats > 0) && (FLIGHTPLAN->FLIGHTLEG[j].seats > 0))
								{
									// seats are available
									// determine ticket fare including transfer charge
									CONNECTION[i][j] = 1.05 * (FLIGHTPLAN->FLIGHTLEG[i].fare + FLIGHTPLAN->FLIGHTLEG[j].fare);
								}

							}
						}
					}
				}
			}
			// here part (B) with ONE-STOP-CONNECTIONS ends

			// now we start part (C) of ONE-STOP-CONNECTIONS 
			int best_fare_i = -1, best_fare_j = -1;
			double best_fare_dist = 1000000;
			double draw_value;
			//printf("-----ONE-STOP-CONNECTING-FLIGHTS (find first available flight) ----\n");
			for (i = 0; i < FLIGHTPLAN->flights; i++)
				for (j = 0; j < FLIGHTPLAN->flights; j++)
					if (CONNECTION[i][j] > 0)
					{

						if (fabs(double(CONNECTION[i][j]) - (double)(BR.willingness_to_pay)) < best_fare_dist)
						{
							best_fare_i = i;
							best_fare_j = j;
							best_fare_dist = fabs(double(CONNECTION[i][j]) - (double)(BR.willingness_to_pay));
						}
					}

			if ((best_fare_i >= 0) && (best_fare_j >= 0))
			{
				//cout << "----- ONE-STOP-CONNECTING-FLIGHTS: -------------" << endl;
				for (i = 0; i < FLIGHTPLAN->flights; i++)
				{
					for (j = 0; j < FLIGHTPLAN->flights; j++)
					{
						if (CONNECTION[i][j] > 0)
						{
							if ((best_fare_i == i) && (best_fare_j == j))
							{
								// now start bargaining about sale of ticket
								if (CONNECTION[i][j] < BR.willingness_to_pay)
								{
									cout << " TICKET SOLD IMMEDIATELY" << endl;
									this->immediate_acceptance += 2;
									this->revenues += CONNECTION[i][j];
									FLIGHTPLAN->FLIGHTLEG[i].seats--;
									FLIGHTPLAN->FLIGHTLEG[j].seats--;
								}
								else
								{
									draw_value = (double)(rand() % 10001) / (double)(10000);


									if (draw_value < (0.5 * this->CurrentSimTime) / this->SimEndTime)
									{
										cout << " ticket finally sold after bargaining\n" << endl;
										this->bargained_accepance += 2;
										this->revenues += CONNECTION[i][j];
										FLIGHTPLAN->FLIGHTLEG[i].seats--;
										FLIGHTPLAN->FLIGHTLEG[j].seats--;
									}
									else
									{
										cout << "Customer does not buy the ticket :-(\n";
									}
								}
							}
						}
					}
				}
			}
			else
			{
				//printf("----- ONE-STOP-CONNECTING-FLIGHTS: -------------\n");
				cout << " no flights available\n";
			}

			for (i = 0; i < FLIGHTPLAN->flights; i++)
				delete[] CONNECTION[i];
			delete[] CONNECTION;

			// here part (C) of ONE-STOP-CONNECTIONS ends
		}
		else
		{
			// we have to deal with direct flight connections (single leg)

			// here we start part (B) of NONSTOP-CONNECTIONS
			int *CONNECTION;
			CONNECTION = new int[FLIGHTPLAN->flights];

			int i;

			for (i = 0; i < FLIGHTPLAN->flights; i++)
			{
				CONNECTION[i] = -1;
				// step 1: identify all possible flights connections (offering)
				if ((FLIGHTPLAN->FLIGHTLEG[i].ORIG_APT == BR.dept_airp) && (FLIGHTPLAN->FLIGHTLEG[i].DEST_APT == BR.dest_airp))
				{
					// NOW CHECK DEPARTURE TIME
					if (BR.earliest_dept_time <= FLIGHTPLAN->FLIGHTLEG[i].dept_minute)
					{
						// departure time fits
						// now check if a seat is available is both flight legs
						if (FLIGHTPLAN->FLIGHTLEG[i].seats > 0)
						{
							// seats are available
							CONNECTION[i] = FLIGHTPLAN->FLIGHTLEG[i].fare;
						}
					}
				}
			}

			// HERE WE FINISH WITH PART (B) FOR NONSTOP-CONNECTIONS

			// HERE WE START PART(C) FOR NONSTOP-CONNECTIONS
			// now find the connection that is closest to the willingness-to-pay
			int best_fare_i = -1;
			double best_fare_dist = 1000000;
			double draw_value;
			for (i = 0; i < FLIGHTPLAN->flights; i++)
				if (CONNECTION[i] > 0)
				{
					if (fabs(double(CONNECTION[i]) - (double)(BR.willingness_to_pay)) < best_fare_dist)
					{
						best_fare_i = i;
						best_fare_dist = fabs(double(CONNECTION[i]) - (double)(BR.willingness_to_pay));
					}

				}

			if ((best_fare_i >= 0))
			{
				//printf("----- NON-STOP-CONNECTING-FLIGHTS: -------------\n");
				for (i = 0; i < FLIGHTPLAN->flights; i++)
				{
					if (CONNECTION[i] > 0)
					{
						if ((best_fare_i == i))
						{

							// now start bargaining about sale of ticket
							if (CONNECTION[i] < BR.willingness_to_pay)
							{
								cout << " TICKET SOLD IMMEDIATELY" << endl;
								this->immediate_acceptance++;
								this->revenues += CONNECTION[i];
								FLIGHTPLAN->FLIGHTLEG[i].seats--;
							}
							else
							{
								//cout << "further bargaining necessary" << endl;
								//cout << "acceptance probability (" << 0.5 * this->CurrentSimTime << "/" << this->SimEndTime << "): " << (0.5 * this->CurrentSimTime) / this->SimEndTime << endl;

								draw_value = (double)(rand() % 10001) / (double)(10000);

								//cout << "drawn value: " << draw_value << endl;

								if (draw_value < (0.5 * this->CurrentSimTime) / this->SimEndTime)
								{
									cout << " ticket finally sold after bargaining\n";
									this->bargained_accepance++;
									this->revenues += CONNECTION[i];
									FLIGHTPLAN->FLIGHTLEG[i].seats--;
								}
								else
								{
									cout << " Customer does not buy the ticket :-(\n";
									this->bargainned_decline++;
								}
							}
						}
					}
				}
			}

			else
			{
				//printf("----- NON-STOP-CONNECTING-FLIGHTS: -------------\n");
				cout << " no flights available\n";
			}

			delete[] CONNECTION;

			// here we finish part (C)

		}

		//cout << "============= PART (C) ENDS HERE =============" << endl;


		// here we start with part (D)
		// to conclude the order processing check if fares have to be increased
		int i;
		for (i = 0; i < FLIGHTPLAN->flights; i++)
		{
			if (FLIGHTPLAN->FLIGHTLEG[i].seats <= 5)
				if (FLIGHTPLAN->FLIGHTLEG[i].increased < 1)
				{
					FLIGHTPLAN->FLIGHTLEG[i].fare = 1.20 * FLIGHTPLAN->FLIGHTLEG[i].fare;
					FLIGHTPLAN->FLIGHTLEG[i].increased = 1;
				}
		}
		printf("---------------------------------------------\n");
		

		// ToDo: wenn alles erledigt ist BR abspeichern (in Array)
	}



	printf("simulation time %d: the simulation terminates\n", this->SimEndTime);
	printf("SUMMARY\n");
	printf("-------------------------------\n");
	printf("processed booking requests:			%d\n", this->requests);
	printf("sum of revenues:				%8.2f\n", this->revenues);
	printf("immediate sale:					%d\n", this->immediate_acceptance);
	printf("sale after bargain:				%d\n", this->bargained_accepance);
	printf("rejection after bargain:			%d\n", this->bargainned_decline);
	printf("requests without availabilities:		%d\n", this->requests - this->immediate_acceptance - this->bargained_accepance - this->bargainned_decline);
}

void _SCENARIO::clear()
{
	// ToDo: dynamisch belegten Speicher wieder frei geben
}

void _SCENARIO::print(int status)
{
	// ToDo: Ausgabe aller generierten Buchungsanfragen gefiltert nach dem Kriterium "status"
}
