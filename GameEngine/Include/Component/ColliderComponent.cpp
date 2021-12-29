#include "ColliderComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"
#include "../Collision/CollisionManager.h"

CColliderComponent::CColliderComponent()
{
	SetTypeID<CColliderComponent>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;
	m_CurrentSectionCheck = false;
	m_MouseCollision = false;
	m_Profile = nullptr;
	m_Mesh = nullptr;
}

CColliderComponent::CColliderComponent(const CColliderComponent& com) :
CSceneComponent(com)
{
	m_MouseCollision = false;
	m_CurrentSectionCheck = false;
	m_Render = com.m_Render;
	// Profile 정보는 공유해서 사용할 것이다. 
	m_Profile = com.m_Profile;
}

CColliderComponent::~CColliderComponent()
{
	// 사라지게 되면 , 이전에 충돌했던 목록 애들한테 나 이제 너희와의 충돌 목록에서 지워줘 ! 라고 세팅해준다.
	auto iter = m_PrevCollisionList.begin();
	auto iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->DeletePrevCollision(this);
		(*iter)->CallCollisionMouseCallback(Collision_State::End);
		CallCollisionMouseCallback(Collision_State::End);
	}
}

void CColliderComponent::SetCollisionProfile(const std::string& Name)
{
	m_Profile = CCollisionManager::GetInst()->FindCollisionProfile(Name);
}

void CColliderComponent::CheckPrevColliderSection()
{
	// 충돌체들에 대해서, 현재 프레임 상 다른 충돌 영역에서 이미 충돌되었는지를 판단하는 코드 

	// 이전 충돌 목록에 대해서 처리를 해줘야 한다
	// 이전 충돌체들의 Section 들을 가져와서 비교한다.
	// 이전 프레임에 충돌되었던 충돌체들을 반복하여 영역이 겹치는지 판단
	// 이전 프레임에 충돌이 되었는데 , 현재는 서로 다른 영역에 존재한다면
	// 해당 충돌체와는 충돌 가능성이 없으므로,
	// 충돌되었다가, 떨어지는 것으로 판단해준다.

	auto iter = m_PrevCollisionList.begin();
	auto iterEnd = m_PrevCollisionList.end();

	// 모든 이전 충돌 목록들을 돌면서
	// 자신이 속한 모든 충돌 영역과, 이전 충돌체가 속한 모든 충돌 영역들을 비교한다
	size_t Size = m_vecSectionIndex.size();
	for (; iter != iterEnd;)
	{
		bool Check = false;

		// 이전 충돌 체들 목록들 모두에 대해서, 
		// 해당 충돌체가 속한 모든 충돌 영역을 체크한다
		for (size_t i = 0; i < Size; i++)
		{
			size_t DestSize = (*iter)->m_vecSectionIndex.size();
			for (size_t j = 0; j < DestSize; j++)
			{
				// 자신이 속한 충돌체와, 이전 충돌 목록에 속한 충돌체의 충돌 영역이 같다면
				if (m_vecSectionIndex[i] == (*iter)->m_vecSectionIndex[j])
				{
					// 겹친다고 표시
					Check = true;
					break;
				}
			}
			if (Check)
				break;
		}

		// 서로 겹치는 충돌 영역이 없으므로, 이전 프레임에는 붙어있다가 현재 프레임에서는 떨어지는 것이다 
		if (!Check)
		{
			// 콜백함수 호출 
			CallCollisionCallback(Collision_State::End);
			(*iter)->CallCollisionCallback(Collision_State::End);

			// 서로 이전 충돌 목록에서 제거해주기
			(*iter)->DeletePrevCollision(this);
			DeletePrevCollision(*iter);

			iter = m_PrevCollisionList.erase(iter);
			iterEnd = m_PrevCollisionList.end();
			continue;
		}
		++iter;
	}
}

void CColliderComponent::AddPrevCollision(CColliderComponent* Collider)
{
	m_PrevCollisionList.push_back(Collider);
}

void CColliderComponent::DeletePrevCollision(CColliderComponent* Collider)
{
	auto iter = m_PrevCollisionList.begin();
	auto iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd;)
	{
		if ((*iter) == Collider)
		{
			m_PrevCollisionList.erase(iter);
			return;
		}
	}
}

bool CColliderComponent::EmptyPrevCollision()
{
	return m_PrevCollisionList.empty();
}

bool CColliderComponent::CheckPrevCollision(CColliderComponent* Collider)
{
	auto iter = m_PrevCollisionList.begin();
	auto iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == Collider)
			return true;
	}
	return false;
}

bool CColliderComponent::CheckCurrentFrameCollision(CColliderComponent* Collider)
{
	auto iter = m_CurrentCollisionList.begin();
	auto iterEnd = m_CurrentCollisionList.end();

	for (;iter != iterEnd; ++iter)
	{
		if (*iter == Collider)
			return true;
	}

	return false;
}

void CColliderComponent::AddCurrentFrameCollision(CColliderComponent* Collider)
{
	if (!CheckCurrentFrameCollision(Collider))
	{
		m_CurrentCollisionList.push_back(Collider);
	}
}

void CColliderComponent::CallCollisionCallback(Collision_State State)
{
	auto iter = m_CollisionListCallback[(int)State].begin();
	auto iterEnd = m_CollisionListCallback[(int)State].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)(m_Result);
	}
}

void CColliderComponent::CallCollisionMouseCallback(Collision_State State)
{
	auto iter = m_CollisionMouseCallback[(int)State].begin();
	auto iterEnd = m_CollisionListCallback[(int)State].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)(m_Result);
	}
}

void CColliderComponent::ClearFrame()
{
	// 한 프레임이 종료되면, Clear Frame을 호출해서
	// 새롭게 충돌을 세팅할 수 있게 해준다
	m_vecSectionIndex.empty(); // 현재 충돌 영역 clear
	m_CurrentCollisionList.clear(); // 현재 충돌한 목록 clear
	m_CurrentSectionCheck = false; // 현재 충돌 목록에서의 충돌 여부도 false
}

void CColliderComponent::Start()
{
	CSceneComponent::Start();
}

bool CColliderComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	// 실제 충돌을 처리하기 위한 충돌 프로파일
	// 2개가 충돌 할때, 둘중 하나라도 서로를 충돌 프로파일 상, 충돌을 안하는 것으로 처리했다면
	// 처리를 진행하지 않게 세팅할 것이다. 
	SetCollisionProfile("Object");

	// m_Mesh = CResourceManager::GetInst()->FindMesh("Box2D");
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
	// 순수 가상함수가 존재하는 추상 클래스 
	return nullptr;
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
