#include "EffectStar.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/CameraManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/ColliderCircle.h"
#include "EffectKirbyRide.h"
#include "Engine.h"
#include "BulletCamera.h"
#include "Player2D.h"
#include "Monster.h"
#include "UI/UIDamageFont.h"
#include "Component/WidgetComponent.h"

CEffectStar::CEffectStar() :
	m_IsRotate(false),
	m_IsBeingPulled(false),
	m_IsSpecialKirbyStar(false),
	m_IsPulledAgainStar(false),
	m_SpecialStarLightToggle(false),
	m_SpecialStarLightTime(0.f),
	m_SpecialStarLightTimeMax(0.1f),
	m_BeginPulledAccel(2.2f),
	m_BeginPulledAccelSum(0.f),
	m_ColliderProfileChangeTimeMax(1.f)
{
	SetTypeID<CEffectStar>();

	m_AbilityState = Ability_State::End;
}

CEffectStar::CEffectStar(const CEffectStar& Beatle) : CAttackEffect(Beatle)
{}

CEffectStar::~CEffectStar()
{}

void CEffectStar::BottomCollisionSpecificAction()
{
	if (m_IsSpecialKirbyStar)
	{
		m_JumpVelocity = m_JumpVelocity * 0.95f;
		m_Jump = true;
		m_FallTime = 0.f;
		m_FallStartY = GetWorldPos().y;
	}
}

void CEffectStar::SideCollisionSpecificAction()
{
	if (m_IsSpecialKirbyStar)
	{
		m_JumpVelocity = m_JumpVelocity * 0.95f;
		m_Jump = true;
		m_FallTime = 0.f;
		m_AttackDir.x = -1.f * m_AttackDir.x;
		m_FallStartY = GetWorldPos().y;

		// m_FallStartY = GetWorldPos().y + GetWorldScale().y;
	}
}

void CEffectStar::UpdateBeingPulled(float DeltaTime)
{
	if (m_IsPulledAgainStar)
	{
		if (m_ColliderProfileChangeTime < m_ColliderProfileChangeTimeMax)
		{
			m_ColliderProfileChangeTime += DeltaTime;
		}
		else
		{
			// Profile을 Monster로 바꿔서, Player에 의해 Pull 될 수 있게 한다.
			m_Collider->SetCollisionProfile("Monster");
			m_IsPulledAgainStar = false;
		}
	}

	if (!m_IsBeingPulled)
		return;

	Vector3 PulledDir = m_Scene->GetPlayerObject()->GetWorldPos() - GetWorldPos();

	PulledDir.Normalize();

	m_BeginPulledAccelSum += m_BeginPulledAccel;

	AddWorldPos(Vector3(PulledDir) * DeltaTime * m_BeginPulledAccelSum);

	if (GetWorldPos().Distance(m_Scene->GetPlayerObject()->GetWorldPos()) <= 5.f)
	{
		m_IsBeingPulled = false;

		CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();

		Player->SetIsEatingMonster(true);
		Player->SetEatenMonsterAsPrevEatenMonster();
	}
}

void CEffectStar::UpdateLightToggle(float DeltaTime)
{
	if (m_IsSpecialKirbyStar)
	{
		if (m_SpecialStarLightTime < m_SpecialStarLightTimeMax)
		{
			m_SpecialStarLightTime += DeltaTime;
		}
		else
		{
			if (m_SpecialStarLightToggle)
			{
				m_MainSprite->SetOpacity(1.f);
				m_SpecialStarLightToggle = false;
			}
			else
			{
				m_MainSprite->SetOpacity(0.8f);
				m_SpecialStarLightToggle = true;
			}
			m_SpecialStarLightTime -= m_SpecialStarLightTimeMax;
		}
	}
}

void CEffectStar::Start()
{
	CAttackEffect::Start();

	m_Collider->SetCollisionProfile("PlayerAttack");
}

bool CEffectStar::Init()
{
	if (!CAttackEffect::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"CSceneChangeStarEffect", TEXT("Effect_SceneChangeStar.anim"));

	// Clone 해서 세팅해줘야 한다.
	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");

	// SetWorldScale(GetWorldScale().x * 0.8f, GetWorldScale().y * 0.8f, 1.f);
	
	m_Collider->SetCollisionProfile("PlayerAttack");
	m_Collider->SetInfo(Vector2(0.f, 0.f), GetWorldScale().x * 0.4f);
	m_Collider->AddCollisionCallback(Collision_State::Begin, this, &CEffectStar::StarCollision);

	m_BottomCollisionApplied = true;

	return true;
}

void CEffectStar::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);

	if (m_IsRotate)
	{
		AddRelativeRotationZ(360.f * DeltaTime);
	}

	UpdateBeingPulled(DeltaTime);

	UpdateLightToggle(DeltaTime);

	if (m_IsBeingPulled)
		return;

	// 일반 Monster를 내뱉은 Star 라면
	if (!m_IsSpecialKirbyStar)
	{
		AddWorldPos(Vector3(m_AttackDir.x, m_AttackDir.y, 0.f) * DeltaTime * m_EffectMoveSpeed);
	}
	// 그게 아니라, Ability Monster 를 내뱉은 Star 라면
	else
	{
		AddWorldPos(Vector3(m_AttackDir.x, 0.f, 0.f) * DeltaTime * m_EffectMoveSpeed * 0.2f);
	}

}

void CEffectStar::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}

CEffectStar* CEffectStar::Clone()
{
	return new CEffectStar(*this);
}

void CEffectStar::StarCollision(const CollisionResult& Result)
{
	if (Result.Dest->GetGameObject() == m_Scene->GetPlayerObject())
	{
		// 당겨지지 않고 있는 상태라면 X
		if (!m_IsBeingPulled)
			return;

		CPlayer2D* Player2D = dynamic_cast<CPlayer2D*>(Result.Dest->GetGameObject());

		// Player의 몸통 충돌체가 아니라면
		if ((CColliderBox2D*)Result.Dest != Player2D->GetBodyCollider())
			return;

		m_IsBeingPulled = false;

		Enable(false);

		// Destroy();

		Player2D->SetIsEatingMonster(true);
		Player2D->SetEatenMonsterAsPrevEatenMonster();
	}
	else
	{
		Destroy();

		CColliderComponent* CollisionDest = Result.Dest;

		CGameObject* Owner = CollisionDest->GetGameObject();

		CLifeObject* AttackSourceObject = (CLifeObject*)Result.Src->GetGameObject();

		CWidgetComponent* ObjectWindow = nullptr;

		if (Owner)
		{
			CMonster* DestMonster = dynamic_cast<CMonster*>(Owner);

			if (!DestMonster)
				return;

			// HP Bar 달게 하기
			DestMonster->Damage(AttackSourceObject->GetAttackAbility());

			DestMonster->SetBeingHit(true);

			DestMonster->SetAIState(Monster_AI::Hit);

			if (m_AttackDir.x > 0)
				DestMonster->SetObjectMoveDir(Vector3(-1.f, 0.f, 0.f));
			else
				DestMonster->SetObjectMoveDir(Vector3(1.f, 0.f, 0.f));

			// DestMonster->Damage(2.f);

			// Create Damage Font
			ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();

			if (ObjectWindow)
			{
				CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");
				DamageFont->SetDamage((int)AttackSourceObject->GetAttackAbility());
			}
		}
	}
}
