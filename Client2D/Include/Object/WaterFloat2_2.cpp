#include "WaterFloat2_2.h"
#include "Component/TileEmptyComponent.h"
#include "Component/Tile.h"
#include "Scene/Scene.h"
#include "Player2D.h"

CWaterFloat2_2::CWaterFloat2_2()
{
	SetTypeID<CWaterFloat2_2>();
}

CWaterFloat2_2::~CWaterFloat2_2()
{}

void CWaterFloat2_2::Start()
{
	CGameObject::Start();
}

bool CWaterFloat2_2::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CWaterFloat2_2::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CWaterFloat2_2::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
