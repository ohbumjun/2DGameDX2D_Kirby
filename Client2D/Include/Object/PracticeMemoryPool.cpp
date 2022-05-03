#include "PracticeMemoryPool.h"

CPracticeMemoryPool::CPracticeMemoryPool()
{}

CPracticeMemoryPool::~CPracticeMemoryPool()
{}

void* CPracticeMemoryPool::Allocate(size_t BlockSize)
{
	// BlockSize : �� ����� ũ�� 

	// �������� �Ҵ���� �ʾ��� ��� --> ������ (��ü ����) ����
	if (!m_pPage)
	{
		m_pPage = (unsigned char*)malloc(BlockSize * MaxBlockCount);

		// �����ִ� �� ����
		m_BlockCountCanAllocate = MaxBlockCount;

		// unsigned char : 1 ����Ʈ
		unsigned char* p = m_pPage;

		// ������ ������ �ּ� 4byte �� �ڽ��� ���� ���� ����Ű�� �ε����� �ִ´�
		for (int i = 0; i < MaxBlockCount; ++i)
		{
			(*p) = i + 1; // ��ũ id (���� ����� Index) �� �� ���´�.
			p += BlockSize; // ���� ������ �̵� --> ����Ʈ ���� �̵�
		}
	}

	// �̹� ������� ����� �� ���־, �Ҵ��� ������ ���� ���
	if (m_BlockCountCanAllocate == 0)
	{
		// ���Ӱ� �� �Ҵ��ϴ� �ڵ带 �����ؾ� �Ѵ�.
		return nullptr; // �ϴ� NULL ��ȯ
	}

	// ��û�� ��(�ּ�)�� �Ѱ��ش�.
	// ��û�� ��� �ּҸ� ����Ѵ�.
	unsigned char* pBlockToAllocate = m_pPage + (m_pBlockIdToAllocate * BlockSize);

	// m_pBlockIdToAllocate (���� �Ҵ��� ����� ����Ű��) ��û�� ����
	// ���� �� (Id) �� ����Ų��.
	m_pBlockIdToAllocate = *pBlockToAllocate;

	// �Ҵ��� ����� �������Ƿ�, �޸� Ǯ�� �����ִ� ������ ���δ�
	m_BlockCountCanAllocate--;

	// ����� ��û ���� ��ȯ�Ѵ�.
	return (void*)pBlockToAllocate;

	// 2���� ������ �ִ�
	// 1) �޸� Ǯ�� �����ϱ� ���� OS�κ��� ū �޸� ��� (������)�� �Ҵ� �ް�
	// �� �޸� ����� ��û�� ũ���� ��ü ������ �����ϴ� �۾��� �����Ѵ�.
	// Allocate �� ���ʷ� ��û�ϴ� �������� �� ������ �����Ѵ�.

	// 2) ����ڷκ��� �޸� �Ҵ� ��û�� ������, ���� �޸� Ǯ��
	// �����ϰ� �ִ� (���� �Ҵ���� ����) �޸� �� �ϳ��� �Ѱ��ش�.
}

void CPracticeMemoryPool::Delocate(void* pToBlockDelete, size_t BlockSize)
{
	// �Ҵ�ƴ� �޸�.�� �����Ͽ�, �޸� Ǯ�� ����������.

	// �Ҵ簡���� ���� ������, ��ü ����� ������ ���� ����
	// ��, �޸� Ǯ�� �޸� �Ҵ��� �̷����� ���� ���¿���
	// Delocate() �� ȣ��Ǵ� ���� �����̴�.
	if (m_BlockCountCanAllocate >= MaxBlockCount)
		return;

	// ���� �Ҵ��� �� Id (m_pBlockIdToAllocate �� ����Ű��) �� ������
	// ���� ����´�.
	// ��, ������ �޸� ���� ���� �� Id �� m_pBlockIdToAllocate �̴�.
	*(int*)pToBlockDelete = m_pBlockIdToAllocate;

	// ������ ���� Id (���� ID. ��, ������ �����ּҷκ��� �������� �Ÿ�)�� ���Ѵ�.
	int Offset = ((int)(unsigned char*)(pToBlockDelete)- *m_pPage) / (int)BlockSize;

	// ���� �Ҵ��� ���� ����Ű�� Id ���� �����Ѵ�.
	m_pBlockIdToAllocate = Offset;

	++m_BlockCountCanAllocate; // �Ҵ� ������ �� ������ �ø���.
}
