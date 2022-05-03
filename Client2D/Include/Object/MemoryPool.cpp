#include "MemoryPool.h"

CMemoryPool::CMemoryPool(const std::size_t& InitializePoolSize, 
	const std::size_t& MemoryChunkSize,
	const std::size_t& MinimalMemorySizeToAllocate, bool SetMemoryData)
{
	m_ptrFirstChunk    = nullptr;
	m_ptrLastChunk     = nullptr;
	m_ptrCursorChunk = nullptr;

	m_TotalMemoryPoolSize = 0;
	m_UsedMemoryPoolSize = 0;
	m_FreeMemoryPoolSize = 0;

	// 총 Memory Chunk의 Size ?
	m_uiMemoryChunkSize = MemoryChunkSize;
	// Memory Chunk 개수
	m_uiMemoryChunkCount = 0;
	// Object 개수 ?
	m_uiObjectCount = 0;

	m_bSetMemoryData = SetMemoryData;

	m_MinimalMemorySizeToAllocate = MinimalMemorySizeToAllocate;
}

bool CMemoryPool::AllocateMemory(size_t InitPoolSize)
{
	return true;
}
