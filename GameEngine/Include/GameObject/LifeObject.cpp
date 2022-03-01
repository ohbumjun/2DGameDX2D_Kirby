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
		// 떨어지는 시간 누적 
		m_FallTime += DeltaTime * m_GravityAccel;

		float Velocity = 0.f;

		// 점프한 상태라면, 초기 점프 속도 * 시간 --> 위로 올라간 거리를 세팅하는 것 
		if (m_Jump)
			Velocity = m_JumpVelocity * m_FallTime;

		// 내려간 거리
		m_FallVelocity = 0.5f * GRAVITY * m_FallTime * m_FallTime * m_JumpAccel;

		// 최대 낙하 속도를 조절한다.
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
