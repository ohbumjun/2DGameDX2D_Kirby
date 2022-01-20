#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/ColliderCircle.h"
#include "MonsterAnimation.h"
#include "Engine.h"
#include "../UI/SimpleHUD.h"

CMonster::CMonster() :
	m_HPMax(10.f),
	m_HP(10.f)
{
	SetTypeID<CMonster>();
}

CMonster::CMonster(const CMonster& Monster)
{
	m_Sprite = (CSpriteComponent*)const_cast<CMonster&>(Monster).FindComponent("MonsterSprite");
	m_ColliderBody = (CColliderCircle*)const_cast<CMonster&>(Monster).FindComponent("ColliderBody");
}

CMonster::~CMonster()
{}

void CMonster::Damage(float Damage)
{
	// 실제 데미지
	m_HP -= Damage;
	if (m_HP < 0.f)
	{
		Destroy();
		return;
	}

	// Widget HP Bar 조정하기
	CSimpleHUD* HUD = dynamic_cast<CSimpleHUD*>(m_SimpleHUDWidget->GetWidgetWindow());
	HUD->GetProgressBar()->SetPercent(m_HP / m_HPMax);
}

void CMonster::SetHPMax(float HPMax)
{
	m_HPMax = HPMax;
	m_HP = HPMax;
}

bool CMonster::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("MonsterSprite");
	SetRootComponent(m_Sprite);

	// m_ColliderBody = CreateComponent<CColliderBox2D>("ColliderBody");
	m_ColliderBody = CreateComponent<CColliderCircle>("ColliderBody");
	m_ColliderBody->SetCollisionProfile("Monster");
	m_ColliderBody->AddCollisionMouseCallback(Collision_State::Begin, this,
		&CMonster::SetAttackMouse);
	m_ColliderBody->AddCollisionMouseCallback(Collision_State::End, this,
		&CMonster::SetNormalMouse);

	m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");
	m_SimpleHUDWidget->CreateUIWindow<CSimpleHUD>("SimpleHUDWindow");

	// m_HpBar = SimpleHUDWindow->CreateUIWidget<CUIProgressBar>("HPBar");
	// m_HpBar->SetPos(0.f, -50.f);

	m_Sprite->AddChild(m_ColliderBody);
	m_Sprite->SetTransparency(true);
	m_Sprite->CreateAnimationInstance<CMonsterAnimation>();

	m_Sprite->AddChild(m_SimpleHUDWidget);
	m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);

	return true;
}

void CMonster::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CMonster::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::SetAttackMouse(const CollisionResult& Result)
{
	CEngine::GetInst()->SetMouseState(Mouse_State::State1);
}

void CMonster::SetNormalMouse(const CollisionResult& Result)
{
	CEngine::GetInst()->SetMouseState(Mouse_State::Normal);
}

void CMonster::CreateDamageFont(const CollisionResult& Result)
{}
