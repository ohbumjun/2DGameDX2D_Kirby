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
	// �⺻������ 1) ���콺 vs UI �浹 --> 2) ���콺 vs GameObject �浹 ó��  

	// �浹ü���� ������ �������� ���Խ����ֵ��� �Ѵ�.
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

	// �� �ϴ��� min, �� ����� max
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

	// ��, � ���� �����ϴ�, ��ü ũ��� Center�� ���� 
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

	// CountX, Y, Z�� �缳�����ָ�, ���� �����鵵 ��� �缳������ ���̴�. 
	m_Section->SectionTotalSize = m_Section->SectionSize * Vector3((float)CountX, (float)CountY, (float)CountZ);
	m_Section->Min = m_Section->Center - m_Section->SectionTotalSize / 2.f;
	m_Section->Max = m_Section->Center + m_Section->SectionTotalSize / 2.f;
}

// ���� �������� �ɰ� ���̴�. 
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
	// �� �浹ü���� �� �����Ӹ��� �̵��Ѵ�
	// ��, �� �����Ӹ��� �ڽ��� ���� �浹������ �޶��� �� �����Ƿ�
	// �� �����Ӹ��� �浹ü ���� ����ְ�, ���Ӱ� �־��ִ� ����� Ȱ���ؾ� �Ѵ�. 
	m_vecCollider.push_back(Collider);
}

// �� Collider Component�� ���� Section�� Ȯ���Ѵ�. 
void CSceneCollision::CheckColliderSection()
{
	size_t Size = m_Section->vecSection.size();

	for (size_t i = 0; i < Size; i++)
	{
		CColliderComponent* Collider = m_vecCollider[i];

		Vector3 Min = Collider->GetMin();
		Vector3 Max = Collider->GetMax();

		// ��ü Section�� ���ϴ��� ���� --> ��ġ�� Section�� Idx�� ���ϱ� �����̴� .
		// �� Collider �� Min, Max�� �� Collider �� ���� �߽����� �������� .
		// ��ü Section�� Min, ��, ���ϴ��� ���༭ 0,0 �� ������ǥ�� �Ͽ� �غ��Ѵ�.
		// �̸� ����, �ش� Section �������� 0,0 ���� ��ǥ�� �ǰ�
		// �̸� ����, �� �浹ü�� ��ġ�� �浹 ������ ���� �� �ִ�.
		// ��ġ TileMap�� Tile ���ϵ��� 
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

		// ���� ����
		IndexMinX = IndexMinX < 0 ? 0 : IndexMinX;
		IndexMinY = IndexMinY < 0 ? 0 : IndexMinY;
		IndexMinZ = IndexMinZ < 0 ? 0 : IndexMinZ;

		IndexMaxX = IndexMaxX >= m_Section->CountX ? m_Section->CountX - 1 : IndexMaxX;
		IndexMaxY = IndexMaxY >= m_Section->CountY ? m_Section->CountY - 1 : IndexMaxY;
		IndexMaxZ = IndexMaxZ >= m_Section->CountZ ? m_Section->CountZ - 1 : IndexMaxZ;

		// �ϳ��� �浹 Component �� �������� �浹������ ���� ���� �� �ֱ� ������
		// ������ �浹 ���� ��� ���� �߰����ִ� ���̴�. 
		for (int z = IndexMinZ; z <= IndexMaxZ; z++)
		{
			for (int y = IndexMinY; y <= IndexMaxY; y++)
			{
				for (int x = IndexMinX; x <= IndexMaxX; x++)
				{
					// ���� ����
					int Index = z * (m_Section->CountX * m_Section->CountY) + y * m_Section->CountX + x;
					m_Section->vecSection[Index]->AddCollider(Collider);
				}
			}
		}
	}
}