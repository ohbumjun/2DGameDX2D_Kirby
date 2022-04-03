#include "DynaFarAttack.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include <Component/SpriteComponent.h>
#include <Scene/CameraManager.h>
#include "BossTree.h"
#include "Player2D.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"
#include "FireAttackBackEffect.h"

CDynaFarAttack::CDynaFarAttack()
{}

CDynaFarAttack::~CDynaFarAttack()
{}

void CDynaFarAttack::SetRightAttackDir(float YDir)
{
	CAttackEffect::SetRightAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
}

void CDynaFarAttack::SetLeftAttackDir(float YDir)
{
	CAttackEffect::SetLeftAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
}

void CDynaFarAttack::BottomCollisionSpecificAction()
{
	// Attack Back Effect
	CFireAttackBackEffect* BackEffect = m_Scene->CreateGameObject<CFireAttackBackEffect>("BackFire");

	BackEffect->SetWorldPos(GetWorldPos().x, GetWorldPos().y - 40.f, GetWorldPos().z);

	BackEffect->SetWorldScale(150.f, 150.f, 1.f);

	BackEffect->GetColliderBody()->SetInfo(Vector2(0.f, 0.f), BackEffect->GetWorldScale().x * 0.5f);

	BackEffect->AddRelativeRotationZ(90.f);

	BackEffect->SetLifeTime(0.4f);

	Destroy();

	m_Scene->GetCameraManager()->GetCurrentCamera()->ApplyShakeEffect();
}


void CDynaFarAttack::Start()
{
	CAttackEffect::Start();
}

bool CDynaFarAttack::Init()
{
	if (!CAttackEffect::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"BossDynaFarAttack", TEXT("Boss_Dyna_FarAttack.anim"));

	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->SetWorldScale(120.f, 120.f, 1.f);

	m_Collider->SetCollisionProfile("MonsterAttack");
	m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);
	m_Collider->AddCollisionCallback(Collision_State::Begin, (CAttackEffect*)this, &CAttackEffect::MonsterAttackCollisionCallback);

	SetBottomCollisionEnable(true);

	return true;
}

void CDynaFarAttack::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);
}

void CDynaFarAttack::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}
