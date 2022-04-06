#include "PlayerClone.h"

CPlayerClone::CPlayerClone() :
	m_OpacityDecreaseMaxTime(0.f)
{}

CPlayerClone::~CPlayerClone()
{}

void CPlayerClone::SetKirbyState(Ability_State State)
{
	switch (State)
	{
	case Ability_State::Beam:
	{
		m_KirbyState = CreateComponent<CBeamKirbyState>("BeamKirbyState");
	}
	break;
	case Ability_State::Fire:
	{
		m_KirbyState = CreateComponent<CFireKirbyState>("FireKirbyState");
	}
	break;
	case Ability_State::Fight:
	{
		m_KirbyState = CreateComponent<CFightKirbyState>("FightKirbyState");
	}
	break;
	case Ability_State::Bomb:
	{
		m_KirbyState = CreateComponent<CBombKirbyState>("BombKirbyState");
	}
	break;
	case Ability_State::Sword:
	{
		m_KirbyState = CreateComponent<CSwordKirbyState>("SwordKirbyState");
	}
	break;
	}

	m_KirbyState->SetLayerName("Tile");

	m_KirbyState->SetTransparency(true);
	m_KirbyState->SetRelativeScale(80.f, 80.f, 1.f);
	m_KirbyState->SetRelativePos(100.f, 50.f, 0.f);
	m_KirbyState->SetPivot(0.5f, 0.5f, 0.f);

	SetRootComponent(m_KirbyState);
}

void CPlayerClone::Start()
{
	CGameObject::Start();
}

bool CPlayerClone::Init()
{
	if (!CGameObject::Init())
		return false;

	m_KirbyState = CreateComponent<CNormalKirbyState>("PlayerSprite");
	// m_KirbyState = CreateComponent<CBombKirbyState>("BombKirbyState");

	// Player ��ü�� ���ؼ��� ���ʿ� �׷��� �ϱ� �����̴�.
	m_KirbyState->SetLayerName("Tile");

	m_KirbyState->SetTransparency(true);
	m_KirbyState->SetRelativeScale(80.f, 80.f, 1.f);
	m_KirbyState->SetRelativePos(100.f, 50.f, 0.f);
	m_KirbyState->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CPlayerClone::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// ����
	/*
	if (m_ObjectMoveDir < 0.f)
	{
		AddWorldPos(Vector3(1.f * -1.f, 0.f, 0.f) * DeltaTime * m_ObjectMoveSpeed);
	}
	// ������
	else
	{
		AddWorldPos(Vector3(1.f, 0.f, 0.f) * DeltaTime * m_ObjectMoveSpeed);
	}
	*/

	// Opacity ����
	m_OpacityDecreaseTime += DeltaTime;

	m_KirbyState->GetMaterial()->SetOpacity(1.f - m_OpacityDecreaseTime / m_OpacityDecreaseMaxTime);

	if (m_OpacityDecreaseTime >= m_OpacityDecreaseMaxTime)
	{
		Destroy();
	}
}
