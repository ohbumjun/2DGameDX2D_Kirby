#include "PracticeMemoryPool.h"

CPracticeMemoryPool::CPracticeMemoryPool()
{}

CPracticeMemoryPool::~CPracticeMemoryPool()
{}

void* CPracticeMemoryPool::Allocate(size_t BlockSize)
{
	// BlockSize : 한 블록의 크기 

	// 페이지가 할당되지 않았을 경우 --> 페이지 (전체 블럭들) 생성
	if (!m_pPage)
	{
		m_pPage = (unsigned char*)malloc(BlockSize * MaxBlockCount);

		// 남아있는 블럭 개수
		m_BlockCountCanAllocate = MaxBlockCount;

		// unsigned char : 1 바이트
		unsigned char* p = m_pPage;

		// 각각의 블럭마다 최소 4byte 에 자신의 다음 블럭을 가리키는 인덱스를 넣는다
		for (int i = 0; i < MaxBlockCount; ++i)
		{
			(*p) = i + 1; // 링크 id (다음 블록의 Index) 를 써 놓는다.
			p += BlockSize; // 다음 블럭으로 이동 --> 바이트 단위 이동
		}
	}

	// 이미 사용중인 블록이 꽉 차있어서, 할당할 공간이 없을 경우
	if (m_BlockCountCanAllocate == 0)
	{
		// 새롭게 더 할당하는 코드를 마련해야 한다.
		return nullptr; // 일단 NULL 반환
	}

	// 요청한 블럭(주소)를 넘겨준다.
	// 요청한 블록 주소를 계산한다.
	unsigned char* pBlockToAllocate = m_pPage + (m_pBlockIdToAllocate * BlockSize);

	// m_pBlockIdToAllocate (현재 할당할 블록을 가리키는) 요청한 블럭의
	// 다음 블럭 (Id) 를 가리킨다.
	m_pBlockIdToAllocate = *pBlockToAllocate;

	// 할당한 블록이 생겼으므로, 메모리 풀에 남아있는 개수를 줄인다
	m_BlockCountCanAllocate--;

	// 블록을 요청 측에 반환한다.
	return (void*)pBlockToAllocate;

	// 2가지 역할이 있다
	// 1) 메모리 풀을 생성하기 위해 OS로부터 큰 메모리 덩어리 (페이지)를 할당 받고
	// 이 메모리 덩어리를 요청한 크기의 객체 단위로 분할하는 작업을 수행한다.
	// Allocate 가 최초로 요청하는 순간에만 이 역할을 수행한다.

	// 2) 사용자로부터 메모리 할당 요청이 들어오면, 현재 메모리 풀이
	// 관리하고 있는 (아직 할당되지 않은) 메모리 블럭 하나를 넘겨준다.
}

void CPracticeMemoryPool::Delocate(void* pToBlockDelete, size_t BlockSize)
{
	// 할당됐던 메모리.를 해제하여, 메모리 풀로 돌려보낸다.

	// 할당가능한 블럭의 개수가, 전체 블록의 개수와 같은 상태
	// 즉, 메모리 풀에 메모리 할당이 이뤄지지 않은 상태에서
	// Delocate() 가 호출되는 것은 오류이다.
	if (m_BlockCountCanAllocate >= MaxBlockCount)
		return;

	// 현재 할당할 블럭 Id (m_pBlockIdToAllocate 가 가리키는) 를 해제한
	// 블럭에 써놓는다.
	// 즉, 해제된 메모리 블럭의 다음 블럭 Id 는 m_pBlockIdToAllocate 이다.
	*(int*)pToBlockDelete = m_pBlockIdToAllocate;

	// 해제된 블럭의 Id (블럭의 ID. 즉, 페이지 시작주소로부터 블럭까지의 거리)를 구한다.
	int Offset = ((int)(unsigned char*)(pToBlockDelete)- *m_pPage) / (int)BlockSize;

	// 현재 할당할 블럭을 가리키는 Id 값을 설정한다.
	m_pBlockIdToAllocate = Offset;

	++m_BlockCountCanAllocate; // 할당 가능한 블럭 개수를 늘린다.
}
