// Author/editor/coder: Simo
#include "NetworkMessage.h"
#include "Endianness.h"
// Using network byte order (big endian)

CNetworkMessage::CNetworkMessage(char * pBytes, int iLength)
{
	m_pBytes = pBytes;
	m_iLength = iLength;
	m_iType = GetInt(0);
}

CNetworkMessage::CNetworkMessage(int iType, int iLength)
{
	m_iLength = iLength;
	m_pBytes = new char[m_iLength];
	Set(iType, 0);
	m_iType = iType;
}


CNetworkMessage::~CNetworkMessage(void)
{
	if(nullptr != m_pBytes)
	{
		delete m_pBytes;
	}
}
int 
CNetworkMessage::Length()
{
	return m_iLength;
}
char * 
CNetworkMessage::GetBytes()
{
	return m_pBytes;
}
int 
CNetworkMessage::GetType()
{
	return m_iType;
}
CNetworkMessage *
CNetworkMessage::GetCopy()
{
	char * pBytes = new char[m_iLength];
	memcpy(pBytes, m_pBytes, m_iLength);
	return new CNetworkMessage(pBytes, m_iLength);
}
int 
CNetworkMessage::GetInt(int iOffset)
{
	return CEndianness::BigInt(GetRawInt(iOffset)); // Endian fix!
}
int
CNetworkMessage::GetIntLittle(int iOffset)
{
    return CEndianness::LittleInt(GetRawInt(iOffset)); // Endian fix!
}
int
CNetworkMessage::GetRawInt(int iOffset)
{
    if (iOffset + 4 > m_iLength || iOffset < 0)
    {
        return 0;
    }
    int iValue = (m_pBytes[iOffset] & 0xff) << 24;
    iValue += (m_pBytes[iOffset + 1] & 0xff) << 16;
    iValue += (m_pBytes[iOffset + 2] & 0xff) << 8;
    iValue += (m_pBytes[iOffset + 3] & 0xff);
    return iValue;
}
char 
CNetworkMessage::GetByte(int iOffset)
{
	if(iOffset >= m_iLength || iOffset < 0)
	{
		return 0;
	}
	return m_pBytes[iOffset];
}
CStr * 
CNetworkMessage::GetString(int iOffset, int iLength)
{
	if(iOffset + iLength > m_iLength || iOffset < 0 || iLength <= 0)
	{
		return nullptr;
	}
	return new CStr(m_pBytes, iOffset, iLength);
}
float 
CNetworkMessage::GetFloat(int iOffset)
{
	return CEndianness::BigFloat(GetRawFloat(iOffset)); // Endian fix!
}
float
CNetworkMessage::GetFloatLittle(int iOffset)
{
    return CEndianness::LittleFloat(GetRawFloat(iOffset)); // Endian fix!
}
float
CNetworkMessage::GetRawFloat(int iOffset)
{
    if (iOffset + 4 > m_iLength || iOffset < 0)
    {
        return 0.0f;
    }
    union {
        float f;
        char c[4];
    } f1;
    f1.c[0] = m_pBytes[iOffset];
    f1.c[1] = m_pBytes[iOffset + 1];
    f1.c[2] = m_pBytes[iOffset + 2];
    f1.c[3] = m_pBytes[iOffset + 3];
    return f1.f;
}
bool 
CNetworkMessage::Set(char cValue, int iOffset)
{
	if(iOffset + 1 > m_iLength || iOffset < 0) return false;
	AsBytes(cValue, m_pBytes, iOffset);
	return true;
}
bool 
CNetworkMessage::Set(int iValue, int iOffset)
{
	if(iOffset + 4 > m_iLength || iOffset < 0) return false;
    iValue = CEndianness::BigInt(iValue); // Endian fix!
	AsBytes(iValue, m_pBytes, iOffset);
	return true;
}
bool
CNetworkMessage::SetLittle(int iValue, int iOffset)
{
    if (iOffset + 4 > m_iLength || iOffset < 0) return false;
    iValue = CEndianness::LittleInt(iValue); // Endian fix!
    AsBytes(iValue, m_pBytes, iOffset);
    return true;
}
bool 
CNetworkMessage::Set(float fValue, int iOffset)
{
	if(iOffset + 4 > m_iLength || iOffset < 0) return false;
    fValue = CEndianness::BigFloat(fValue); // Endian fix!
	AsBytes(fValue, m_pBytes, iOffset);
	return true;
}
bool
CNetworkMessage::SetLittle(float fValue, int iOffset)
{
    if (iOffset + 4 > m_iLength || iOffset < 0) return false;
    fValue = CEndianness::LittleFloat(fValue); // Endian fix!
    AsBytes(fValue, m_pBytes, iOffset);
    return true;
}
bool 
CNetworkMessage::Set(CStr * pStr, int iOffset)
{
	if(nullptr == pStr) return false;
	int iLength = pStr->Length();
	if(iOffset + 4 + iLength > m_iLength || iOffset < 0) return false;
    Set(iLength, iOffset);
	memcpy(m_pBytes + iOffset + 4, pStr->GetArray(), iLength);
	return true;
}
bool 
CNetworkMessage::Set(char * pData, int iSize, int iOffset)
{
	if(nullptr == pData) return false;
	memcpy(m_pBytes + iOffset, pData, iSize);
	return true;
}
char * 
CNetworkMessage::AsBytes(char cValue)
{
	return AsBytes(cValue, new char[1], 0);
}
char * 
CNetworkMessage::AsBytes(char cValue, char * pBytes, int iOffset)
{
	pBytes[iOffset] = cValue;
	return pBytes;
}
char * 
CNetworkMessage::AsBytes(int iValue)
{
    return AsBytes(iValue, new char[4], 0);
}
char * 
CNetworkMessage::AsBytes(int iValue, char * pBytes, int iOffset)
{
	pBytes[iOffset] = (char)((iValue >> 24) & 0xff);
	pBytes[iOffset + 1] = (char)((iValue >> 16) & 0xff);
	pBytes[iOffset + 2] = (char)((iValue >> 8) & 0xff);
	pBytes[iOffset + 3] = (char)(iValue & 0xff);
	return pBytes;
}
char * 
CNetworkMessage::AsBytes(float fValue)
{
    return AsBytes(fValue, new char[4], 0);
}
char * 
CNetworkMessage::AsBytes(float fValue, char * pBytes, int iOffset)
{
	union{
		float f;
		char c[4];
	} f1;
	f1.f = fValue;
	pBytes[iOffset] = f1.c[0];
	pBytes[iOffset + 1] = f1.c[1];
	pBytes[iOffset + 2] = f1.c[2];
	pBytes[iOffset + 3] = f1.c[3];
	return pBytes;
};