#include "BombUltimate.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../Component/BombKirbyState.h"
#include "Scene/CameraManager.h"
#include "FireMonster.h"
#include "Player2D.h"
#include "FireAttackBackEffect.h"
#include "Component/ColliderCircle.h"

CBombUltimate::CBombUltimate()  :
m_ExplodeWaitTime(1.f),
m_CameraReturned(false)
{}

CBombUltimate::~CBombUltimate()
{}

void CBombUltimate::Start()
{
	CKirbyAttackEffect::Start();
}

bool CBombUltimate::Init()
{
	if (!CKirbyAttackEffect::Init())
		return false;

	m_MainSprite->SetWorldScale(700.f, 700.f, 1.f);
	m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);

	m_AttackDistLimitMax = 1200.f;
	m_AttackObjectSpeed = 700.f;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"KirbyFallBombAttackEffect", TEXT("Ability_Bomb_ThrowBomb.anim"));

	m_MainSprite->SetAnimationInstance(AnimationInstance);

	m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectRight")->SetPlayTime(0.1f);
	m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectLeft")->SetPlayTime(0.1f);

	m_SideCollisionApplied = false;

	SetLifeTime(1.2f);


	return true;
}

void CBombUltimate::Update(float DeltaTime)
{
	CKirbyAttackEffect::Update(DeltaTime);

	m_ExplodeCurTime += DeltaTime;

	if (m_ExplodeCurTime >= m_ExplodeWaitTime - 0.1f)
	{
		if (!m_CameraReturned)
		{
			m_Scene->GetCameraManager()->ReturnCamera();
			m_CameraReturned = true;
		}
	}

	if (m_ExplodeCurTime >= m_ExplodeWaitTime)
	{
		CreateMultipleExplosionEffect();

		// 주변에 폭발 효과를 연달아 낸다.

		Destroy();

		// 카메라 되돌리기
	}
}
void CBombUltimate::PostUpdate(float DeltaTime)
{
	CKirbyAttackEffect::PostUpdate(DeltaTime);
}

void CBombUltimate::CreateMultipleExplosionEffect()
{

	for (int j = 1; j <= 2; j++)
	{
		for (int i = 0; i < 12; i++)
		{
			float XPos = GetWorldPos().x + cosf(DegreeToRadian(30.f * i)) * (100.f + (j - 1) * 200);
			float YPos = GetWorldPos().y + sinf(DegreeToRadian(30.f * i)) * (100.f + (j - 1) * 200);

			CBombUltimate* AttackEffect = m_Scene->CreateGameObject<CBombUltimate>("Attack");
			AttackEffect->SetRightAttackDir(0.f);
			AttackEffect->SetAttackDirX(0.f);

			if (j & 1)
			{
				AttackEffect->m_MainSprite->SetWorldScale(100.f, 100.f, 1.f);
				AttackEffect->m_Collider->SetInfo(Vector2(0.f, 0.f), AttackEffect->m_MainSprite->GetWorldScale().x * 0.4f);
			}
			else
			{
				AttackEffect->m_MainSprite->SetWorldScale(220.f, 220.f, 1.f);
				AttackEffect->m_Collider->SetInfo(Vector2(0.f, 0.f), AttackEffect->m_MainSprite->GetWorldScale().x * 0.4f);

			}

			AttackEffect->m_MainSprite->GetAnimationInstance()->ChangeAnimation("Explode");
			AttackEffect->m_MainSprite->GetAnimationInstance()->GetCurrentAnimation()->SetPlayTime(0.5f);
			AttackEffect->SetWorldPos(XPos, YPos, GetWorldPos().z);
			AttackEffect->SetKirbyOwner(m_KirbyOwner);
			AttackEffect->SetAttackDamage(m_AttackDamage);
			AttackEffect->SetLifeTime(0.5f);
		}
	}
}
