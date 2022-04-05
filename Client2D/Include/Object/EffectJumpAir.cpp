#include "EffectJumpAir.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/WidgetComponent.h"
#include "UI/UINumberWidget.h"
#include "UI/UIDamageFont.h"
#include "Monster.h"
#include "Animation/AnimationSequence2DInstance.h"

CEffectJumpAir::CEffectJumpAir() :
	m_MoveDir(1.f),
	m_EffectMoveSpeed(100.f)
{
}

CEffectJumpAir::~CEffectJumpAir()
{
}

void CEffectJumpAir::SetRightEffect()
{
	m_MoveDir = 1.f;

	m_Sprite->GetAnimationInstance()->ChangeAnimation("EffectRight");
}

void CEffectJumpAir::SetLeftEffect()
{
	m_MoveDir = -1.f;

	m_Sprite->GetAnimationInstance()->ChangeAnimation("EffectLeft");
}

void CEffectJumpAir::SetPlayerAttackCollider()
{
	m_ColliderBody->Enable(true);
}

void CEffectJumpAir::PlayerAttackCallback(const CollisionResult& Result)
{
	CColliderComponent* CollisionDest = Result.Dest;

	CGameObject* Owner = CollisionDest->GetGameObject();

	CWidgetComponent* ObjectWindow = nullptr;

	if (Owner)
	{
		CMonster* DestMonster = dynamic_cast<CMonster*>(Owner);

		if (!DestMonster)
			return;

		// 이미 맞은 상태였다면
		if (DestMonster->IsBeingHit())
			return;

		DestMonster->Damage(30.f);

		DestMonster->SetBeingHit(true);

		DestMonster->SetAIState(Monster_AI::Hit);

		if (m_MoveDir < 0)
			DestMonster->SetObjectMoveDir(Vector3(-1.f, 0.f, 0.f));
		else
			DestMonster->SetObjectMoveDir(Vector3(1.f, 0.f, 0.f));

		// Create Damage Font
		ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();

		if (ObjectWindow)
		{
			CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");

			DamageFont->SetDamage(30);
		}
	}
}

bool CEffectJumpAir::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("JumpAirEffect");

	SetRootComponent(m_Sprite);

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"JumpAirEffect", TEXT("Object_Jump.anim"));

	m_Sprite->SetAnimationInstance(AnimationInstance);
	m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->SetPlayTime(0.5f);
	m_Sprite->SetRelativeScale(80.f, 40.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	SetLifeTime(0.7f);

	m_ColliderBody = CreateComponent<CColliderCircle>("Body");
	m_RootComponent->AddChild(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerAttack");
	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectJumpAir::PlayerAttackCallback);
	m_ColliderBody->SetInfo(Vector2(0.f, 0.f), 40.f);
	m_ColliderBody->Enable(false);

	return true;
}

void CEffectJumpAir::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	AddWorldPos(Vector3(m_MoveDir, 0.f, 0.f) * m_EffectMoveSpeed * DeltaTime);
}

void CEffectJumpAir::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
