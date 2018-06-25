#include "stdafx.h"

// Additional
#include <ctime>





#include "AuthSocket.h"

int main(int argumentCount, char* arguments[])
{
	Random::SetSeed(static_cast<unsigned long>(time(0)));

	CLog log;

	CAuthSocket socket(DEFAULT_HOST, DEFAULT_PORT);

	socket.SendLogonChallenge();

	socket.ReceiveLoop();

	return 0;
}
