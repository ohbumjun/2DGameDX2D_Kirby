#include "Item.h"
#include <Scene/Scene.h>
#include "Component/ColliderComponent.h"
#include "Component/ColliderCircle.h"
#include "Player2D.h"

CItem::CItem()
{}

CItem::CItem(const CItem& Monster)
{}

CItem::~CItem()
{}

void CItem::Start()
{
	CGameObject::Start();

	m_ColliderBody = FindComponentByType<CColliderCircle>();

	if (m_ColliderBody)
	{
		m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CItem::CollisionPlayerCallback);
	}
}

bool CItem::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CItem::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CItem::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}


void CItem::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	fread(&m_ItemType, sizeof(Item_Type), 1, pFile);
}

void CItem::CollisionPlayerCallback(const CollisionResult& Result)
{
	CGameObject* DestObj = Result.Dest->GetGameObject();

	if (DestObj == m_Scene->GetPlayerObject())
	{
		CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();

		Player->AddHP(150.f);

		Destroy();
	}
}
