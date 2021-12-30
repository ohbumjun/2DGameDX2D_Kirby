#include "CollisionSection.h"
#include "../Input.h"
#include "../Component/ColliderComponent.h"


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
	// �ڱⰡ ���� ��ü ������ �ּ�, �ִ밪�� �̸� ó���� �س��� ��  
	m_Min = Min;
	m_Max = Max;

	// ���� ��ĭ�� ũ�� + ���� ���� 
	m_SectionSize = SectionSize;

	// ���� ��ü�� ũ��
	// ���� ��ü ������ ���� ���� ��ü�� Ŀ���ϱ⵵ ������
	// ���� ���� ��ü�� �ʹ� ũ��, ��ü�� Ŀ������ �ʱ⵵ �ϴ�
	// �� ��ü ����������, �� ������ ������, �� ������ ��ü ������ ���� �ϱ⵵ �Ѵ�.
	m_SectionTotatlSize = SectionTotalSize;

	// �� ���Ǵ� �켱 100�� ������ �浹ü���� ���� ���� �����Ѵ�.
	m_vecCollider.reserve(100);
}

void CCollisionSection::Clear()
{
	m_vecCollider.clear();
}

void CCollisionSection::AddCollider(CColliderComponent* Collider)
{
	m_vecCollider.push_back(Collider);
	Collider->AddSectionIndex(m_Index);
}

void CCollisionSection::Collision(float DeltaTime)
{
	size_t Size = m_vecCollider.size();

	// �ش� ������ �浹ü�� 2�� ���϶��, �浹��ų ������ ����.
	if (Size < 2)
		return;

	for (size_t  i = 0; i < Size - 1; i++)
	{
		CColliderComponent* Src = m_vecCollider[i];
		for (size_t  j = i + 1; j < Size; j++)
		{
			CColliderComponent* Dest = m_vecCollider[j];

			// ���� ������ �� , �̹� �浹 ó���� �༮�̶��
			if (Src->CheckCurrentFrameCollision(Dest))
				continue;

			Collision_Profile* SrcProfile  = Src->GetCurrentProfile();
			Collision_Profile* DestProfile = Dest->GetCurrentProfile();

			// �� �� �ϳ��� ���� �ٸ� �ϳ��� �����ϰ� �����ߴٸ� continue
			if (SrcProfile->vecInteraction[(int)DestProfile->Channel] == Collision_Interaction::Ignore ||
				DestProfile->vecInteraction[(int)SrcProfile->Channel] == Collision_Interaction::Ignore)
				continue;

			if (Src->Collision(Dest))
			{
				// ���� �� �浹�� ������ �Ǵ��Ѵ�
				if (!Src->CheckPrevCollision(Dest))
				{
					// ���� �浹 ��Ͽ� �������� �ʾҴٸ�, �̹��� ó�� �浹�ϰ� �� ���̴�
					// ������ �浹 ��Ͽ� �߰����ش�.
					Src->AddPrevCollision(Dest);
					Dest->AddPrevCollision(Src);

					Src->CallCollisionCallback(Collision_State::Begin);
					Dest->CallCollisionCallback(Collision_State::Begin);
				}
				// ���� ������ �󿡼� �̹� �浹 ó���� �ߴٰ� ǥ��
				// �̷��� �صθ�, �ٸ� ��ġ�� �浹 �������� �浹�� �ϴ���, ���� �浹 ó���� �����ְ� �ȴ�. 
				Src->AddCurrentFrameCollision(Dest);
				Dest->AddCurrentFrameCollision(Src);
			}
			// �浹�� ��������
			// ���� �浹 ��Ͽ� �����ϸ� : �̹��� ������ ��
			else if (Src->CheckPrevCollision(Dest))
			{
				Src->DeletePrevCollision(Dest);
				Dest->DeletePrevCollision(Src);

				Src->CallCollisionCallback(Collision_State::End);
				Dest->CallCollisionCallback(Collision_State::End);
			}
		}
	}
}

CColliderComponent* CCollisionSection::CollisionMouse(bool Is2D, float DeltaTime)
{
	if (Is2D)
	{
		Vector2 MousePos = CInput::GetInst()->GetMouseWorld2DPos();

		size_t Size = m_vecCollider.size();

		if (Size > 1)
		{
			// y sorting�� ���ش� --> y �� ��ǥ ���� ���� --> ���� ���� �ֺ��� ���� ������ --> ���� ���� y ��ǥ�ְ� ���� ���߿� �׷����� �ؾ��Ѵ�.
			qsort(&m_vecCollider[0], (size_t)m_vecCollider.size(), sizeof(CColliderComponent*), SortY);
		}

		// y sort ���� �Ʒ��� �ִ� ���� �տ� ���´�
		// �Ʒ��� �ִ� ����� �浹 �˻縦 �� ���̴�.
		for (size_t i = 0; i < Size; i++)
		{
			if (m_vecCollider[i]->CollisionMouse(MousePos))
				return m_vecCollider[i];
		}
	}
	else
	{
		
	}
	return nullptr;
}

// ���� ����
int CCollisionSection::SortY(const void* Src, const void* Dest)
{
	CColliderComponent* SrcComponent = *(CColliderComponent**)Src;
	CColliderComponent* DestComponent = *(CColliderComponent**)Dest;

	if (SrcComponent->GetMin().y < DestComponent->GetMin().y)
		return 1;
	else if (SrcComponent->GetMin().y > DestComponent->GetMin().y)
		return -1;

	return 0;
}
