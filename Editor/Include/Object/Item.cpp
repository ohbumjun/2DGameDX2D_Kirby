#include "Item.h"

CItem::CItem()
{
	m_ItemType = Item_Type::HPYellowPotion;
}

CItem::CItem(const CItem& Monster)
{}

CItem::~CItem()
{}

void CItem::Start()
{
	CGameObject::Start();
}

bool CItem::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("ItemSprite");
	SetRootComponent(m_Sprite);
	SetWorldScale(50.f, 50.f, 1.f); //
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	// m_ColliderBody = CreateComponent<CColliderBox2D>("ColliderBody");
	m_ColliderBody = CreateComponent<CColliderCircle>("ColliderBody");
	m_ColliderBody->SetCollisionProfile("MonsterAttack");

	m_Sprite->AddChild(m_ColliderBody);

	m_ColliderBody->SetInfo(Vector2(0.f, 0.f), m_Sprite->GetWorldScale().x * 0.5f);

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

void CItem::Save(FILE* pFile)
{
	CGameObject::Save(pFile);

	fwrite(&m_ItemType, sizeof(Item_Type), 1, pFile);
}