// Author/editor/coder: Simo
#pragma once
#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include <cstring>

template <class T>
class CArrayList
{
	//typedef CArrayList<T> self_type;
public:
	CArrayList()
	{
		Init(10);// With space for 10 elements!
	}
	CArrayList(int initialCapacity)
	{
		Init(initialCapacity);
	}
	~CArrayList(void)
	{
		if(NULL != m_ppData)
		{
			delete m_ppData;
		}
	}

	void TrimToSize()
	{
		int oldCapacity = m_iCapacity;
		if(m_iSize < oldCapacity)
		{
			T ** ppNew = new T*[m_iSize];
			T ** ppOld = m_ppData;
			memcpy(ppNew, ppOld, sizeof(T*) * m_iSize);
			m_ppData = ppNew;
			delete ppOld;
			m_iCapacity = m_iSize;
		}
	}
	void EnsureCapacity(int minCapacity)
	{
		if(minCapacity < 0)
		{
			return;
		}
		if(minCapacity - m_iCapacity > 0)
		{
			Grow(minCapacity);
		}
	}
	int Size()
	{
		return m_iSize;
	}
	bool IsEmpty()
	{
		return 0 == m_iSize;
	}
	bool Contains(T * o)
	{
		return IndexOf(o) >= 0;
	}
	int IndexOf(T * o)
	{
		for(int i = 0; i < m_iSize; i++)
		{
			if(o == m_ppData[i])
			{
				return i;
			}
		}
		return -1;
	}
	int LastIndexOf(T * o)
	{
		for(int i = m_iSize - 1; i > 0; i--)
		{
			if(o == m_ppData[i])
			{
				return i;
			}
		}
		return -1;
	}
	T ** ToArray()
	{
		T ** ppArray = new T*[m_iSize];
		memcpy(ppArray, m_ppData, m_iSize * sizeof(T*));
	}
	T * Get(int index)
	{
		return m_ppData[index];
	}
	T * Set(int index, T * element)
	{
		T * pOld = m_ppData[index];
		m_ppData[index] = element;
		return pOld;
	}
	bool Add(T * e)
	{
		EnsureCapacity(m_iSize + 1);
		m_ppData[m_iSize++] = e;
		return true;
	}
	bool Add(int index, T * element)
	{
		EnsureCapacity(m_iSize + 1);
		int unitSize = sizeof(T*);
		int length = (m_iSize - index) * unitSize;
		int oldPos = index * unitSize;
		int newPos = (index + 1) * unitSize;
		memcpy(m_ppData + newPos, m_ppData + oldPos, length);
		m_ppData[index] = element;
		return true;
	}
	T * Remove(int index)
	{
		T * oldValue = m_ppData[index];
		int numMoved = m_iSize - index - 1;
		if(numMoved > 0)
		{
			int unitSize = sizeof(T*);
			int offset = index * unitSize;
			int oldOffset = (index + 1) * unitSize;
			int left = (m_iSize - index) * unitSize;
			memcpy(m_ppData + offset, m_ppData + oldOffset, left);
		}
		m_iSize--;
		return oldValue;
	}
	bool Remove(T * o)
	{
		for(int i = 0; i < m_iSize; i++)
		{
			if(o == m_ppData[i])
			{
				fastRemove(i);
				return true;
			}
		}
		return false;
	}
	void DeleteAll()
	{
		// Deletes pointers and therefore the data with them!
		for(int i = 0; i < m_iSize; i++)
		{
			if(NULL != m_ppData[i])
			{
				delete m_ppData[i];
			}
		}
		m_iSize;
	}
	//bool AddAll(ArrayList<T> c);
	//bool AddAll(int index, ArrayList<T> c);
	//bool RemoveAll(ArrayList<T> c);
	//bool RetainAll(ArrayList<T> c);
	//ArrayList<T> * SubList(int fromIndex, int toIndex);
private:
	void Init(int iCapacity)
	{
		SIZE_OF = sizeof(T*);
		m_ppData = new T*[iCapacity];
		m_iCapacity = iCapacity;
		m_iSize = 0;
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
		T ** ppNewData = new T*[iNewCapacity];
		memcpy(ppNewData, m_ppData, SIZE_OF * m_iSize);
		T ** ppOldData = m_ppData;
		m_ppData = ppNewData;
		m_iCapacity = iNewCapacity;
		delete ppOldData;
	}
	void FastRemove(int index)
	{
		int numMoved = m_iSize - index - 1;
		if(numMoved > 0)
		{
			int unitSize = sizeof(T*);
			int offset = index * unitSize;
			int oldOffset = (index + 1) * unitSize;
			int left = (m_iSize - index) * unitSize;
			memcpy(m_ppData + offset, m_ppData + oldOffset, left);
		}
		m_iSize--;
	}


	int m_iCapacity;
	int m_iSize;
	T ** m_ppData;
	int SIZE_OF; // probably just sizeof(int) = 4bytes (pointer)..
	static const int MAX_SIZE = 0x7fffffff - 8;
};

#endif