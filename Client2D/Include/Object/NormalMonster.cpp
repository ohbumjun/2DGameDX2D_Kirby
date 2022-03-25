#include "NormalMonster.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Player2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderComponent.h"
#include "UI/UIDamageFont.h"

CNormalMonster::CNormalMonster()
{
	// SetTypeID<CNormalMonster>();

	m_MonsterType = Monster_Type::Normal;

	m_AttackAbility = 20.f;
	m_HPMax = 100.f;
	m_HP = 100.f;
	m_AttackDistance = 30.f;
}

CNormalMonster::CNormalMonster(const CNormalMonster& Monster) : CMonster(Monster)
{}

CNormalMonster::~CNormalMonster()
{}

void CNormalMonster::Start()
{
	CMonster::Start();

	m_MonsterType = Monster_Type::Normal;

	m_AttackAbility = 20.f;
	m_HPMax = 100.f;
	m_HP = 100.f;

	// Death Animation 이후 바로 Destroy 세팅한다
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightDeath")->SetPlayTime(1.5f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightDeath")->SetEndFunction((CRef*)this, &CRef::Destroy);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftDeath")->SetPlayTime(1.5f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftDeath")->SetEndFunction((CRef*)this, &CRef::Destroy);

	m_ColliderBody->AddCollisionCallback(Collision_State::Begin,this, &CNormalMonster::OnNormalMonsterBodyCollisionBegin);
}

bool CNormalMonster::Init()
{
	if (!CMonster::Init())
		return false;

	return true;
}

void CNormalMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CNormalMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CNormalMonster* CNormalMonster::Clone()
{
	return new CNormalMonster(*this);
}

// 부딪히는 순간 Player에게 Damage를 준다.
// 단, Player가 위에서 떨어지는 순간은 X 
void CNormalMonster::OnNormalMonsterBodyCollisionBegin(const CollisionResult& Result)
{
	CGameObject* Owner = Result.Dest->GetGameObject();

	CWidgetComponent* ObjectWindow = nullptr;

	CPlayer2D* Player = dynamic_cast<CPlayer2D*>(Result.Dest->GetGameObject());

	if (!Player)
		return;

	// Player가 위에서 떨어지는 경우는 X --> Player Fall Attack 만을 적용해야 하기 때문이다.
	if (GetWorldPos().y + m_ColliderBody->GetInfo().Radius < Player->GetWorldPos().y)
		return;

	// Player 가 Slide Attack 중에는 X
	if (Player->IsSlideAttacking())
		return;

	// Scene Change 가 일어나고 있다면
	if (Player->IsSceneChanging())
		return;

	// 이미 맞아서 데미지 받아 이동중이라면
	if (Player->IsBeingHit())
		return;

	Player->Damage(m_AttackAbility);

	Player->SetIsBeingHit();

	if (m_ObjectMoveDir.x > 0)
		Player->SetBeingHitDirection(m_ObjectMoveDir.x);
	else
		Player->SetBeingHitDirection(m_ObjectMoveDir.x);

	// DestMonster->Damage(2.f);

	// Create Damage Font
	ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();

	if (ObjectWindow)
	{
		CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");

		DamageFont->SetDamage((int)m_AttackAbility);
	}
}

void CNormalMonster::Save(FILE* pFile)
{
	CMonster::Save(pFile);
}

void CNormalMonster::Load(FILE* pFile)
{
	CMonster::Load(pFile);
}