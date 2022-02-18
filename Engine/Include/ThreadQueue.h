#pragma once

#include "GameInfo.h"
#include "Sync.h"
#include <assert.h>

template<typename T, int SIZE = 200>
class CThreadQueue {
public :
	CThreadQueue()
{
		m_Size = 0;
		m_Head = 0;
		m_Tail = 0;
		m_Capacity = SIZE + 1;

		InitializeCriticalSection(&m_Crt);
}
	~CThreadQueue()
{
		DeleteCriticalSection(&m_Crt);
}
private :
	T m_Queue[SIZE + 1];
	int m_Capacity;
	int m_Size;
	int m_Head;
	int m_Tail;

	CRITICAL_SECTION m_Crt;
public :
	void push (const T& data)
{
		CSync sync(&m_Crt);

		int Tail = (m_Tail + 1) % m_Capacity;

		if (Tail == m_Head)
			return;

		m_Queue[Tail] = data;

		m_Tail = Tail;

		++m_Size;
}
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
		{
			assert(false);
			return;
		}

		CSync sync(&m_Crt);

		m_Head = (m_Head + 1) % m_Capacity;
		m_Size -= 1;
}

	int size() 
{
		CSync sync(&m_Crt);
		return m_Size;
}
	bool empty()
{
		CSync sync(&m_Crt);

		return m_Size == 0;
}
	void clear()
{
		CSync sync(&m_Crt);

		m_Head = 0;
		m_Tail = 0;
		m_Size = 0;
}
};