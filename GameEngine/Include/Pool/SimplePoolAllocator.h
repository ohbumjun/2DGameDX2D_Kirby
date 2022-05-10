#pragma once

#include <memory>

template<typename T>
union SimplePoolChunk {
	/*
	 * Chunk 가 사용가능할 때, Chunk 안의 Next 는  그 다음 Chunk의 메모리 주소를 가지고 있다.
	 */
	T value;
	SimplePoolChunk<T>* m_NextPoolChunk;

	SimplePoolChunk() {}
	~SimplePoolChunk() {}
};

template<typename T>
class CSimplePoolAllocator {
private:
	static const size_t POOLALLOCATORDEFAULTSIZE = 1024;
	size_t m_BlockSize = 0;

	SimplePoolChunk<T>* m_Data = nullptr;
	SimplePoolChunk<T>* m_AllocPointer = nullptr;

public:
	CSimplePoolAllocator(const CSimplePoolAllocator& other) = delete;   // 복사 생성
	CSimplePoolAllocator(const CSimplePoolAllocator&& other) = delete; // 이동 생성
	CSimplePoolAllocator& operator = (const CSimplePoolAllocator& other) = delete; // 복사 대입
	CSimplePoolAllocator& operator = (const CSimplePoolAllocator&& other) = delete; // 이동 대입

	explicit CSimplePoolAllocator(size_t size = POOLALLOCATORDEFAULTSIZE) :
		m_BlockSize(size)
	{
		m_Data = new SimplePoolChunk<T>[m_BlockSize];
		m_AllocPointer = m_Data;

		// Chaining
		for (size_t i = 0; i < m_BlockSize - 1; ++i)
		{
			m_Data[i].m_NextPoolChunk = std::addressof(m_Data[i + 1]);
		}

		m_Data[m_BlockSize - 1].m_NextPoolChunk = nullptr;
	}

	~CSimplePoolAllocator()
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

		SimplePoolChunk<T>* poolChunk = m_AllocPointer;
		m_AllocPointer = m_AllocPointer->m_NextPoolChunk;

		T* NewVal = new (std::addressof(poolChunk->value)) T(std::forward<Arguments>(args)...);

		return NewVal;
	}

	template<typename ...Arguments>
	void allocateNewBlock(Arguments&& ...args)
	{
		SimplePoolChunk<T>* newData = new SimplePoolChunk<T>[m_BlockSize];
		m_AllocPointer = newData;

		for (size_t i = 0; i < m_BlockSize - 1; ++i)
		{
			newData[i].m_NextPoolChunk = std::addressof(newData[i + 1]);
		}

		newData[m_BlockSize - 1].m_NextPoolChunk = nullptr;
	}

	void deallocate(T* Data)
	{
		Data->~T();
		SimplePoolChunk<T>* poolChunk = reinterpret_cast<SimplePoolChunk<T>*>(Data);
		poolChunk->m_NextPoolChunk = m_AllocPointer;
		m_AllocPointer = poolChunk;
	}
};

