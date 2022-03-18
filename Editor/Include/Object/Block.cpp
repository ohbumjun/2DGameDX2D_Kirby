#include "Block.h"
#include "../Window/TileMapWindow.h"
#include "../EditorManager.h"
#include "Component/TileEmptyComponent.h"
#include "Component/Tile.h"
#include "Scene/Scene.h"

CBlock::CBlock()
{
	SetTypeID<CBlock>();
}

CBlock::CBlock(const CBlock& Monster)
{}

CBlock::~CBlock()
{}

void CBlock::Start()
{
	CGameObject::Start();
}

bool CBlock::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("ItemSprite");
	SetRootComponent(m_Sprite);
	SetWorldScale(70.f, 70.f, 1.f); //
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"GreenItem", TEXT("Item_Block.anim"));

	// Clone 해서 세팅해줘야 한다.
	m_Sprite->SetAnimationInstance(AnimationInstance);

	// m_ColliderBody = CreateComponent<CColliderBox2D>("ColliderBody");
	m_ColliderBody = CreateComponent<CColliderCircle>("ColliderBody");
	m_ColliderBody->SetCollisionProfile("Monster");

	m_Sprite->AddChild(m_ColliderBody);

	m_ColliderBody->SetInfo(Vector2(0.f, 0.f), m_Sprite->GetWorldScale().x * 0.5f);

	return true;
}

void CBlock::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CBlock::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBlock::MakeTileTypeAround(bool MakeBlock)
{
	Vector3 m_Pos = GetWorldPos();

	CTileEmptyComponent* TileMap = CEditorManager::GetInst()->GetTileMapWindow()->GetTileMapEmpty();

	if (!TileMap)
		return;

	Vector3 Pivot = GetPivot();
	Vector3 WorldScale = GetWorldScale();

	float CurBottom = m_Pos.y - Pivot.y * WorldScale.y;
	float CurTop       = m_Pos.y + Pivot.y * WorldScale.y;
	float CurLeft      = m_Pos.x - Pivot.x * WorldScale.x;
	float CurRight    = CurLeft + WorldScale.x;

	int LeftIdx = -1, TopIdx = -1, RightIdx = -1, BottomIdx = -1;

	Vector3 LB = Vector3(CurLeft, CurBottom, 0.f);
	Vector3 RT = Vector3(CurRight, CurTop, 0.f);

	LeftIdx = TileMap->GetTileEmptyIndexX(LB);
	RightIdx = TileMap->GetTileEmptyIndexX(RT);

	BottomIdx = TileMap->GetTileEmptyIndexY(LB);
	TopIdx = TileMap->GetTileEmptyIndexY(RT);

	if (LeftIdx < 0)
		LeftIdx = 0;

	if (BottomIdx < 0)
		BottomIdx = 0;

	if (RightIdx >= TileMap->GetTileCountX())
		RightIdx = TileMap->GetTileCountX() - 1;

	if (TopIdx >= TileMap->GetTileCountY())
		TopIdx = TileMap->GetTileCountY() - 1;

	for (int row = TopIdx; row >= BottomIdx; row--)
	{
		for (int col = LeftIdx; col <= RightIdx; col++)
		{
			// Block Type 으로 만들기 
			if (MakeBlock)
			{
				if (TileMap->GetTileEmpty(col, row)->GetTileType() == Tile_Type::Normal)
				{
					TileMap->GetTileEmpty(col, row)->SetTileType(Tile_Type::Block);
				}
			}
			// Normal Type으로 만들기 
			else
			{
				if (TileMap->GetTileEmpty(col, row)->GetTileType() == Tile_Type::Block)
				{
					TileMap->GetTileEmpty(col, row)->SetTileType(Tile_Type::Normal);
				}
			}
		}
	}
}

