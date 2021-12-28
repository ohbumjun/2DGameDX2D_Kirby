#include "SceneCollision.h"
#include "CollisionSection.h"
#include "../Component/ColliderComponent.h"

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
	// 기본적으로 1) 마우스 vs UI 충돌 --> 2) 마우스 vs GameObject 충돌 처리  

	// 충돌체들을 각자의 영역으로 포함시켜주도록 한다.
	CheckColliderSection();

	m_vecCollider.clear();
}

void CSceneCollision::CollisionMouse(float DelatTime)
{
}

void CSceneCollision::SetSectionSize(const Vector3& Size)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;


	m_Section->SectionSize = Size;
	m_Section->SectionTotalSize = m_Section->SectionSize
	* Vector3((float)m_Section->CountX, (float)m_Section->CountY, (float)m_Section->CountZ);
}

void CSceneCollision::SetSectionSize(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;
	m_Section->SectionSize = Vector3(x, y, z);
	m_Section->SectionTotalSize = m_Section->SectionSize *
		Vector3((float)m_Section->CountX, (float)m_Section->CountY, (float)m_Section->CountZ);
}

void CSceneCollision::SetSectionCenter(const Vector3& Center)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Center = Center;

	// 좌 하단이 min, 우 상단이 max
	m_Section->Min = m_Section->Center - m_Section->SectionTotalSize / 2.f;
	m_Section->Max = m_Section->Center + m_Section->SectionTotalSize / 2.f;
}

void CSceneCollision::SetSectionCenter(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Center = Vector3(x, y, z);

	m_Section->Min = m_Section->Center - m_Section->SectionTotalSize / 2.f;
	m_Section->Max = m_Section->Center + m_Section->SectionTotalSize / 2.f;
}

void CSceneCollision::SetSectionMin(const Vector3& Min)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Min = Min;

	// 즉, 어떤 값을 세탕하던, 전체 크기와 Center를 세팅 
	m_Section->SectionTotalSize = m_Section->Max - m_Section->Min;
	m_Section->SectionSize = m_Section->SectionTotalSize /
		Vector3((float)m_Section->CountX, (float)m_Section->CountY, (float)m_Section->CountZ);
	m_Section->Center = (m_Section->Min + m_Section->Max) / 2.f;
}

void CSceneCollision::SetSectionMin(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Min = Vector3(x, y, z);
	m_Section->SectionTotalSize = m_Section->Max - m_Section->Min;
	m_Section->SectionSize = m_Section->SectionTotalSize /
		Vector3((float)m_Section->CountX, (float)m_Section->CountY, (float)m_Section->CountZ);
	m_Section->Center = (m_Section->Min + m_Section->Max) / 2.f;
}

void CSceneCollision::SetSectionMax(const Vector3& Max)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Max = Max;
	m_Section->SectionTotalSize = m_Section->Max - m_Section->Min;
	m_Section->SectionSize = m_Section->SectionTotalSize /
		Vector3((float)m_Section->CountX, (float)m_Section->CountY, (float)m_Section->CountZ);
	m_Section->Center = (m_Section->Min + m_Section->Max) / 2.f;
}

void CSceneCollision::SetSectionMax(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Max = Vector3(x, y, z);
	m_Section->SectionTotalSize = m_Section->Max - m_Section->Min;
	m_Section->SectionSize = m_Section->SectionTotalSize /
		Vector3((float)m_Section->CountX, (float)m_Section->CountY, (float)m_Section->CountZ);
}

void CSceneCollision::SetSectionCount(int CountX, int CountY, int CountZ)
{
	m_Section->CountX = CountX;
	m_Section->CountY = CountY;
	m_Section->CountZ = CountZ;

	// CountX, Y, Z를 재설정해주면, 관련 변수들도 모두 재설정해줄 것이다. 
	m_Section->SectionTotalSize = m_Section->SectionSize * Vector3((float)CountX, (float)CountY, (float)CountZ);
	m_Section->Min = m_Section->Center - m_Section->SectionTotalSize / 2.f;
	m_Section->Max = m_Section->Center + m_Section->SectionTotalSize / 2.f;
}

