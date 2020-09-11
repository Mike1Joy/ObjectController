// Author/editor/coder: Simo
#pragma once
#include <Windows.h>

/*
* Based on "Simple, Fast, and Practical Non-Blocking and Blocking Concurrent Queue Algorithms"
* Abstract at:
* http://www.cs.rochester.edu/u/michael/PODC96.html
* Or pdf at:
* http://www.cs.rochester.edu/u/scott/papers/1996_PODC_queues.pdf
*
* InterlockedCompareExchangePointer is atomic compare_and_swap (CAS) function, details at:
* http://msdn.microsoft.com/en-us/library/ms683568(v=vs.85).aspx
*/

template <class T>
class CNode
{
public:
	CNode(void)
	{
		m_pValue = NULL;
		m_pNext = NULL;
	}
	CNode(T * pValue)
	{
		m_pValue = pValue;
		m_pNext = NULL;
	}
	~CNode(void)
	{
	}

	volatile T * m_pValue;
	volatile CNode<T> * m_pNext;
};

template <class T>
class CConcurrentLinkedQueue
{
public:
	CConcurrentLinkedQueue(void)
	{
		CNode<T> * pNode = new CNode<T>();
		m_pHead = m_pTail = pNode;
	}
	~CConcurrentLinkedQueue(void)
	{
		while(NULL != Take());
		if(NULL != m_pTail)
		{
			if(m_pTail == m_pHead)
			{
				delete m_pTail;
				m_pTail = m_pHead = NULL;
			}
			else
			{
				delete m_pTail;
				m_pTail = NULL;
			}
		}
		if(NULL != m_pHead)
		{
			delete m_pHead;
			m_pHead = NULL;
		}
	}

	bool Add(T * pValue)
	{
		if(NULL == pValue || NULL == m_pTail)
		{
			return false;
		}

		// New node
		volatile CNode<T> * pNode = new CNode<T>();
		pNode->m_pValue = pValue;
		// m_pNext is already NULL!
		while(true)
		{
			volatile CNode<T> * pTail = m_pTail;
			volatile CNode<T> * pNext = pTail->m_pNext;
			if(pTail == m_pTail)
			{
				if(pNext == NULL) // m_pTail is the last node!
				{
					if(pNext == InterlockedCompareExchangePointer((PVOID volatile *)&pTail->m_pNext, (PVOID)pNode, (PVOID)pNext)) // CAS if next is still NULL
					{
						pNext = pNode; // pNext is now pNode!
						InterlockedCompareExchangePointer((PVOID volatile *)&m_pTail, (PVOID)pNode, (PVOID)pTail); // Failure is OK
						return true;
					}
				}
				else // m_pTail is not the last node for some reason!
				{
					InterlockedCompareExchangePointer((PVOID volatile *)&m_pTail, (PVOID)pNext, (PVOID)pTail); // Try to swing m_pTail to the m_pNext node
				}
			}
		}
		return false;
	}
	T * Take()
	{
		while(true)
		{
			volatile CNode<T> * pHead = m_pHead;
			volatile CNode<T> * pTail = m_pTail;
			volatile CNode<T> * pNext = m_pHead->m_pNext;
			if(pHead == m_pHead)
			{
				if(pHead == pTail)
				{
					if(pHead->m_pNext == NULL)
					{
						return NULL; // Queue is empty
					}
					InterlockedCompareExchangePointer((PVOID volatile *)&m_pTail, (PVOID)pNext, (PVOID)pTail); // Tail is falling behind. Try to advance it
				}
				else // No need to deal with tail!
				{
					T * pValue = (T *)pNext->m_pValue;
					if(pHead == InterlockedCompareExchangePointer((PVOID volatile *)&m_pHead, (PVOID)pNext, (PVOID)pHead))
					{
						delete pHead; // It is now safe to delete old head!
						return pValue;
					}
				}
			}
		}
		return NULL;
	}
	bool IsEmpty()
	{
		return m_pHead == m_pTail && m_pHead->m_pNext == NULL;
	}
	void DeleteAllData()
	{
		bool bDeleted = true;
		while(bDeleted)
		{
			T * pObject = Take();
			if(NULL != pObject)	delete pObject;
			else bDeleted = false;
		}
		// The following lines causes some weird compilation errors, so we use the ones above instead!
		//T * pObject = NULL;
		//while(NULL != (pObject = Take()) delete pObject;
	}

protected:
	volatile CNode<T> * m_pHead;
	volatile CNode<T> * m_pTail;
};
