#include "EffectSpitOut.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"

CEffectSpitOut::CEffectSpitOut() :
	m_GoRight(true)
{
	SetTypeID<CEffectSpitOut>();

	m_LifeTime = 0.5f;
}

CEffectSpitOut::CEffectSpitOut(const CEffectSpitOut& Beatle)
{}

CEffectSpitOut::~CEffectSpitOut()
{}

void CEffectSpitOut::SetDirGoRight(bool Enable)
{
	m_GoRight = Enable ? true : false;

	if (Enable)
	{
		m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
	}
	else
	{
		m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
	}
}

void CEffectSpitOut::Start()
{
	CGameObject::Start();
}

bool CEffectSpitOut::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("EffectSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRenderState("AlphaBlend");

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("SpitOutEffect", TEXT("Effect_SpitOut.anim"));

	m_Sprite->SetAnimationInstance(AnimationInstance);

	// Clone 해서 세팅해줘야 한다.
	m_Sprite->GetAnimationInstance()->SetPlayScaleOfAllAnimation(m_LifeTime);
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
	m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);
	// SetWorldScale(GetWorldScale().x * 0.7f, GetWorldScale().y * 0.7f, 1.f);


	// Todo : Collider Circle
	m_ColliderBody = CreateComponent<CColliderCircle>("ColliderBody");

	Vector2 ColliderCenter = Vector2(
		m_Sprite->GetWorldPos().x + m_Sprite->GetWorldScale().x * m_Sprite->GetPivot().x,
		m_Sprite->GetWorldPos().y + m_Sprite->GetWorldScale().y * m_Sprite->GetPivot().y
	);

	m_ColliderBody->SetInfo(ColliderCenter, m_Sprite->GetWorldScale().x * 0.3f);

	m_ColliderBody->SetCollisionProfile("Player");

	m_Sprite->AddChild(m_ColliderBody);

	return true;
}

void CEffectSpitOut::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CEffectSpitOut::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	if (m_GoRight)
	{
		m_Sprite->AddWorldPos(Vector3(1.f, 0.f, 0.f) * 320.f * DeltaTime);
	}
	else
	{
		m_Sprite->AddWorldPos(Vector3(1.f * -1.f, 0.f, 0.f) * 320.f * DeltaTime);
	}
}

CEffectSpitOut* CEffectSpitOut::Clone()
{
	return new CEffectSpitOut(*this);
}
