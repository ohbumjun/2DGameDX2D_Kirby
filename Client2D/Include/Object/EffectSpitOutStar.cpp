#include "EffectSpitOutStar.h"
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
#include "Monster.h"
#include "UI/UIDamageFont.h"
#include "Component/WidgetComponent.h"

CEffectSpitOutStar::CEffectSpitOutStar() :
	m_AliveTime(3.f)
{
	SetTypeID<CEffectSpitOutStar>();
}

CEffectSpitOutStar::CEffectSpitOutStar(const CEffectSpitOutStar& Beatle) : CGameObject(Beatle)
{}

CEffectSpitOutStar::~CEffectSpitOutStar()
{}

void CEffectSpitOutStar::Start()
{
	CGameObject::Start();

	m_Sprite = (CSpriteComponent*)FindComponent("EffectSprite");
	// m_Sprite = (CSpriteComponent*)m_RootComponent.Get();
	// m_ColliderBody = (CColliderCircle*)FindComponent("EffectChangeStarColliderBody");
	m_ColliderBody = (CColliderCircle*)m_RootComponent->FindComponent("EffectChangeStarColliderBody");
	m_ColliderBody->SetCollisionProfile("PlayerAttack");
}

bool CEffectSpitOutStar::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("EffectSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRenderState("AlphaBlend");

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"CSceneChangeStarEffect", TEXT("Effect_SceneChangeStar.anim"));


	if (!AnimationInstance)
	{
		assert(false);
		return false;
	}

	// Clone 해서 세팅해줘야 한다.
	m_Sprite->SetAnimationInstance(AnimationInstance);

	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");

	SetWorldScale(GetWorldScale().x * 0.8f, GetWorldScale().y * 0.8f, 1.f);

	m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);

	// Collider
	m_ColliderBody = CreateComponent<CColliderCircle>("EffectChangeStarColliderBody");

	Vector2 ColliderCenter = Vector2(
		m_Sprite->GetWorldPos().x + m_Sprite->GetWorldScale().x * m_Sprite->GetPivot().x,
		m_Sprite->GetWorldPos().y + m_Sprite->GetWorldScale().y * m_Sprite->GetPivot().y
	);

	m_ColliderBody->SetInfo(ColliderCenter, m_Sprite->GetWorldScale().x * 0.4f);
	m_ColliderBody->SetCollisionProfile("PlayerAttack");
	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectSpitOutStar::StarCollision);

	m_Sprite->AddChild(m_ColliderBody);

	return true;
}

void CEffectSpitOutStar::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	AddWorldPos(Vector3(m_SpitOutDir, 0.f, 0.f) * DeltaTime * 300.f);

	if (m_AliveTime > 0.f)
	{
		m_AliveTime -= DeltaTime;

		if (m_AliveTime < 0)
		{
			Destroy();
		}
	}
}

void CEffectSpitOutStar::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CEffectSpitOutStar* CEffectSpitOutStar::Clone()
{
	return new CEffectSpitOutStar(*this);
}

void CEffectSpitOutStar::StarCollision(const CollisionResult& Result)
{
	Destroy();

	CColliderComponent* CollisionDest = Result.Dest;

	CGameObject* Owner = CollisionDest->GetGameObject();

	CWidgetComponent* ObjectWindow = nullptr;

	if (Owner)
	{
		CMonster* DestMonster = dynamic_cast<CMonster*>(Owner);

		if (!DestMonster)
			return;

		// HP Bar 달게 하기
		DestMonster->SetBeingHit(true);

		DestMonster->SetAIState(Monster_AI::Hit);

		if (m_SpitOutDir > 0)
		{
			DestMonster->SetObjectMoveDir(Vector3(-1.f, 0.f, 0.f));
		}
		else
		{
			DestMonster->SetObjectMoveDir(Vector3(1.f, 0.f, 0.f));
		}

		// DestMonster->Damage(2.f);

		// Create Damage Font
		ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();

		if (ObjectWindow)
		{
			CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");
		}
	}
}
