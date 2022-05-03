#pragma once

#include "GameInfo.h"

typedef struct SMemoryChunk {
	void* Data; // Actual Date
	std::size_t DataSize; // Total Size Of DataBlock Available
	std::size_t UsedSize; // Actual Used Size
	bool IsAllocationChunk;  // true, when this MemoryChunks
			                            // Points to a "Data"-Block
			                            // which can be deallocated via "free()"
	SMemoryChunk* Next; // Pointer To Next Memory Chunk In List (may be NULL)
} SMemoryChunk;

class CMemoryPool
{
public :
	// 생성자에서는 OS 로부터 큰 Memory Chunck 를 가져오게 된다.
	CMemoryPool(const std::size_t& InitializePoolSize,
		const std::size_t& MemoryChunkSize,
		const std::size_t& MinimalMemorySizeToAllocate,
		bool SetMemoryData);
private :
	SMemoryChunk* m_ptrFirstChunk;
	SMemoryChunk* m_ptrLastChunk;
	SMemoryChunk* m_ptrCursorChunk;

	size_t m_TotalMemoryPoolSize;
	size_t m_UsedMemoryPoolSize;
	size_t m_FreeMemoryPoolSize;

	unsigned int m_uiMemoryChunkCount;
	unsigned int m_uiMemoryChunkSize;
	unsigned int m_uiObjectCount;

	bool m_bSetMemoryData;

	size_t m_MinimalMemorySizeToAllocate;
private :
	bool AllocateMemory(size_t InitPoolSize);
};


