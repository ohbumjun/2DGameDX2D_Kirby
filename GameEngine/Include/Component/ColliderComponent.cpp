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
	// 1) 이렇게 모든 충돌체 목록들을 SceneCollision 내의 충돌체 목록에 모아두고
	// 2) SceneCollision 내의 CheckColliderSection() 함수를 통해 각 Collider Component를 자기가 속한 영역에 배치해주는 것 
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
