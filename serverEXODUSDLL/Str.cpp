// Author/editor/coder: Simo
#include "Str.h"


CStr::CStr(void)
{
	m_iCount = 0;
	m_pszValue = NULL;
	m_bIsTemp = false;
}

CStr::CStr(CStr * pOriginal)
{
	m_iCount = pOriginal->m_iCount;
	m_pszValue = new char[m_iCount + 1];
	memcpy(m_pszValue, pOriginal->m_pszValue, m_iCount);
	m_pszValue[m_iCount] = '\0';
	m_bIsTemp = false;
	if(pOriginal->m_bIsTemp){
		delete pOriginal;
	}
}

CStr::CStr(char * pszValue)
{
	m_iCount = strlen(pszValue);
	m_pszValue = new char[m_iCount + 1];
	memcpy(m_pszValue, pszValue, m_iCount);
	m_pszValue[m_iCount] = '\0';
	m_bIsTemp = false;
}

CStr::CStr(const char * pszValue)
{
	m_iCount = strlen(pszValue);
	m_pszValue = new char[m_iCount + 1];
	memcpy(m_pszValue, pszValue, m_iCount);
	m_pszValue[m_iCount] = '\0';
	m_bIsTemp = false;
}

CStr::CStr(char * pszValue, int iOffset, int iCount)
{
	m_iCount = iCount;
	m_pszValue = new char[m_iCount + 1];
	memcpy(m_pszValue, pszValue + iOffset, iCount);
	m_pszValue[m_iCount] = '\0';
	m_bIsTemp = false;
}

CStr::CStr(const char * pszValue, int iOffset, int iCount)
{
	m_iCount = iCount;
	m_pszValue = new char[m_iCount + 1];
	memcpy(m_pszValue, pszValue + iOffset, iCount);
	m_pszValue[m_iCount] = '\0';
	m_bIsTemp = false;
}

CStr::CStr(int iValue)
{
	m_pszValue = new char[MAX_INT_LENGTH];
	sprintf(m_pszValue, "%d\0", iValue);
	m_iCount = strlen(m_pszValue);
	m_bIsTemp = false;
}

CStr::CStr(float fValue)
{
	m_pszValue = new char[MAX_FLOAT_LENGTH];
	sprintf(m_pszValue, "%f\0", fValue);
	m_iCount = strlen(m_pszValue);
	m_bIsTemp = false;
}
CStr::CStr(float fValue, int iDecimalPoints)
{
	CStr format = (CStr("%.") + (CStr(iDecimalPoints) + &CStr("f\0")));
	m_pszValue = new char[MAX_FLOAT_LENGTH];
	sprintf(m_pszValue, format.GetArray(), fValue);
	m_iCount = strlen(m_pszValue);
	m_bIsTemp = false;
}

CStr::CStr(char cValue)
{
	m_pszValue = new char[2];
	m_pszValue[0] = cValue;
	m_pszValue[1] = '\0';
	m_iCount = 1;
	m_bIsTemp = false;
}


CStr::~CStr(void)
{
	if(NULL != m_pszValue)
	{
		delete m_pszValue;
	}
}

