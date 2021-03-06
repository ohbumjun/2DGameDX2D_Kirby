#include "EffectRandomStar.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"

CEffectRandomStar::CEffectRandomStar()
{
	SetTypeID<CEffectRandomStar>();

	m_LifeTime = 0.5f;
}

CEffectRandomStar::CEffectRandomStar(const CEffectRandomStar& Beatle)
{}

CEffectRandomStar::~CEffectRandomStar()
{}

void CEffectRandomStar::SetAttackXDir(float XDir)
{
	m_RandomDir = Vector3(XDir, m_RandomDir.y, 0.f);
}

void CEffectRandomStar::Start()
{
	CGameObject::Start();
}

bool CEffectRandomStar::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("EffectSprite");

	SetRootComponent(m_Sprite);

	SetPivot(0.5, 0.5f, 0.f);

	m_Sprite->SetRenderState("AlphaBlend");

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"RandomStarEffect", TEXT("Effect_RandomStar.anim"));

	if (!AnimationInstance)
	{
		assert(false);
		return false;
	}

	// Clone 해서 세팅해줘야 한다.
	m_Sprite->SetAnimationInstance(AnimationInstance);

	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");

	m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->SetPlayScale(
		m_LifeTime + ((float)rand() / (float)RAND_MAX));

	Vector3 CurWorldScale = GetWorldScale();

	float DirX = (int)(rand() % 2) == 0 ? -1.f : 1.f;
	float DirY = float(rand()) / float((RAND_MAX));

	m_RandomDir = Vector3(DirX, DirY, 0.f);

	SetWorldScale(CurWorldScale.x * 1.8f, CurWorldScale.y * 1.8f, 1.f);

	m_MoveSpeed = 200.f + ((float)rand() / (float)RAND_MAX) * 200.f;

	return true;
}

void CEffectRandomStar::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Vector3 WorldPos = GetWorldPos();

	m_Sprite->AddWorldPos(Vector3(m_RandomDir.x, m_RandomDir.y, 0.f) * m_MoveSpeed * DeltaTime);
}

void CEffectRandomStar::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

}

CEffectRandomStar* CEffectRandomStar::Clone()
{
	return new CEffectRandomStar(*this);
}
