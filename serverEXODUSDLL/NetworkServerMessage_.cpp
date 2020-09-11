// Author/editor/coder: Simo
#include "NetworkServerMessage.h"
#include "Endianness.h"
// Using network byte order (big endian)

CNetworkServerMessage::CNetworkServerMessage(char * pBytes, int iLength)
{
	m_pBytes = pBytes;
	m_iLength = iLength;
	m_iType = GetInt(0);
}

CNetworkServerMessage::CNetworkServerMessage(int iType, int iLength)
{
	m_iLength = iLength;
	m_pBytes = new char[m_iLength];
	Set(iType, 0);
}


CNetworkServerMessage::~CNetworkServerMessage(void)
{
	if(NULL != m_pBytes)
	{
		delete m_pBytes;
	}
}
int 
CNetworkServerMessage::Length()
{
	return m_iLength;
}
char * 
CNetworkServerMessage::GetBytes()
{
	return m_pBytes;
}
int 
CNetworkServerMessage::GetType()
{
	return m_iType;
}
CNetworkServerMessage *
CNetworkServerMessage::GetCopy()
{
	char * pBytes = new char[m_iLength];
	memcpy(pBytes, m_pBytes, m_iLength);
	return new CNetworkServerMessage(pBytes, m_iLength);
}
int 
CNetworkServerMessage::GetInt(int iOffset)
{
	if(iOffset + 4 > m_iLength || iOffset < 0)
	{
		return 0;
	}
	int iValue = (m_pBytes[iOffset] & 0xff) << 24;
	iValue += (m_pBytes[iOffset + 1] & 0xff) << 16;
	iValue += (m_pBytes[iOffset + 2] & 0xff) << 8;
	iValue += (m_pBytes[iOffset + 3] & 0xff);
	iValue = CEndianness::BigInt(iValue); // Endian fix!
	return iValue;
}
char 
CNetworkServerMessage::GetByte(int iOffset)
{
	if(iOffset >= m_iLength || iOffset < 0)
	{
		return 0;
	}
	return m_pBytes[iOffset];
}
CStr * 
CNetworkServerMessage::GetString(int iOffset, int iLength)
{
	if(iOffset + iLength > m_iLength || iOffset < 0 || iLength <= 0)
	{
		return NULL;
	}
	return new CStr(m_pBytes, iOffset, iLength);
}
float 
CNetworkServerMessage::GetFloat(int iOffset)
{
	if(iOffset + 4 > m_iLength || iOffset < 0)
	{
		return 0.0f;
	}
	union{
		float f;
		char c[4];
	} f1;
	f1.c[0] = m_pBytes[iOffset];
	f1.c[1] = m_pBytes[iOffset + 1];
	f1.c[2] = m_pBytes[iOffset + 2];
	f1.c[3] = m_pBytes[iOffset + 3];
	float fValue = f1.f;
	fValue = CEndianness::BigFloat(fValue); // Endian fix!
	return fValue;
}
bool 
CNetworkServerMessage::Set(char cValue, int iOffset)
{
	if(iOffset + 1 > m_iLength || iOffset < 0) return false;
	AsBytes(cValue, m_pBytes, iOffset);
	return true;
}
bool 
CNetworkServerMessage::Set(int iValue, int iOffset)
{
	if(iOffset + 4 > m_iLength || iOffset < 0) return false;
	AsBytes(iValue, m_pBytes, iOffset);
	return true;
}
bool 
CNetworkServerMessage::Set(float fValue, int iOffset)
{
	if(iOffset + 4 > m_iLength || iOffset < 0) return false;
	AsBytes(fValue, m_pBytes, iOffset);
	return true;
}
bool 
CNetworkServerMessage::Set(CStr * pStr, int iOffset)
{
	if(NULL == pStr) return false;
	int iLength = pStr->Length();
	if(iOffset + 4 + iLength > m_iLength || iOffset < 0) return false;
	AsBytes(iLength, m_pBytes, iOffset);
	memcpy(m_pBytes + iOffset + 4, pStr->GetArray(), iLength);
	return true;
}
bool 
CNetworkServerMessage::Set(char * pData, int iSize, int iOffset)
{
	if(NULL == pData) return false;
	memcpy(m_pBytes + iOffset, pData, iSize);
	return true;
}
char * 
CNetworkServerMessage::AsBytes(char cValue)
{
	char * c = new char[1];
	c[0] = cValue;
	return c;
}
char * 
CNetworkServerMessage::AsBytes(char cValue, char * pBytes, int iOffset)
{
	pBytes[iOffset] = cValue;
	return pBytes;
}
char * 
CNetworkServerMessage::AsBytes(int iValue)
{
	iValue = CEndianness::BigInt(iValue); // Endian fix!
	char * c = new char[4];
	c[0] = (char)((iValue >> 24) & 0xff);
	c[1] = (char)((iValue >> 16) & 0xff);
	c[2] = (char)((iValue >> 8) & 0xff);
	c[3] = (char)(iValue & 0xff);
	return c;
}
char * 
CNetworkServerMessage::AsBytes(int iValue, char * pBytes, int iOffset)
{
	iValue = CEndianness::BigInt(iValue); // Endian fix!
	pBytes[iOffset] = (char)((iValue >> 24) & 0xff);
	pBytes[iOffset + 1] = (char)((iValue >> 16) & 0xff);
	pBytes[iOffset + 2] = (char)((iValue >> 8) & 0xff);
	pBytes[iOffset + 3] = (char)(iValue & 0xff);
	return pBytes;
}
char * 
CNetworkServerMessage::AsBytes(float fValue)
{
	fValue = CEndianness::BigFloat(fValue); // Endian fix!
	union{
		float f;
		char c[4];
	} f1;
	f1.f = fValue;
	char * c = new char[4];
	c[0] = f1.c[0];
	c[1] = f1.c[1];
	c[2] = f1.c[2];
	c[3] = f1.c[3];
	return c;
}
char * 
CNetworkServerMessage::AsBytes(float fValue, char * pBytes, int iOffset)
{
	fValue = CEndianness::BigFloat(fValue); // Endian fix!
	union{
		float f;
		char c[4];
	} f1;
	f1.f = fValue;
	char * c = new char[4];
	pBytes[iOffset] = f1.c[0];
	pBytes[iOffset + 1] = f1.c[1];
	pBytes[iOffset + 2] = f1.c[2];
	pBytes[iOffset + 3] = f1.c[3];
	return c;
}