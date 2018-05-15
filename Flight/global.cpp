#include "global.h"

int gauss(unsigned int n)
{
	return n*(n + 1) / 2;
}

int fakultaet(unsigned int n)
{
	return (n == 0) ? 1 : n* (fakultaet(n - 1));
}

int convert_int_to_day(int _N)
{
	return(_N / (24 * 60));
}

int convert_int_to_hour(int _N)
{
	return((_N - (24 * 60)*convert_int_to_day(_N)) / 60);
}

int convert_int_to_minute(int _N)
{
	return(_N - (24 * 60)*convert_int_to_day(_N) - 60 * convert_int_to_hour(_N));
}

string convert_int_to_TimeStr(int minutes)
{
	string dayStr = "Tag " + to_string(convert_int_to_day(minutes) + 1);
	string hourStr = convert_int_to_hour(minutes) < 10 ? (string("0") + to_string(convert_int_to_hour(minutes))) : to_string(convert_int_to_hour(minutes));
	string minStr = convert_int_to_minute(minutes) < 10 ? (string("0") + to_string(convert_int_to_minute(minutes))) : to_string(convert_int_to_minute(minutes));

	return dayStr + " " + hourStr + ":" + minStr;
}
