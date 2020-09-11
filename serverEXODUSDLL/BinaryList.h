// Author/editor/coder: Simo
#pragma once
#include "IComparable.h"
/*
* Bodies of member functions of template class must be declared in the header
* or be declared as inline or as export
*/
template <class T>
class CBinaryList
{
public:
	CBinaryList<T>(void)
	{
		SIZE_OF = sizeof(CIComparable<T>*);
		// Make 10 as initial capacity
		m_iSize = 0;
		m_iCapacity = 10;
		m_ppData = new CIComparable<T>*[m_iCapacity];
	}
	CBinaryList(int iInitialCapacity)
	{
		SIZE_OF = sizeof(CIComparalble<T>*);
		m_iSize = 0;
		m_iCapacity = iInitialCapacity;
		m_ppData = new CIComparable<T>*[m_iCapacity];
	}
	~CBinaryList(void)
	{
		if(NULL != m_ppData)
		{
			delete m_ppData;
		}
	}

	bool Add(CIComparable<T> * pObject)
	{
		if(NULL == pObject)
		{
			return false; // No empties!
		}
		int iIndex = Search(pObject);
		if(iIndex >= 0)
		{
			return false; // already exists!
		}
		iIndex = -iIndex-1;
		Insert(pObject, iIndex);
		return true;
	}

	CIComparable<T> * Get(int iIndex)
	{
		return m_ppData[iIndex];
	}
	CIComparable<T> * Get(CIComparable<T> * pObject)
	{
		int iIndex = Search(pObject);
		if(iIndex >= 0)
		{
			return Get(iIndex);
		}
		return NULL;
	}
	CIComparable<T> * Remove(int iIndex)
	{
		CIComparable<T> * pRet = m_ppData[i];
		//memcpy doesn't work and memmove is probably slower, so shift manually
		for(int i = iIndex; i < m_iSize - 1; i++)
		{
			m_ppData[i] = m_ppData[i + 1]; // Shift one up;
		}
		return pRet;
	}
	CIComparable<T> * Remove(CIComparable<T> * pObject)
	{
		int iIndex = Search(pObject);
		if(iIndex < 0)
		{
			return NULL;
		}
		return Remove(iIndex);
	}

	bool Contains(CIComparable<T> * pObject)
	{
		return Search(pObject) >= 0;
	}
	int Size()
	{
		return m_iSize;
	}
	void EnsureCapacity(int iMinCapacity)
	{

		if(iMinCapacity < 0)
		{
			return;
		}
		if(iMinCapacity - m_iCapacity > 0)
		{
			Grow(iMinCapacity);
		}
	}
	void TrimToSize()
	{

		int iOldCapacity = m_iCapacity;
		if(m_iSize < iOldCapacity)
		{
			CIComparable<T> ** ppNew = new CIComparable<T>*[m_iSize];
			CIComparable<T> ** ppOld = m_ppData;
			memcpy(ppNew, ppOld, SIZE_OF * m_iSize);
			m_ppData = ppNew;
			delete ppOld;
			m_iCapacity = m_iSize;
		}
	}
	int Search(CIComparable<T> * pKey)
	{
		int iLow = 0;
		int iHigh = Size() - 1;
		while(iLow <= iHigh)
		{
			int iMid = (iLow + iHigh) >> 1;
			CIComparable<T> * pMidVal = Get(iMid);
			int iCmp = pMidVal->CompareTo(pKey);

			if(iCmp < 0)
			{
				iLow = iMid + 1;
			}
			else if(iCmp > 0)
			{
				iHigh = iMid - 1;
			}
			else
			{
				return iMid; // Key found!
			}
		}
		return -(iLow + 1); // Key not found!
	}
	void DeleteAll()
	{

		for(int i = 0; i < m_iSize; i++)
		{
			delete m_ppData[i];
		}
		m_iSize = 0;
	}
private:
	// The location must be searched first! Messes sorting otherwise!
	void Insert(CIComparable<T> * pObject, int iIndex)
	{
		EnsureCapacity(m_iSize + 1);
		//memcpy doesn't work and memmove is probably slower, so shift manually
		for(int i = m_iSize; i > iIndex; i--)
		{
			m_ppData[i] = m_ppData[i - 1]; // Shift one up;
		}
		m_ppData[iIndex] = pObject;
		m_iSize++;
	}
	void Grow(int iMinCapacity)
	{
		int iOldCapacity = m_iCapacity;
		 // Increase 50 percent
		int iNewCapacity = iOldCapacity + (iOldCapacity >> 1);
		if(iNewCapacity - iMinCapacity < 0)
			iNewCapacity = iMinCapacity;
		if(iNewCapacity - MAX_SIZE > 0)
			iNewCapacity = MAX_SIZE;
		CIComparable<T> ** ppNewData = new CIComparable<T>*[iNewCapacity];
		memcpy(ppNewData, m_ppData, SIZE_OF * m_iSize);
		CIComparable<T> ** ppOldData = m_ppData;
		m_ppData = ppNewData;
		m_iCapacity = iNewCapacity;
		delete ppOldData;
	}

	// Variables
	int m_iCapacity;
	int m_iSize;
	CIComparable<T> ** m_ppData;
	int SIZE_OF; // probably just sizeof(int) = 4bytes (pointer)..
	static const int MAX_SIZE = 0x7ffffff7; // 8 less than max signed integer, just to be safe
};

