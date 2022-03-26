#include "BombKirbyThrowBomb.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CBombKirbyThrowBomb::CBombKirbyThrowBomb() :
m_IsSetRight(true),
m_RotateComplete(false)
{}

CBombKirbyThrowBomb::~CBombKirbyThrowBomb()
{}

void CBombKirbyThrowBomb::SetLeft()
{
	m_IsSetRight = false;
	m_Bomb->AddRelativeRotationZ(70.f);
	m_Bomb->SetRelativePos(-45.f, 0.f, 0.f);
}

void CBombKirbyThrowBomb::SetRight()
{
	m_IsSetRight = true;
	m_Bomb->AddRelativeRotationZ(-70.f);
	m_Bomb->SetRelativePos(60.f, 0.f, 0.f);
}

void CBombKirbyThrowBomb::Start()
{
	CGameObject::Start();
}

bool CBombKirbyThrowBomb::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Root = CreateComponent<CSceneComponent>("Root");
	m_Bomb = CreateComponent<CSpriteComponent>("BombSprite");

	SetRootComponent(m_Root);

	m_Root->AddChild(m_Bomb);

	// 기본적으로 오른쪽 세팅
	m_Bomb->SetRelativePos(60.f, 0.f, 0.f);
	m_Bomb->AddRelativeRotationZ(-70.f);
	m_Bomb->SetWorldScale(80.f, 80.f, 1.f);
	m_Bomb->SetPivot(0.5f, 0.5f, 0.f);
	m_Bomb->SetInheritRotZ(true);

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"ReadyBomb", TEXT("Ability_Bomb_ThrowBomb.anim"));

	m_Bomb->SetAnimationInstance(AnimationInstance);

	return true;
}

void CBombKirbyThrowBomb::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_RotateComplete)
		return;

	float RotateAmount = -1.f;

	if (m_IsSetRight)
	{
		RotateAmount = DeltaTime * 450.f;

		m_Root->AddRelativeRotationZ(RotateAmount);

		m_RotateAmount += RotateAmount;

		if (m_RotateAmount >= 140.f)
		{
			m_RotateComplete = true;
		}
	}
	else
	{
		RotateAmount = DeltaTime * 450.f * -1.f;

		m_Root->AddRelativeRotationZ(RotateAmount);

		m_RotateAmount += RotateAmount;

		if (m_RotateAmount <= 140.f * -1.f)
		{
			m_RotateComplete = true;
		}
	}

}
