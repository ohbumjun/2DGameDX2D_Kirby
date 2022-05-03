#pragma once

#include "GameInfo.h"

// 메모리 풀 : 메모리 블럭 조각 관리자 
class CPracticeMemoryPool
{
public :
	CPracticeMemoryPool();
	~CPracticeMemoryPool();
private :
	int m_BlockCountCanAllocate; // 현재까지 남아있는 블럭 개수
	int m_pBlockIdToAllocate; // 할당한 블럭의 id(포인터)를 저장
	unsigned char* m_pPage; // 페이지 시작 주소

	static const int MaxBlockCount = 1024; // 한 페이지에 할당되는 블럭 개수
public :
	// 메모리 풀에서 남아도는 메모리 블럭들이 없을 시, 원래 크기만큼의
	// 메모리 덩어리(Page) 를 Heap 메모리에 할당받게 처리해야 한다.
	// 지금은 그게 안된 상황

	// 메모리 풀에게, 특정 크기의 메모리 블럭을 실제로 요청하는 부분
	void* Allocate(size_t Size);
	void Delocate(void* pToBlockDelete, size_t BlockSize);
};

