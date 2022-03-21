#include "Tornado.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include <Component/SpriteComponent.h>
#include "BossTree.h"
#include "Player2D.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"

CTornado::CTornado()
{}

CTornado::CTornado(const CTornado& Attack) : CAttackEffect(Attack)
{}

CTornado::~CTornado()
{}

void CTornado::SetRightAttackDir(float YDir)
{
	CAttackEffect::SetRightAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
}

void CTornado::SetLeftAttackDir(float YDir)
{
	CAttackEffect::SetLeftAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
}


void CTornado::Start()
{
	CAttackEffect::Start();
}

bool CTornado::Init()
{
	if (!CAttackEffect::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"BossTreeTornado", TEXT("Boss_Tree_AttackClose_Tornaido.anim"));

	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->SetWorldScale(300.f, 300.f, 1.f);

	m_Collider->SetCollisionProfile("MonsterAttack");
	m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);
	m_Collider->AddCollisionCallback(Collision_State::Begin, (CAttackEffect*)this, &CAttackEffect::MonsterAttackCollisionCallback);

	return true;
}

void CTornado::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);

	AddWorldPos(Vector3(m_AttackDir.x, m_AttackDir.y, 0.f) * DeltaTime * 200.f);
}

void CTornado::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}

void CTornado::CollisionCallback(const CollisionResult& Result)
{
	Destroy();

	m_MonsterOwner->SetAttackEnd();

	CColliderComponent* CollisionDest = Result.Dest;

	CGameObject* Owner = CollisionDest->GetGameObject();

	CWidgetComponent* ObjectWindow = nullptr;

	if (Owner == m_Scene->GetPlayerObject())
	{
		CPlayer2D* Player = (CPlayer2D*)Owner;

		// HP Bar 달게 하기
		Player->SetIsBeingHit();

		if (m_AttackDir.x > 0)
			Player->SetBeingHitDirection(m_AttackDir.x);
		else
			Player->SetBeingHitDirection(m_AttackDir.x);

		// DestMonster->Damage(2.f);

		// Create Damage Font
		ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();

		if (ObjectWindow)
		{
			CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");
		}
	}
}
