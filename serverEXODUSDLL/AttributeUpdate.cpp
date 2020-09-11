// Author/editor/coder: Simo
#define NULL 0
#include "AttributeUpdate.h"

CAttributeUpdate::CAttributeUpdate(void)
{
	m_cId = 0;
	m_iLength = 0;
	m_pValue = NULL;
}
CAttributeUpdate::CAttributeUpdate(char cId, float fValue)
{
	m_cId = cId;
	m_iLength = 0;
	m_pValue = NULL;
	SetValue(fValue);

}
CAttributeUpdate::CAttributeUpdate(char cId, int iValue)
{
	m_cId = cId;
	m_iLength = 0;
	m_pValue = NULL;
	SetValue(iValue);
}
CAttributeUpdate::CAttributeUpdate(char cId, char * pBytes, int offset)
{
	m_cId = cId;
	m_iLength = 0;
	m_pValue = NULL;
	SetValue(pBytes, offset);
}

CAttributeUpdate::~CAttributeUpdate(void)
{
	if(NULL != m_pValue) delete m_pValue;
	m_pValue = NULL;
}
void 
CAttributeUpdate::SetValue(float fValue)
{
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
	if(NULL != m_pValue) delete m_pValue;
	m_pValue = c;
	m_iLength = 4;
}
void 
CAttributeUpdate::SetValue(int iValue)
{
	char * c = new char[4];
	c[0] = (char)((iValue >> 24) & 0xff);
	c[1] = (char)((iValue >> 16) & 0xff);
	c[2] = (char)((iValue >> 8) & 0xff);
	c[3] = (char)(iValue & 0xff);
	if(NULL != m_pValue) delete m_pValue;
	m_pValue = c;
	m_iLength = 4;
}
void 
CAttributeUpdate::SetValue(char * pBytes, int offset)
{
	if(NULL == pBytes) return;
	char * c = new char[4];
	c[0] = pBytes[offset++];
	c[1] = pBytes[offset++];
	c[2] = pBytes[offset++];
	c[3] = pBytes[offset++];
	m_pValue = c;
	m_iLength = 4;
}
float 
CAttributeUpdate::GetFloat()
{
	if(NULL == m_pValue || 4 != m_iLength) return 0.0f;
	union{
		float f;
		char c[4];
	} f1;
	f1.c[0] = m_pValue[0];
	f1.c[1] = m_pValue[1];
	f1.c[2] = m_pValue[2];
	f1.c[3] = m_pValue[3];
	return f1.f;
}
int 
CAttributeUpdate::GetInt()
{
	if(NULL == m_pValue || 4 != m_iLength) return 0.0f;
	int iValue = (m_pValue[0] && 0xff) << 24;
	iValue |= (m_pValue[1] && 0xff) << 16;
	iValue |= (m_pValue[2] && 0xff) << 8;
	iValue |= (m_pValue[3] && 0xff);
	return iValue;
}
int 
CAttributeUpdate::GetType(char cId)
{
	switch(cId)
	{	
	case X_LOCATION:
	case Z_LOCATION:
	case X_SPEED:
	case Y_SPEED:
	case Z_SPEED:
	case Y_ROTATION:
	case Y_LOCATION: return FLOAT;
	case ACTION_2:
	case LEVEL_ID:
	case HAZARD:
	case ACTION_1: return INTEGER;
	case UNKNOWN:
	default: return UNKNOWN;
	}
	return UNKNOWN;
}
bool 
CAttributeUpdate::IsInt()
{
	return INTEGER == GetType(m_cId);
}
bool 
CAttributeUpdate::IsFloat()
{
	return FLOAT == GetType(m_cId);
}