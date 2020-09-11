// Author/editor/coder: Simo
#pragma once
class CAttributeUpdate
{
public:
	static const char
		UNKNOWN = 0,
		X_LOCATION = 1,
		Y_LOCATION = 2,
		Z_LOCATION = 3,
		X_SPEED = 4,
		Y_SPEED = 5,
		Z_SPEED = 6,
		Y_ROTATION = 7,
		ACTION_1 = 8,
		ACTION_2 = 9,
		LEVEL_ID = 10,
		HAZARD = 11;

	static const int
		INTEGER = 1,
		FLOAT = 2;

	CAttributeUpdate(void);
	CAttributeUpdate(char cId, float fValue);
	CAttributeUpdate(char cId, int iValue);
	CAttributeUpdate(char cId, char * pBytes, int offset);
	~CAttributeUpdate(void);
	
	char m_cId; // Key value pair again :)
	int m_iLength;
	char * m_pValue;

	void SetValue(float fValue);
	void SetValue(int iValue);
	void SetValue(char * pBytes, int offset);
	float GetFloat();
	int GetInt();
	static int GetType(char cId);
	bool IsInt();
	bool IsFloat();
};

