#include "EffectSceneChangeStar.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"

CEffectSceneChangeStar::CEffectSceneChangeStar()
{
	SetTypeID<CEffectSceneChangeStar>();
}

CEffectSceneChangeStar::CEffectSceneChangeStar(const CEffectSceneChangeStar& Beatle)
{}

CEffectSceneChangeStar::~CEffectSceneChangeStar()
{}

void CEffectSceneChangeStar::Start()
{
	CGameObject::Start();

	m_Sprite = (CSpriteComponent*)FindComponent("EffectSprite");
	m_ColliderBody = (CColliderCircle*)FindComponent("ColliderBody");

}

bool CEffectSceneChangeStar::Init()
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

	SetWorldScale(GetWorldScale().x * 2.0f, GetWorldScale().y * 2.0f, 1.f);

	m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);

	// Todo : Collider Circle
	m_ColliderBody = CreateComponent<CColliderCircle>("ColliderBody");

	Vector2 ColliderCenter = Vector2(
		m_Sprite->GetWorldPos().x + m_Sprite->GetWorldScale().x * m_Sprite->GetPivot().x,
		m_Sprite->GetWorldPos().y + m_Sprite->GetWorldScale().y * m_Sprite->GetPivot().y
	);

	m_ColliderBody->SetInfo(ColliderCenter, m_Sprite->GetWorldScale().x * 0.4f);

	m_ColliderBody->SetCollisionProfile("Player");

	m_Sprite->AddChild(m_ColliderBody);


	return true;
}

void CEffectSceneChangeStar::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// 계속 회전 시키기 
}

void CEffectSceneChangeStar::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CEffectSceneChangeStar* CEffectSceneChangeStar::Clone()
{
	return new CEffectSceneChangeStar(*this);
}
