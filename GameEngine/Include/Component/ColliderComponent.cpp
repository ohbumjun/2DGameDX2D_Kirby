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
	// Profile ������ �����ؼ� ����� ���̴�. 
	m_Profile = com.m_Profile;
}

CColliderComponent::~CColliderComponent()
{
	// ������� �Ǹ� , ������ �浹�ߴ� ��� �ֵ����� �� ���� ������� �浹 ��Ͽ��� ������ ! ��� �������ش�.
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
	// �浹ü�鿡 ���ؼ�, ���� ������ �� �ٸ� �浹 �������� �̹� �浹�Ǿ������� �Ǵ��ϴ� �ڵ� 

	// ���� �浹 ��Ͽ� ���ؼ� ó���� ����� �Ѵ�
	// ���� �浹ü���� Section ���� �����ͼ� ���Ѵ�.
	// ���� �����ӿ� �浹�Ǿ��� �浹ü���� �ݺ��Ͽ� ������ ��ġ���� �Ǵ�
	// ���� �����ӿ� �浹�� �Ǿ��µ� , ����� ���� �ٸ� ������ �����Ѵٸ�
	// �ش� �浹ü�ʹ� �浹 ���ɼ��� �����Ƿ�,
	// �浹�Ǿ��ٰ�, �������� ������ �Ǵ����ش�.

	auto iter = m_PrevCollisionList.begin();
	auto iterEnd = m_PrevCollisionList.end();

	// ��� ���� �浹 ��ϵ��� ���鼭
	// �ڽ��� ���� ��� �浹 ������, ���� �浹ü�� ���� ��� �浹 �������� ���Ѵ�
	size_t Size = m_vecSectionIndex.size();
	for (; iter != iterEnd;)
	{
		bool Check = false;

		// ���� �浹 ü�� ��ϵ� ��ο� ���ؼ�, 
		// �ش� �浹ü�� ���� ��� �浹 ������ üũ�Ѵ�
		for (size_t i = 0; i < Size; i++)
		{
			size_t DestSize = (*iter)->m_vecSectionIndex.size();
			for (size_t j = 0; j < DestSize; j++)
			{
				// �ڽ��� ���� �浹ü��, ���� �浹 ��Ͽ� ���� �浹ü�� �浹 ������ ���ٸ�
				if (m_vecSectionIndex[i] == (*iter)->m_vecSectionIndex[j])
				{
					// ��ģ�ٰ� ǥ��
					Check = true;
					break;
				}
			}
			if (Check)
				break;
		}

		// ���� ��ġ�� �浹 ������ �����Ƿ�, ���� �����ӿ��� �پ��ִٰ� ���� �����ӿ����� �������� ���̴� 
		if (!Check)
		{
			// �ݹ��Լ� ȣ�� 
			CallCollisionCallback(Collision_State::End);
			(*iter)->CallCollisionCallback(Collision_State::End);

			// ���� ���� �浹 ��Ͽ��� �������ֱ�
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
	// �� �������� ����Ǹ�, Clear Frame�� ȣ���ؼ�
	// ���Ӱ� �浹�� ������ �� �ְ� ���ش�
	m_vecSectionIndex.empty(); // ���� �浹 ���� clear
	m_CurrentCollisionList.clear(); // ���� �浹�� ��� clear
	m_CurrentSectionCheck = false; // ���� �浹 ��Ͽ����� �浹 ���ε� false
}

void CColliderComponent::Start()
{
	CSceneComponent::Start();
}

bool CColliderComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	// ���� �浹�� ó���ϱ� ���� �浹 ��������
	// 2���� �浹 �Ҷ�, ���� �ϳ��� ���θ� �浹 �������� ��, �浹�� ���ϴ� ������ ó���ߴٸ�
	// ó���� �������� �ʰ� ������ ���̴�. 
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
	// ���� �����Լ��� �����ϴ� �߻� Ŭ���� 
	return nullptr;
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
