#include "FightMonsterAttack.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include <Component/SpriteComponent.h>
#include "BeamMonster.h"
#include "Player2D.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"

CFightMonsterAttack::CFightMonsterAttack() :
	m_AttackDir(1.f),
	m_AttackDistLimit(0.f),
	m_AttackDistLimitMax(1000.f)
{}

CFightMonsterAttack::CFightMonsterAttack(const CFightMonsterAttack& Attack) : CGameObject(Attack)
{}

CFightMonsterAttack::~CFightMonsterAttack()
{}

void CFightMonsterAttack::SetRightAttackDir()
{
	m_AttackDir = 1.f;
}

void CFightMonsterAttack::SetLeftAttackDir()
{
	m_AttackDir = -1.f;
}

void CFightMonsterAttack::Start()
{
	CGameObject::Start();
}

bool CFightMonsterAttack::Init()
{
	if (!CGameObject::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"FightAttackEffect", TEXT("Ability_Fight_AttackEffect.anim"));

	float AnimDelayTime = AnimationInstance->GetCurrentAnimation()->GetPlayTime()
		/ AnimationInstance->GetCurrentAnimation()->GetFrameCount();

	m_Sprite = CreateComponent<CSpriteComponent>("MainSprite");
	m_Sprite->SetAnimationInstance(AnimationInstance);
	m_Sprite->SetWorldScale(200.f, 200.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	CColliderCircle* ColliderCirle = CreateComponent<CColliderCircle>("FirstCollider");
	m_Sprite->AddChild(ColliderCirle);
	ColliderCirle->SetCollisionProfile("MonsterAttack");
	ColliderCirle->SetInfo(Vector2(0.f, 0.f), m_Sprite->GetWorldScale().x * 0.3f);
	ColliderCirle->AddCollisionCallback(Collision_State::Begin, this, &CFightMonsterAttack::CollisionCallback);

	return true;
}

void CFightMonsterAttack::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	float MoveDist = m_AttackDir * DeltaTime * 500.f;

	AddWorldPos(Vector3(m_AttackDir, 0.f, 0.f) * DeltaTime * 500.f);

	if (m_AttackDistLimit < m_AttackDistLimitMax)
	{
		m_AttackDistLimit += MoveDist;

		if (m_AttackDistLimit >= m_AttackDistLimitMax)
		{
			Destroy();
		}
	}
}

void CFightMonsterAttack::CollisionCallback(const CollisionResult& Result)
{
	CColliderComponent* CollisionDest = Result.Dest;

	CGameObject* Owner = CollisionDest->GetGameObject();

	CWidgetComponent* ObjectWindow = nullptr;

	if (Owner == m_Scene->GetPlayerObject())
	{
		CPlayer2D* Player = (CPlayer2D*)Owner;

		// HP Bar �ް� �ϱ�
		Player->SetIsBeingHit();

		if (m_AttackDir > 0)
			Player->SetBeingHitDirection(m_AttackDir);
		else
			Player->SetBeingHitDirection(m_AttackDir);

		// DestMonster->Damage(2.f);

		// Create Damage Font
		ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();

		if (ObjectWindow)
		{
			CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");
		}
	}
}