// 실제 공간들을 쪼갤 것이다. 
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
				m_Section->vecSection.push_back(Section);
			}
		}
	}
}

void CSceneCollision::Clear()
{
	size_t Size = m_Section->vecSection.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_Section->vecSection[i]);
	}
	SAFE_DELETE(m_Section);
}

void CSceneCollision::AddCollider(CColliderComponent* Collider)
{
	// 각 충돌체들은 매 프레임마다 이동한다
	// 즉, 매 프레임마다 자신이 속한 충돌영역이 달라질 수 있으므로
	// 매 프레임마다 충돌체 들을 비워주고, 새롭게 넣어주는 방식을 활용해야 한다. 
	m_vecCollider.push_back(Collider);
}

// 각 Collider Component가 속한 Section을 확인한다. 
void CSceneCollision::CheckColliderSection()
{
	size_t Size = m_Section->vecSection.size();

	for (size_t i = 0; i < Size; i++)
	{
		CColliderComponent* Collider = m_vecCollider[i];

		Vector3 Min = Collider->GetMin();
		Vector3 Max = Collider->GetMax();

		// 전체 Section의 좌하단을 뺀다 --> 위치한 Section의 Idx를 구하기 위함이다 .
		// 각 Collider 의 Min, Max는 각 Collider 의 센터 중심으로 구해진다 .
		// 전체 Section의 Min, 즉, 좌하단을 빼줘서 0,0 을 기준좌표로 하여 준비한다.
		// 이를 통해, 해당 Section 기준으로 0,0 기준 좌표가 되고
		// 이를 통해, 각 충돌체가 위치한 충돌 영역을 구할 수 있다.
		// 마치 TileMap의 Tile 구하듯이 
		Min -= m_Section->Min;
		Max -= m_Section->Min;

		int IndexMinX, IndexMinY, IndexMinZ;
		int IndexMaxX, IndexMaxY, IndexMaxZ;

		IndexMinX = (int)(Min.x / m_Section->SectionSize.x);
		IndexMinY = (int)(Min.y / m_Section->SectionSize.y);
		IndexMinZ = (int)(Min.z / m_Section->SectionSize.z);

		IndexMaxX = (int)(Max.x / m_Section->SectionSize.x);
		IndexMaxY = (int)(Max.y / m_Section->SectionSize.y);
		IndexMaxZ = (int)(Max.z / m_Section->SectionSize.z);

		// 범위 제한
		IndexMinX = IndexMinX < 0 ? 0 : IndexMinX;
		IndexMinY = IndexMinY < 0 ? 0 : IndexMinY;
		IndexMinZ = IndexMinZ < 0 ? 0 : IndexMinZ;

		IndexMaxX = IndexMaxX >= m_Section->CountX ? m_Section->CountX - 1 : IndexMaxX;
		IndexMaxY = IndexMaxY >= m_Section->CountY ? m_Section->CountY - 1 : IndexMaxY;
		IndexMaxZ = IndexMaxZ >= m_Section->CountZ ? m_Section->CountZ - 1 : IndexMaxZ;

		// 하나의 충돌 Component 가 여러개의 충돌영역에 걸쳐 있을 수 있기 때문에
		// 가능한 충돌 영역 모든 곳에 추가해주는 것이다. 
		for (int z = IndexMinZ; z <= IndexMaxZ; z++)
		{
			for (int y = IndexMinY; y <= IndexMaxY; y++)
			{
				for (int x = IndexMinX; x <= IndexMaxX; x++)
				{
					// 전제 범위
					int Index = z * (m_Section->CountX * m_Section->CountY) + y * m_Section->CountX + x;
					m_Section->vecSection[Index]->AddCollider(Collider);
				}
			}
		}
	}
}