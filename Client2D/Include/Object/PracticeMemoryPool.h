#pragma once

#include "GameInfo.h"

// �޸� Ǯ : �޸� �� ���� ������ 
class CPracticeMemoryPool
{
public :
	CPracticeMemoryPool();
	~CPracticeMemoryPool();
private :
	int m_BlockCountCanAllocate; // ������� �����ִ� �� ����
	int m_pBlockIdToAllocate; // �Ҵ��� ���� id(������)�� ����
	unsigned char* m_pPage; // ������ ���� �ּ�

	static const int MaxBlockCount = 1024; // �� �������� �Ҵ�Ǵ� �� ����
public :
	// �޸� Ǯ���� ���Ƶ��� �޸� ������ ���� ��, ���� ũ�⸸ŭ��
	// �޸� ���(Page) �� Heap �޸𸮿� �Ҵ�ް� ó���ؾ� �Ѵ�.
	// ������ �װ� �ȵ� ��Ȳ

	// �޸� Ǯ����, Ư�� ũ���� �޸� ���� ������ ��û�ϴ� �κ�
	void* Allocate(size_t Size);
	void Delocate(void* pToBlockDelete, size_t BlockSize);
};

