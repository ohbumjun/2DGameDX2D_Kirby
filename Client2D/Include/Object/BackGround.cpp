#include "BackGround.h"
#include "Component/BackGroundComponent.h"

CBackGround::CBackGround()
{
	SetTypeID<CBackGround>();
}

CBackGround::CBackGround(const CBackGround& Map)
{
	m_BackGroundComponent = (CBackGroundComponent*)FindComponent("BackGround");
}

CBackGround::~CBackGround()
{}

void CBackGround::Start()
{
	CGameObject::Start();
}

bool CBackGround::Init()
{
	if (!CGameObject::Init())
		return false;

	// m_BackGroundComponent = CreateComponent<CBackGroundComponent>("BackGround");

	// Tile
	// m_TileEmptyComponent->CreateTileEmpty(10, 10, Vector3(50.f, 50.f, 0.f));


	return true;
}

void CBackGround::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CBackGround::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBackGround::PrevRender()
{
	CGameObject::PrevRender();

	// ???? ?ڵ?
	// m_BackGroundComponent->GetBackGroundMaterial()->SetBaseColor(1.f, 1.f, 0.7f, 0.5f);
}

void CBackGround::Render()
{

	CGameObject::Render();
}

void CBackGround::PostRender()
{
	CGameObject::PostRender();
}

CBackGround* CBackGround::Clone()
{
	return new CBackGround(*this);
}

void CBackGround::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CBackGround::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	m_BackGroundComponent = (CBackGroundComponent*)m_RootComponent.Get();
}
