#include "stdafx.h"

// Additional
#include <ctime>

CAuthWorldController* m_authWorldController = nullptr;

void S_CharEnum(ByteBuffer& _buff)
{
	uint8 charCnt;
	_buff.readBytes(&charCnt);

	CharacterTemplate* chars = new CharacterTemplate[charCnt];
	for (uint8 i = 0; i < charCnt; i++)
	{
		chars[i].TemplateFill(_buff);
		chars[i].TemplatePrint();
	}

	ByteBuffer bb;
	bb.Write((uint64)chars[1].GUID);
	m_authWorldController->getWorldSocket()->SendData(CMSG_PLAYER_LOGIN, bb);

	//Orc
	/*ByteBuffer bb;
	bb.Write("Txtyryer", 9);
	bb.Write((uint8)2);
	bb.Write((uint8)1);
	bb.Write((uint8)0);
	bb.Write((uint8)15);
	bb.Write((uint8)6);
	bb.Write((uint8)3);
	bb.Write((uint8)6);
	bb.Write((uint8)5);
	bb.Write((uint8)0);
	SendData(CMSG_CHAR_CREATE, bb);*/

}

int main(int argumentCount, char* arguments[])
{
	Random::SetSeed(static_cast<unsigned long>(time(0)));

	CBaseManager baseManager;
	_BaseManager = &baseManager;

	CLog log;

	CAuthWorldController authWorldController;
	authWorldController.StartAuth();

	m_authWorldController = &authWorldController;
	m_authWorldController->getWorldSocket()->AddHandler(SMSG_CHAR_ENUM, FUNCTION_WA_Builder(S_CharEnum, ByteBuffer&));


	// Routine
	while (true)
	{
		Sleep(16);
	}

	return 0;
}
