#include "LifeObject.h"

CLifeObject::CLifeObject()
{}

CLifeObject::CLifeObject(const CLifeObject& obj) : CGameObject(obj)
{}

CLifeObject::~CLifeObject()
{}

void CLifeObject::Start()
{
	CGameObject::Start();
}

bool CLifeObject::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CLifeObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CLifeObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CLifeObject::PrevRender()
{
	CGameObject::PrevRender();
}

void CLifeObject::Render()
{
	CGameObject::Render();
}

void CLifeObject::PostRender()
{
	CGameObject::PostRender();
}

CLifeObject* CLifeObject::Clone()
{
	return new CLifeObject(*this);
}
