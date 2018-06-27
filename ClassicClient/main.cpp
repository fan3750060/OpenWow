#include "stdafx.h"

// Additional
#include <ctime>

#include "AuthWorldController.h"

int main(int argumentCount, char* arguments[])
{
	Random::SetSeed(static_cast<unsigned long>(time(0)));

	CLog log;

	CAuthWorldController authWorldController;
	authWorldController.StartAuth();

	// Routine
	while (true)
	{
		Sleep(16);
	}

	return 0;
}
