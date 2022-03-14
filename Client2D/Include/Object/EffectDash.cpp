#include "EffectDash.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"

CEffectDash::CEffectDash() :
	m_GoRight(true)
{
	SetTypeID<CEffectDash>();

	m_LifeTime = 1.f;
}

CEffectDash::CEffectDash(const CEffectDash& Beatle)
{}

CEffectDash::~CEffectDash()
{}

void CEffectDash::SetDirGoRight(bool Enable)
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

void CEffectDash::Start()
{
	CGameObject::Start();
}

bool CEffectDash::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("EffectSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRenderState("AlphaBlend");

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("DashEffect", TEXT("Effect_Air.anim"));

	if (!AnimationInstance)
	{
		assert(false);
		return false;
	}

	// Clone �ؼ� ��������� �Ѵ�.
	m_Sprite->SetAnimationInstance(AnimationInstance);

	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");

	return true;
}

void CEffectDash::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectDash::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	if (m_GoRight)
	{
		m_Sprite->AddWorldPos(Vector3(1.f, 0.f, 0.f) * 50.f * DeltaTime);
	}
	else
	{
		m_Sprite->AddWorldPos(Vector3(1.f * -1.f, 0.f, 0.f) * 50.f * DeltaTime);
	}
}

CEffectDash* CEffectDash::Clone()
{
	return new CEffectDash(*this);
}
