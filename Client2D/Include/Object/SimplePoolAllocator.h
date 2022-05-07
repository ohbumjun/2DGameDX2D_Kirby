#pragma once

template<typename T>
struct SimplePoolChunk {
	/*
	 * Chunk �� ��밡���� ��, Chunk ���� Next ��  �� ���� Chunk�� �޸� �ּҸ� ������ �ִ�.
	 */
	T value;
	SimplePoolChunk<T>* m_NextPoolChunk;

	SimplePoolChunk(){}
	~SimplePoolChunk(){}
};

template<typename T>
class PoolAllocator {
private :
	static const size_t POOLALLOCATORDEFAULTSIZE = 1024;
	size_t m_BlockSize = 0;

	SimplePoolChunk<T>* m_Data = nullptr;
	SimplePoolChunk<T>* m_AllocPointer = nullptr;

public :
	PoolAllocator(const PoolAllocator& other) = delete;   // ���� ����
	PoolAllocator(const PoolAllocator&& other) = delete; // �̵� ����
	PoolAllocator& operator = (const PoolAllocator& other) = delete; // ���� ����
	PoolAllocator& operator = (const PoolAllocator&& other) = delete; // �̵� ����

	explicit PoolAllocator (size_t size = POOLALLOCATORDEFAULTSIZE) :
		m_BlockSize(size)
	{
		m_Data = new SimplePoolChunk<T>[m_BlockSize];
		m_AllocPointer = m_Data;

		// Chaining
		for (size_t i = 0; i < m_BlockSize - 1; ++i)
		{
			m_Data[i]->m_NextPoolChunk = m_Data[i + 1];
		}

		m_Data[m_BlockSize - 1].m_NextPoolChunk = nullptr;
	}

	~PoolAllocator()
	{
		delete[] m_Data;
		m_Data = nullptr;
		m_AllocPointer = nullptr;
	}

	template<typename ...Arguments>
	T* allocate(Arguments&& ...args)
	{
		if (m_AllocPointer == nullptr)
		{
			allocateNewBlock(args...);
		}
	}

	template<typename ...Arguments>
	void allocateNewBlock(Arguments&& ...args)
	{
		SimplePoolChunk<T>* NewData = new SimplePoolChunk<T>[m_BlockSize];
		m_AllocPointer = NewData;

		for (size_t i = 0; i < m_BlockSize - 1; ++i)
		{
			NewData[i]->m_NextPoolChunk = NewData[i + 1];
		}

		NewData[m_BlockSize - 1]->m_NextPoolChunk = nullptr;
	}
};

class CSimplePoolAllocator
{
};

