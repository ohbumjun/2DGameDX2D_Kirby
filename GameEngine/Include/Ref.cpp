#include "Ref.h"

CRef::CRef() :
	m_RefCount(0),
	m_Enable(true),
	m_Active(true),
	m_TypeID(0)
{
}

CRef::~CRef()
{
}

void CRef::Save(FILE* pFile)
{
	int Length = (int)m_Name.length();
	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), Length, pFile);

	// Ref Count�� �������� �ʴ´�
	// Ref Count�� ���, Ref ��� ��ü�� ������ �Բ� �ڵ����� 1 �����ϴµ�,
	// �̴� ������ Load �ϴ� ��������, ��ü�� �� �����Ǵ� ������ ��ġ�鼭, ���� �� �ִ�
	fwrite(&m_Enable, sizeof(bool), 1, pFile);
	fwrite(&m_Active, sizeof(bool), 1, pFile);
	fwrite(&m_TypeID, sizeof(size_t), 1, pFile);
}

void CRef::Load(FILE* pFile)
{
}
