#include "SceneCollision.h"
#include "CollisionSection.h"

CSceneCollision::CSceneCollision() :
m_Section(nullptr)
{
	m_vecCollider.reserve(10000);
}

CSceneCollision::~CSceneCollision()
{
	size_t Size = m_Section->vecSection.size();

	for (size_t i = 0; i < Size; i ++)
	{
		SAFE_DELETE(m_Section->vecSection[i]);
	}
	SAFE_DELETE(m_Section);
}

void CSceneCollision::Start()
{
}

bool CSceneCollision::Init()
{
	SetSectionSize(1000.f, 1000.f, 1.f);
	SetSectionCenter(0.f, 0.f, 0.f);
	SetSectionCount(10, 10, 1);
	CreateSection();
}

void CSceneCollision::Collision(float DeltaTime) //
{
	// 충돌체들을 각자의 영역으로 포함시켜주도록 한다.
	CheckColliderSection();

	m_vecCollider.clear();
}

void CSceneCollision::SetSectionSize(const Vector3& Size)
{
}

void CSceneCollision::SetSectionSize(float x, float y, float z)
{
}

void CSceneCollision::SetSectionCenter(const Vector3& Size)
{
}

void CSceneCollision::SetSectionCenter(float x, float y, float z)
{
}

void CSceneCollision::SetSectionMin(const Vector3& Size)
{
}

void CSceneCollision::SetSectionMin(float x, float y, float z)
{
}

void CSceneCollision::SetSectionMax(const Vector3& Size)
{
}

void CSceneCollision::SetSectionMax(float x, float y, float z)
{
}

void CSceneCollision::SetSectionCount(int CountX, int CountY, int CountZ)
{
}

void CSceneCollision::CreateSection()
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	for (int z = 0; z < m_Section->CountZ; z++)
	{
		for (int y = 0; y < m_Section->CountY; y++)
		{
			for (int x = 0; x < m_Section->CountX; x++)
			{
				CCollisionSection* Section = new CCollisionSection;
				Section->Init(x, y, z, z * (m_Section->CountX * m_Section->CountY) + y * m_Section->CountX + x,
					m_Section->Min, m_Section->Max, m_Section->SectionSize, m_Section->SectionTotalSize);
			}
		}
	}
}

void CSceneCollision::Clear()
{
}

void CSceneCollision::AddCollider(CColliderComponent* Collider)
{
}

void CSceneCollision::CheckColliderSection()
{
}