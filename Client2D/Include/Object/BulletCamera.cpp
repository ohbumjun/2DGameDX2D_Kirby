#include "BulletCamera.h"

#include "Scene/SceneManager.h"
#include "Scene/CameraManager.h"
#include "Component/CameraComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

CBulletCamera::CBulletCamera() : m_Distance(700.f)
{
	SetTypeID<CBulletCamera>();
}

CBulletCamera::CBulletCamera(const CBulletCamera& Camera) :
CGameObject(Camera)
{
	m_Sprite    = (CSpriteComponent*)const_cast<CBulletCamera&>(Camera).FindComponent("BulletSprite");
	m_Camera  = (CCameraComponent*)const_cast<CBulletCamera&>(Camera).FindComponent("BulletSprite");
	m_Body      = (CColliderBox2D*)const_cast<CBulletCamera&>(Camera).FindComponent("BulletSprite");
	m_Distance = Camera.m_Distance;
}

CBulletCamera::~CBulletCamera()
{
	
}

void CBulletCamera::Start()
{
	CGameObject::Start();

	m_Scene->GetCameraManager()->KeepCamera();
	m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);
}

bool CBulletCamera::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("BulletSprite");
	m_Camera = CreateComponent<CCameraComponent>("BulletCamera");
	m_Body = CreateComponent<CColliderBox2D>("BulletCollider");

	m_Body->SetCollisionProfile("PlayerAttack");
	m_Body->AddCollisionCallback(Collision_State::Begin, this, &CBulletCamera::CollisionCallback);

	SetRootComponent(m_Sprite);

	m_Sprite->AddChild(m_Camera);
	m_Sprite->AddChild(m_Body);

	m_Camera->OnViewPortCenter();

	m_Sprite->SetRelativeScale(50.f, 50.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CBulletCamera::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	float CurrentMove = 300.f * DeltaTime;

	m_Distance -= CurrentMove;

	if (m_Distance < 0.f)
	{
		m_Scene->GetCameraManager()->ReturnCamera();
		Destroy();
	}

	AddWorldPos(GetWorldAxis(AXIS_Y) * CurrentMove);
}

void CBulletCamera::CollisionCallback(const CollisionResult& Result)
{
	m_Scene->GetCameraManager()->ReturnCamera();
	Destroy();
}
