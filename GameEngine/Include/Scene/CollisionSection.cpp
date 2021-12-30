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
	// 자기가 속한 전체 영역의 최소, 최대값을 미리 처리를 해놓는 것  
	m_Min = Min;
	m_Max = Max;

	// 공간 한칸의 크기 + 공간 깊이 
	m_SectionSize = SectionSize;

	// 영역 전체의 크기
	// 보통 전체 섹션이 월드 공간 전체를 커버하기도 하지만
	// 월드 공간 전체가 너무 크면, 전체를 커버하지 않기도 하다
	// 그 전체 공간에서도, 또 공간을 나눠서, 그 공간을 전체 섹션이 덮게 하기도 한다.
	m_SectionTotatlSize = SectionTotalSize;

	// 각 섹션당 우선 100개 정도의 충돌체들이 들어올 것을 예상한다.
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

	// 해당 영역의 충돌체가 2개 이하라면, 충돌시킬 이유가 없다.
	if (Size < 2)
		return;

	for (size_t  i = 0; i < Size - 1; i++)
	{
		CColliderComponent* Src = m_vecCollider[i];
		for (size_t  j = i + 1; j < Size; j++)
		{
			CColliderComponent* Dest = m_vecCollider[j];

			// 현재 프레임 상 , 이미 충돌 처리된 녀석이라면
			if (Src->CheckCurrentFrameCollision(Dest))
				continue;

			Collision_Profile* SrcProfile  = Src->GetCurrentProfile();
			Collision_Profile* DestProfile = Dest->GetCurrentProfile();

			// 둘 중 하나라도 서로 다른 하나를 무시하게 세팅했다면 continue
			if (SrcProfile->vecInteraction[(int)DestProfile->Channel] == Collision_Interaction::Ignore ||
				DestProfile->vecInteraction[(int)SrcProfile->Channel] == Collision_Interaction::Ignore)
				continue;

			if (Src->Collision(Dest))
			{
				// 지금 막 충돌된 것인지 판단한다
				if (!Src->CheckPrevCollision(Dest))
				{
					// 이전 충돌 목록에 존재하지 않았다면, 이번에 처음 충돌하게 된 것이다
					// 서로의 충돌 목록에 추가해준다.
					Src->AddPrevCollision(Dest);
					Dest->AddPrevCollision(Src);

					Src->CallCollisionCallback(Collision_State::Begin);
					Dest->CallCollisionCallback(Collision_State::Begin);
				}
				// 현재 프레임 상에서 이미 충돌 처리를 했다고 표시
				// 이렇게 해두면, 다른 겹치는 충돌 영역에서 충돌을 하더라도, 서로 충돌 처리를 안해주게 된다. 
				Src->AddCurrentFrameCollision(Dest);
				Dest->AddCurrentFrameCollision(Src);
			}
			// 충돌은 안했지만
			// 이전 충돌 목록에 존재하면 : 이번에 떨어진 것
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
			// y sorting을 해준다 --> y 축 좌표 오름 차순 --> 제일 낮은 애부터 높은 놈으로 --> 제일 낮은 y 좌표애가 가장 나중에 그려지게 해야한다.
			qsort(&m_vecCollider[0], (size_t)m_vecCollider.size(), sizeof(CColliderComponent*), SortY);
		}

		// y sort 기준 아래에 있는 놈이 앞에 나온다
		// 아래에 있는 놈부터 충돌 검사를 할 것이다.
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

// 오름 차순
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
