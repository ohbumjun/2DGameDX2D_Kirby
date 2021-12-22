#include "Component.h"

CComponent::CComponent() :
	m_Scene(nullptr),
	m_Object(nullptr)
{
}

CComponent::CComponent(const CComponent& com)
{
	*this      = com;
	m_RefCount = 0;

	m_Object = nullptr;
}

CComponent::~CComponent()
{
}

void CComponent::SetScene(CScene* Scene)
{
	m_Scene = Scene;
}

void CComponent::SetGameObject(CGameObject* Object)
{
	m_Object = Object;
}

void CComponent::Start()
{
}

bool CComponent::Init()
{
	return true;
}

void CComponent::Save(FILE* pFile)
{
	CRef::Save(pFile);

	// Scene, Object ������, Load�Ǵ� �������� �ڵ����� ���õǱ� ������ Save ���� �ʴ� ���ΰ� ?
	fwrite(&m_ComponentType, sizeof(Component_Type), 1, pFile);
}

void CComponent::Load(FILE* pFile)
{
}
