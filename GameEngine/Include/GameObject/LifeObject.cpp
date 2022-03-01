#include "LifeObject.h"

CLifeObject::CLifeObject():
	m_MoveSpeed(200.f),
	m_PhysicsSimulate(true),
	m_IsGround(true),
	m_FallTime(0.f),
	m_FallStartY(0.f),
	m_Jump(false),
	m_JumpVelocity(0.f),
	m_FallVelocity(0.f),
	m_FallVelocityMax(1000.f),
	m_GravityAccel(10.f),
	m_FloorCheck(false),
	m_SideWallCheck(false),	// Jump
	m_JumpAccel(1.1f),
	m_JumpAccelAccTime(0.f),
	m_PrevPos{}
{}

CLifeObject::CLifeObject(const CLifeObject& obj) : CGameObject(obj)
{
	*this = obj;

	m_PrevPos.y = -1.f;
}

CLifeObject::~CLifeObject()
{}

void CLifeObject::UpdateWhileOffGround(float DeltaTime)
{
	if (!m_IsGround && m_PhysicsSimulate)
	{
		// �������� �ð� ���� 
		m_FallTime += DeltaTime * m_GravityAccel;

		float Velocity = 0.f;

		// ������ ���¶��, �ʱ� ���� �ӵ� * �ð� --> ���� �ö� �Ÿ��� �����ϴ� �� 
		if (m_Jump)
			Velocity = m_JumpVelocity * m_FallTime;

		// ������ �Ÿ�
		m_FallVelocity = 0.5f * GRAVITY * m_FallTime * m_FallTime * m_JumpAccel;

		// �ִ� ���� �ӵ��� �����Ѵ�.
		if (Velocity - m_FallVelocity < m_FallVelocityMax * -1.f)
		{
			m_FallVelocity = Velocity + m_FallVelocityMax;
		}

		float CurrentYPos = m_FallStartY + (Velocity - m_FallVelocity);

		SetWorldPos(GetWorldPos().x, CurrentYPos, GetWorldPos().z);
	}
}

void CLifeObject::Start()
{
	CGameObject::Start();
}

bool CLifeObject::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CLifeObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	UpdateWhileOffGround(DeltaTime);
}

void CLifeObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CLifeObject::PrevRender()
{
	CGameObject::PrevRender();
}

void CLifeObject::Render()
{
	CGameObject::Render();
}

void CLifeObject::PostRender()
{
	CGameObject::PostRender();
}

CLifeObject* CLifeObject::Clone()
{
	return new CLifeObject(*this);
}
