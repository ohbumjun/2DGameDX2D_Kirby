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

	// Ref Count는 저장하지 않는다
	// Ref Count의 경우, Ref 상속 객체의 생성과 함께 자동으로 1 증가하는데,
	// 이는 오히려 Load 하는 과정에서, 객체가 또 생성되는 과정을 거치면서, 꼬일 수 있다
	fwrite(&m_Enable, sizeof(bool), 1, pFile);
	fwrite(&m_Active, sizeof(bool), 1, pFile);
	fwrite(&m_TypeID, sizeof(size_t), 1, pFile);
}

void CRef::Load(FILE* pFile)
{
}
