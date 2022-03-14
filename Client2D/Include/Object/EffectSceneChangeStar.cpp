#include "EffectSceneChangeStar.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/CameraManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/ColliderCircle.h"
#include "EffectKirbyRide.h"
#include "Engine.h"

CEffectSceneChangeStar::CEffectSceneChangeStar()
{
	SetTypeID<CEffectSceneChangeStar>();
}

CEffectSceneChangeStar::CEffectSceneChangeStar(const CEffectSceneChangeStar& Beatle) : CGameObject(Beatle)
{}

CEffectSceneChangeStar::~CEffectSceneChangeStar()
{}

void CEffectSceneChangeStar::Start()
{
	CGameObject::Start();

	m_Sprite = (CSpriteComponent*)FindComponent("EffectSprite");
	m_ColliderBody = (CColliderCircle*)FindComponent("EffectChangeStarColliderBody");
	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectSceneChangeStar::CreateKirbyRideAndChangeToNextScene);
	m_ColliderBody->SetCollisionProfile("PlayerEffect");
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

	// Clone �ؼ� ��������� �Ѵ�.
	m_Sprite->SetAnimationInstance(AnimationInstance);

	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");

	SetWorldScale(GetWorldScale().x * 2.0f, GetWorldScale().y * 2.0f, 1.f);

	m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);

	// Todo : Collider Circle
	m_ColliderBody = CreateComponent<CColliderCircle>("EffectChangeStarColliderBody");

	Vector2 ColliderCenter = Vector2(
		m_Sprite->GetWorldPos().x + m_Sprite->GetWorldScale().x * m_Sprite->GetPivot().x,
		m_Sprite->GetWorldPos().y + m_Sprite->GetWorldScale().y * m_Sprite->GetPivot().y
	);

	m_ColliderBody->SetInfo(ColliderCenter, m_Sprite->GetWorldScale().x * 0.4f);
	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectSceneChangeStar::CreateKirbyRideAndChangeToNextScene);

	m_Sprite->AddChild(m_ColliderBody);


	return true;
}

void CEffectSceneChangeStar::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// ��� ȸ�� ��Ű�� 
}

void CEffectSceneChangeStar::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CEffectSceneChangeStar* CEffectSceneChangeStar::Clone()
{
	return new CEffectSceneChangeStar(*this);
}

void CEffectSceneChangeStar::CreateKirbyRideAndChangeToNextScene(const CollisionResult& Result)
{
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (m_Scene->GetPlayerObject() == DestObject)
	{
		// Kirby Ride Object �����ϰ�
		CEffectKirbyRide* KirbyRide = m_Scene->CreateGameObject<CEffectKirbyRide>("KirbyRide");

		KirbyRide->SetWorldPos(GetWorldPos());

		// Player�� ȭ�鿡�� �Ⱥ��̰� �ϰ�
		DestObject->Enable(false);

		// ���� ī�޶� Player ���� �����ְ�, EffectKirby Ride �� �ٿ��ش�.
		CCameraComponent* MainCamera = m_Scene->GetCameraManager()->GetCurrentCamera();

		DestObject->GetRootComponent()->DeleteChild(MainCamera);

		KirbyRide->GetRootComponent()->AddChild(MainCamera);

		Resolution  RS = CEngine::GetInst()->GetResolution();

		/*
		MainCamera->SetWorldPos(Vector3(KirbyRide->GetWorldPos().x - (float)RS.Width * 0.5f,
															KirbyRide->GetWorldPos().y - (float)RS.Height * 0.5f, 
															KirbyRide->GetWorldPos().z));
		*/

		MainCamera->SetGameObject(this);

		// �ڱ� �ڽŵ� ������� �����Ѵ�
		Destroy();
	}
}
