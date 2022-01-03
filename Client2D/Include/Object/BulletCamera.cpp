#include "BulletCamera.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"

CBulletCamera::CBulletCamera() :
	m_Distance(600.f)
{
	SetTypeID<CBulletCamera>();
}

CBulletCamera::CBulletCamera(const CBulletCamera& Bullet)  :
CGameObject(Bullet)
{
}


CBulletCamera::~CBulletCamera() 
{}

void CBulletCamera::Start()
{
	CGameObject::Start();
}

bool CBulletCamera::Init()
{
	return CGameObject::Init();
}

void CBulletCamera::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CBulletCamera::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CBulletCamera* CBulletCamera::Clone()
{
	return CGameObject::Clone();
}

void CBulletCamera::CollisionCallback(const CollisionResult& result)
{}
