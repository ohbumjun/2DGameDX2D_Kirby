#include "BackGround.h"

CBackGround::CBackGround()
{
	SetTypeID<CBackGround>();
}

CBackGround::CBackGround(const CBackGround& Map)
{}

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

	m_BackGroundComponent = CreateComponent<CBackGroundComponent>("BackGround");


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
