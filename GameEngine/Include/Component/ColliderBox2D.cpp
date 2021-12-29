#include "ColliderBox2D.h"
#include "../Collision/Collision.h"

CColliderBox2D::CColliderBox2D()
{
	SetTypeID<CColliderBox2D>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;

	m_ColliderType = Collider_Type::Box_2D;
}

CColliderBox2D::CColliderBox2D(const CColliderBox2D& Collider) : CColliderComponent(Collider)
{
	m_Info = Collider.m_Info;
}

CColliderBox2D::~CColliderBox2D()
{
}

void CColliderBox2D::Start()
{
	CColliderComponent::Start();
}

bool CColliderBox2D::Init()
{
	if (!CColliderComponent::Init())
		return false;

	m_Info.Axis[0] = Vector2(1.f, 0.f);
	m_Info.Axis[1] = Vector2(0.f, 1.f);
	m_Info.Length = Vector2(50.f, 50.f);

	// �θ��� Z ȸ���� ������ �ް� �����Ѵ�. 
	SetInheritRotZ(true);
	SetWorldScale(m_Info.Length.x * 2.f, m_Info.Length.y * 2.f, 1.f);

	return true;
}

void CColliderBox2D::Update(float DeltaTime)
{
	CColliderComponent::Update(DeltaTime);
}

void CColliderBox2D::PostUpdate(float DeltaTime)
{
	CColliderComponent::PostUpdate(DeltaTime);

	// Center�� WorldPos �� ������ ����� ��ġ�� ���� ���̴�. �̸� ���� Offset ������ �������� ���̴�. 
	m_Info.Center.x = GetWorldPos().x + m_Offset.x;
	m_Info.Center.y = GetWorldPos().y + m_Offset.y;

	m_Info.Axis[0].x = GetWorldAxis(AXIS_X).x;
	m_Info.Axis[0].y = GetWorldAxis(AXIS_X).y;

	m_Info.Axis[1].x = GetWorldAxis(AXIS_Y).x;
	m_Info.Axis[1].y = GetWorldAxis(AXIS_Y).y;

	Vector2 Pos[4] = {};

	// 4���� �������� �� �����ϰ� ����
	// �� �߿��� min�� ���� ���̴�.
	// ���� �� 
	Pos[0] = m_Info.Center - m_Info.Axis[0] * m_Info.Length.x + m_Info.Axis[1] * m_Info.Length.y;
	// ������ �� 
	Pos[1] = m_Info.Center + m_Info.Axis[0] * m_Info.Length.x + m_Info.Axis[1] * m_Info.Length.y;
	// ���� �Ʒ� 
	Pos[2] = m_Info.Center - m_Info.Axis[0] * m_Info.Length.x - m_Info.Axis[1] * m_Info.Length.y;
	// ������ �Ʒ�
	Pos[3] = m_Info.Center + m_Info.Axis[0] * m_Info.Length.x - m_Info.Axis[1] * m_Info.Length.y;

	m_Min.x = Pos[0].x;
	m_Min.y = Pos[0].y;

	m_Max.x = Pos[0].x;
	m_Max.y = Pos[0].y;

	for (int i = 1; i < 4; i++)
	{
		if (Pos[i].x < m_Min.x)
			m_Min.x = Pos[i].x;
		if (Pos[i].x > m_Max.x)
			m_Max.x = Pos[i].x;

		if (Pos[i].y < m_Min.y)
			m_Min.y = Pos[i].y;
		if (Pos[i].y > m_Max.y)
			m_Max.y = Pos[i].y;
	}

	m_Info.Min.x = m_Min.x;
	m_Info.Min.y = m_Min.y;

	m_Info.Max.x = m_Max.x;
	m_Info.Max.y = m_Max.y;

}

void CColliderBox2D::PrevRender()
{
	CColliderComponent::PrevRender();
}

void CColliderBox2D::Render(HDC hDC)
{
	CColliderComponent::Render(hDC);
}

void CColliderBox2D::PostRender()
{
	CColliderComponent::PostRender();
}

CColliderComponent* CColliderBox2D::Clone()
{
	return new CColliderBox2D(*this);
}

void CColliderBox2D::CheckCollision()
{
	CColliderComponent::CheckCollision();
}

void CColliderBox2D::Save(FILE* pFile)
{
	CColliderComponent::Save(pFile);
	fwrite(&m_Info, sizeof(Box2DInfo), 1, pFile);
}

void CColliderBox2D::Load(FILE* pFile)
{
	CColliderComponent::Load(pFile);
	fread(&m_Info, sizeof(Box2DInfo), 1, pFile);
}

bool CColliderBox2D::Collision(CColliderComponent* Dest)
{
	return false;
}

bool CColliderBox2D::CollisionMouse(const Vector2& MousePos)
{
	CollisionResult Result;
	m_MouseCollision =  CCollision::CollisionBox2DToPoint(m_MouseResult, Result, m_Info, MousePos))
	return m_MouseCollision;
}