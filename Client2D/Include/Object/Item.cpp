#include "Item.h"
#include <Scene/Scene.h>
#include "Component/ColliderComponent.h"
#include "Component/ColliderCircle.h"
#include "Player2D.h"

CItem::CItem() :
	m_ToggleCurTimeMax(0.5f)
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

	m_Sprite = CreateComponent<CSpriteComponent>("ItemSprite");
	SetRootComponent(m_Sprite);
	SetWorldScale(50.f, 50.f, 1.f); 
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	// m_ColliderBody = CreateComponent<CColliderBox2D>("ColliderBody");
	m_ColliderBody = CreateComponent<CColliderCircle>("ColliderBody");
	m_ColliderBody->SetCollisionProfile("MonsterAttack");
	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CItem::CollisionPlayerCallback);
	m_ColliderBody->SetInfo(Vector2(0.f, 0.f), m_Sprite->GetWorldScale().x * 0.5f);

	m_Sprite->AddChild(m_ColliderBody);

	return true;
}

void CItem::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	UpdateToggleTime(DeltaTime);
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

void CItem::UpdateToggleTime(float DeltaTime)
{
	m_ToggleCurTime += DeltaTime;

	if (m_ToggleUp)
		AddWorldPos(Vector3(0.f, 1.f, 0.f) * DeltaTime * 30.f);
	else
		AddWorldPos(Vector3(0.f, -1.f, 0.f) * DeltaTime * 30.f);

	if (m_ToggleCurTime >= m_ToggleCurTimeMax)
	{
		m_ToggleCurTime = 0.f;

		m_ToggleUp = m_ToggleUp ? false : true;
	}
}

void CItem::CollisionPlayerCallback(const CollisionResult& Result)
{
	CGameObject* DestObj = Result.Dest->GetGameObject();

	if (DestObj == m_Scene->GetPlayerObject())
	{
		CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();

		Player->AddHP(300.f);

		Destroy();
	}
}