int 
CStr::Length()
{
	return m_iCount;
}
bool 
CStr::IsEmpty()
{
	return m_iCount == 0;
}
//Returns temp str
CStr * 
CStr::operator+(CStr * rhs)
{
	int iTotalLength = rhs->m_iCount + m_iCount;
	CStr * pRet = new CStr();
	pRet->m_pszValue = new char[iTotalLength + 1];
	pRet->m_iCount = iTotalLength;
	memcpy(pRet->m_pszValue, m_pszValue, m_iCount);
	memcpy(pRet->m_pszValue + m_iCount, rhs->m_pszValue, rhs->m_iCount);
	pRet->m_pszValue[iTotalLength] = '\0';
	pRet->m_bIsTemp = true;
	if(rhs->m_bIsTemp){
		delete rhs;
	}
	return pRet;
}
//Returns temp str
CStr * 
CStr::operator+(const char * rhs)
{
	int charLen = strlen(rhs);
	int iTotalLength = charLen + m_iCount;
	CStr * pRet = new CStr();
	pRet->m_pszValue = new char[iTotalLength + 1];
	pRet->m_iCount = iTotalLength;
	memcpy(pRet->m_pszValue, m_pszValue, m_iCount);
	memcpy(pRet->m_pszValue + m_iCount, rhs, charLen);
	pRet->m_pszValue[iTotalLength] = '\0';
	pRet->m_bIsTemp = true;
	return pRet;
}
//Returns temp str
CStr * 
CStr::operator+(int rhs)
{
	CStr * pRet = new CStr(rhs);
	pRet->m_bIsTemp = true;
	return *this + pRet;
}
//Returns temp str
CStr * 
CStr::operator+(float rhs)
{
	CStr * pRet = new CStr(rhs);
	pRet->m_bIsTemp = true;
	return *this + pRet;
}
//Returns temp str
CStr * 
CStr::operator+(char rhs)
{
	CStr * pRet = new CStr(rhs);
	pRet->m_bIsTemp = true;
	return *this + pRet;
}
CStr * 
CStr::operator=(CStr * rhs) // Make this a copy!
{
	if(*this == rhs)
	{
		// This is already a copy!
		return this;
	}
	if(NULL != m_pszValue)
	{
		delete m_pszValue;
	}
	m_iCount = rhs->m_iCount;
	m_pszValue = new char[m_iCount + 1];
	memcpy(m_pszValue, rhs->m_pszValue, m_iCount);
	m_pszValue[m_iCount] = '\0';
	if(rhs->m_bIsTemp){
		delete rhs;
	}
	return this;
}
CStr * 
CStr::operator=(const char * rhs)
{
	//Does this cause memory leaks?
	if(*this == &CStr(rhs))
	{
		// This is already a copy!
		return this;
	}
	if(NULL != m_pszValue)
	{
		delete m_pszValue;
	}
	m_iCount = strlen(rhs);
	m_pszValue = new char[m_iCount + 1];
	memcpy(m_pszValue, rhs, m_iCount);
	m_pszValue[m_iCount] = '\0';
	return this;
}
void
CStr::operator+=(CStr * rhs)
{
	int iTotalLength = m_iCount + rhs->m_iCount;
	char * pszOld = m_pszValue;
	m_pszValue = new char[iTotalLength + 1];
	memcpy(m_pszValue, pszOld, m_iCount);
	memcpy(m_pszValue + m_iCount, rhs->m_pszValue, rhs->m_iCount);
	m_pszValue[iTotalLength] = '\0';
	delete pszOld;
	m_iCount = iTotalLength;
	if(rhs->m_bIsTemp){
		delete rhs;
	}
}
void 
CStr::operator+=(const char * rhs)
{
	int strLen = strlen(rhs);
	int iTotalLength = m_iCount + strLen;
	char * pszOld = m_pszValue;
	m_pszValue = new char[iTotalLength + 1];
	memcpy(m_pszValue, pszOld, m_iCount);
	memcpy(m_pszValue + m_iCount, rhs, strLen);
	m_pszValue[iTotalLength] = '\0';
	delete pszOld;
	m_iCount = iTotalLength;
}
void 
CStr::operator+=(int rhs)
{
	CStr * pAdd = new CStr(rhs);
	pAdd->m_bIsTemp = true;
	*this += pAdd;
}
void 
CStr::operator+=(float rhs)
{
	CStr * pAdd = new CStr(rhs);
	pAdd->m_bIsTemp = true;
	*this += pAdd;
}
void 
CStr::operator+=(char rhs)
{
	CStr * pAdd = new CStr(rhs);
	pAdd->m_bIsTemp = true;
	*this += pAdd;
}
bool
CStr::operator==(CStr * rhs)
{
	if(m_iCount != rhs->m_iCount)
	{
		return false;
	}
	for(int i = 0; i < m_iCount; i++)
	{
		if(m_pszValue[i] != rhs->m_pszValue[i])
		{
			return false;
		}
	}
	return true;
}
void 
CStr::Keep()
{
	m_bIsTemp = false;
}
const char * 
CStr::GetArray()
{
	return (const char*) m_pszValue;
}
CStr * 
CStr::Temp()
{
	m_bIsTemp = true;
	return this;
}
char 
CStr::operator[](int index)
{
	return m_pszValue[index];
}
CStr * 
CStr::operator()(int iValue)
{
	return (new CStr(iValue))->Temp();
}
CStr * 
CStr::operator()(float fValue)
{
	return (new CStr(fValue))->Temp();
}
CStr * 
CStr::Trim()
{
	int len = m_iCount;
	int st = 0;
	char * val = m_pszValue;
	while((st < len) && (val[st] <= ' ')){
		st++;
	}
	while((st < len) && (val[len - 1] <= ' ')){
		len--;
	}
	return ((st > 0) || (len < m_iCount)) ? Substring(st, len) : this;
}
CStr * 
CStr::Substring(int iBeginIndex)
{
	return Substring(iBeginIndex, m_iCount);
}
CStr * 
CStr::Substring(int iBeginIndex, int iEndIndex)
{
	if(iBeginIndex < 0)
	{
		return NULL;
	}
	if(iEndIndex > m_iCount)
	{
		return NULL;
	}
	if(iBeginIndex > iEndIndex)
	{
		return NULL;
	}
	//return ((iBeginIndex == 0) && (iEndIndex == m_iCount)) ? this :
	//	new CStr(m_pszValue, iBeginIndex, iEndIndex);
	return new CStr(m_pszValue, iBeginIndex, iEndIndex - iBeginIndex); // Always return new!
}
/*
CStr ** 
CStr::Split(CStr * pRegex, int iLimit)
{
	if(pRegex->m_iCount == 1)
	{
		char ch = (*pRegex)[0];
		int off = 0;
		int next = 0;
		bool limited = iLimit > 0;
		CLinkedList * pList = new CLinkedList();
		while((next = IndexOf(ch, off)) != 0)
		{
			if(!limited || pList->Length() < iLimit - 1)
			{
				pList->Add(Substring(off, next));
				off = next + 1;
			}
			else
			{
				pList->Add(Substring(off, m_iCount));
				off = m_iCount;
				break;
			}
		}
		// If no match was found, return this
		if(off == 0)
		{
			delete pList;
			CStr ** ppArray = new CStr*[1]();
			ppArray[0] = this;
			return ppArray;
		}
		// Add remaining segment
		if(!limited || pList->Length() < iLimit)
		{
			pList->Add(Substring(off, m_iCount));
		}

		// Construct result
		int resultSize = pList->Length();
		if(0 == iLimit)
		{
			CLinkedListNode * pNode = pList->GetFirst();
			while(NULL != pNode && pNode != pList->GetLast())
			{
				CStr * pStr = (CStr *)pNode->GetData();
				if(0 == pStr->Length())
				{
					resultSize--;
				}
				pNode = pNode->GetNext();
				pList->Remove(pStr);
				delete pStr;
			}
		}
		CStr ** ppArray = new CStr*[resultSize]();
		CLinkedListNode * pNode = pList->GetFirst();
		for(int i = 0; i < resultSize; i++)
		{
			ppArray[i] = (CStr*)pNode->GetData();
			pNode = pNode->GetNext();
		}
		delete pList;
		return ppArray;
	}
	return NULL;
}*/
CArrayList<CStr> *
CStr::Split(char ch, int iLimit)
{
	int off = 0;
	int next = 0;
	bool limited = iLimit > 0;
	CArrayList<CStr> * pList = new CArrayList<CStr>();
	while((next = IndexOf(ch, off)) != 0)
	{
		if(!limited || pList->Size() < iLimit - 1)
		{
			pList->Add(Substring(off, next));
			off = next + 1;
		}
		else
		{
			pList->Add(Substring(off, m_iCount));
			off = m_iCount;
			break;
		}
	}
	// If no match was found, return this
	if(off == 0)
	{
		return pList;
	}
	// Add remaining segment
	if(!limited || pList->Size() < iLimit)
	{
		pList->Add(Substring(off, m_iCount));
	}

	// Construct result
	int resultSize = pList->Size();
	if(0 == iLimit)
	{
		for(int i = 0; i < pList->Size(); i++)
		{
			if(0 == pList->Get(i)->Length())
			{
				delete pList->Remove(i);
			}
		}
	}
	return pList;
}
int 
CStr::IndexOf(char ch)
{
	return IndexOf(ch, 0);
}
int 
CStr::IndexOf(char ch, int iFromIndex)
{
	if(iFromIndex < 0)
	{
		iFromIndex = 0;
	}
	else if(iFromIndex >= m_iCount)
	{
		//Note: iFromIndex might be near -1>>>1.
		return -1;
	}
	const char * value = m_pszValue;
	const int max = m_iCount;
	for(int i = iFromIndex; i < max; i++)
	{
		if(value[i] == ch)
		{
			return i;
		}
	}
	return -1;
}
void 
CStr::ToLowerCase()
{
	for(int iChar = 0; iChar < m_iCount; iChar++)
	{
		m_pszValue[iChar] = (char)tolower((int)m_pszValue[iChar]);
	}
}
CStr *
CStr::FindExtension()
{
	return new CStr(PathFindExtension(m_pszValue));
}
bool 
CStr::IsInt()
{
	for(int i = 0; i < m_iCount; i++)
	{
		char c = m_pszValue[i];
		if(c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' && c != '7' && c != '8' && c != '9' && c != '0')
		{
			return false;
		}
	}
	return true;
}
bool 
CStr::IsFloat()
{
	bool dotFound = false;
	for(int i = 0; i < m_iCount; i++)
	{
		char c = m_pszValue[i];
		if(c == '.' && !dotFound)
		{
			dotFound = true;
		}
		else if(c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' && c != '7' && c != '8' && c != '9' && c != '0')
		{
			return false;
		}
	}
	return dotFound;
}
int 
CStr::AsInt()
{
	return atoi(m_pszValue);
}
float 
CStr::AsFloat()
{
	return (float)atof(m_pszValue);
}
int 
CStr::CompareTo(CIComparable<CStr> * pOther)
{
	CStr * pO = (CStr*)pOther;
	int len1 = m_iCount;
	int len2 = pO->m_iCount;
	int n = min(len1, len2);
	char * v1 = m_pszValue;
	char * v2 = pO->m_pszValue;

	int k = 0;
	int lim = n;
	while(k < lim){
		char c1 = v1[k];
		char c2 = v2[k];
		if(c1 != c2){
			return c1 - c2;
		}
		k++;
	}
	return len1 - len2;
}
bool 
CStr::Equals(CIComparable<CStr> * pOther)
{
	if(NULL != pOther)
	{
		return 0 == CompareTo(pOther);
	}
	return false;
}
bool 
CStr::Equals(char * pStr)
{
	CStr compare(pStr);
	return Equals(&compare);
}
bool 
CStr::Equals(const char * pStr)
{
	CStr compare(pStr);
	return Equals(&compare);
}