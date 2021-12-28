#include "CollisionSection.h"

CCollisionSection::CCollisionSection()
{
}

CCollisionSection::~CCollisionSection()
{
}

void CCollisionSection::Init(int IndexX, int IndexY, int IndexZ, int Index, const Vector3& Min,
	const Vector3& Max, const Vector3& SectionSize, const Vector3& SectionTotalSize)
{
	m_IndexX = IndexX;
	m_IndexY = IndexY;
	m_IndexZ = IndexZ;

	m_Index = Index;
	m_Min = Min;
	m_Max = Max;

	m_SectionSize = SectionSize;
	m_SectionTotatlSize = SectionTotalSize;

	// �� ���Ǵ� �켱 100�� ������ �浹ü���� ���� ���� �����Ѵ�.
	m_vecCollider.reserve(100);
}

void CCollisionSection::Clear()
{
}

void CCollisionSection::AddCollider(CColliderComponent* Collider)
{
	m_vecCollider.push_back(Collider);
}

void CCollisionSection::Collision(float DeltaTime)
{
}