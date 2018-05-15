#include "scenario.h"
#include <iostream>


int main()
{
	_FLIGHTPLAN flightplan("Testplan", 28);

	_SCENARIO scenario(0, 1440 * 15, 15);

	scenario.ExecuteSim(&flightplan);
}


