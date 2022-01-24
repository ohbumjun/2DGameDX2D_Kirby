#pragma once

#include "GameInfo.h"
#include "Sync.h"
#include <assert.h>

template<typename T, int SIZE = 200>
class CThreadQueue {
public :
	CThreadQueue()
{
		m_Head = 0;
		m_Tail   = 0;
		m_Size  = 0;
		m_Capacity = SIZE + 1;
		InitializeCriticalSection(&m_Crt);
}
	~CThreadQueue()
{
		DeleteCriticalSection(&m_Crt);
}
private :
	int m_Head;
	int m_Tail;
	int m_Size;
	int m_Capacity;
	T m_Queue[SIZE + 1];
	CRITICAL_SECTION m_Crt;
public :
	T& front()
{
		if (empty())
			assert(false);

		CSync sync(&m_Crt);

		int Head = (m_Head + 1) % m_Capacity;

		return m_Queue[Head];
}
	void pop()
{
		if (empty())
			assert(false);

		CSync sync(&m_Crt);

		m_Head = (m_Head + 1) % m_Capacity;

		--m_Size;
}
	void push(T Elem)
{
		CSync sync(&m_Crt);

		int Tail = (m_Tail + 1) % m_Capacity;

		if (Tail == m_Head)
			return;

		m_Queue[Tail] = Elem;

		m_Tail = Tail;

		++m_Size;
}
	bool empty()
{
		CSync sync(&m_Crt);

		return m_Size == 0;
}
	int size()
{
		CSync sync(&m_Crt);

		return m_Size;
}
	void clear()
{
		CSync sync(&m_Crt);

		m_Size  = 0;
		m_Head = 0;
		m_Tail    = 0;
}
};