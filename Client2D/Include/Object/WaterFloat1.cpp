#include "WaterFloat1.h"
#include "Component/TileEmptyComponent.h"
#include "Component/Tile.h"
#include "Scene/Scene.h"
#include "Player2D.h"

CWaterFloat1::CWaterFloat1()
{
	SetTypeID<CWaterFloat1>();
}

CWaterFloat1::CWaterFloat1(const CWaterFloat1& Monster)
{}

CWaterFloat1::~CWaterFloat1()
{}

void CWaterFloat1::Start()
{
	CGameObject::Start();
}

bool CWaterFloat1::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CWaterFloat1::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CWaterFloat1::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
