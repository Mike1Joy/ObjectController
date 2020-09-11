// Author/editor/coder: Simo
#pragma once

#include "Str.h"

class CNetworkMessage
{
public:
    CNetworkMessage(char * pBytes, int iLength);
    CNetworkMessage(int iType, int iLength);
	~CNetworkMessage(void);

	int Length();
	char * GetBytes();

	int GetType();
	CNetworkMessage * GetCopy();

	static const int NWMSG_UNKNOWN = 0;

	/*
	static const int NWMSG_GET_LEVEL_COUNT = 1;
	static const int NWMSG_RET_LEVEL_COUNT = 2;
	static const int NWMSG_GET_LEVEL = 3;
	static const int NWMSG_RET_LEVEL = 4;

	static const int NWMSG_GET_WALL_COUNT = 5;
	static const int NWMSG_RET_WALL_COUNT = 6;
	static const int NWMSG_GET_WALL = 7;
	static const int NWMSG_RET_WALL = 8;
	*/
	static const int NWMSG_SET_LEVEL = 1;
	static const int NWMSG_SET_WALL = 2;
	static const int NWMSG_SET_PERSON = 3;
	static const int NWMSG_SET_PERSON_ACTION = 4;
	static const int NWMSG_SET_PLAY = 5;
	static const int NWMSG_SET_STOP = 6;
	static const int NWMSG_SET_REWIND = 7;
	static const int NWMSG_SET_RESET = 8;
	static const int NWMSG_SET_SEEK = 9;
	static const int NWMSG_SET_SPEED = 10;
	static const int NWMSG_SET_MODEL_VERSION = 11;
	static const int NWMSG_SET_FRAME = 12;
	static const int NWMSG_SET_PERSON_MOVEMENT_TARGET = 13;
	
    // No endianness on these functions!
	static char * AsBytes(char cValue);
	static char * AsBytes(char cValue, char * pBytes, int iOffset);
	static char * AsBytes(int iValue);
	static char * AsBytes(int iValue, char * pBytes, int iOffset);
	static char * AsBytes(float fValue);
	static char * AsBytes(float fValue, char * pBytes, int iOffset);
	
	bool Set(char cValue, int iOffset);
	bool Set(int iValue, int iOffset);
    bool SetLittle(int iValue, int iOffset);
	bool Set(float fValue, int iOffset);
    bool SetLittle(float fValue, int iOffset);
	bool Set(CStr * pStr, int iOffset);
	bool Set(char * pData, int iSize, int iOffset);

	int GetInt(int iOffset);
    int GetIntLittle(int iOffset);
	char GetByte(int iOffset);
	CStr * GetString(int iOffset, int iLength);
	float GetFloat(int iOffset); // Default is BigEndian
    float GetFloatLittle(int iOffset);
private:
    float GetRawFloat(int iOffset);
    int GetRawInt(int iOffset);
	int m_iLength;
	char * m_pBytes;
	int m_iType; // pre-calculated!
};
