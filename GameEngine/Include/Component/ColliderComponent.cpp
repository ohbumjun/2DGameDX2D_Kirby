#include "ColliderComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"

CColliderComponent::CColliderComponent()
{
	SetTypeID<CColliderComponent>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;
}

CColliderComponent::CColliderComponent(const CColliderComponent& com) :
CSceneComponent(com)
{
}

CColliderComponent::~CColliderComponent()
{
}

void CColliderComponent::Start()
{
	CSceneComponent::Start();
}

bool CColliderComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;
	return true;
}

void CColliderComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CColliderComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CColliderComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CColliderComponent::Render(HDC hDC)
{
}

void CColliderComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CColliderComponent* CColliderComponent::Clone()
{
	return new CColliderComponent(*this);
}

void CColliderComponent::CheckCollision()
{
	// 1) �̷��� ��� �浹ü ��ϵ��� SceneCollision ���� �浹ü ��Ͽ� ��Ƶΰ�
	// 2) SceneCollision ���� CheckColliderSection() �Լ��� ���� �� Collider Component�� �ڱⰡ ���� ������ ��ġ���ִ� �� 
	m_Scene->GetCollision()->AddCollider(this);

	CSceneComponent::CheckCollision();
}

void CColliderComponent::Save(FILE* pFile)
{
	fwrite(&m_ColliderType, sizeof(Collider_Type), 1, pFile);
	fwrite(&m_Offset, sizeof(Vector3), 1, pFile);
	fwrite(&m_Max, sizeof(Vector3), 1, pFile);
	fwrite(&m_Min, sizeof(Vector3), 1, pFile);

	CSceneComponent::Save(pFile);
}

void CColliderComponent::Load(FILE* pFile)
{
	fread(&m_ColliderType, sizeof(Collider_Type), 1, pFile);
	fread(&m_Offset, sizeof(Vector3), 1, pFile);
	fread(&m_Max, sizeof(Vector3), 1, pFile);
	fread(&m_Min, sizeof(Vector3), 1, pFile);

	CSceneComponent::Load(pFile);
}
