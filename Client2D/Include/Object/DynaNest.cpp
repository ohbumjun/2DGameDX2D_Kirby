#include "DynaNest.h"
#include "Component/SpriteComponent.h"

CDynaNest::CDynaNest()
{}

CDynaNest::~CDynaNest()
{}

void CDynaNest::MakeDynaBabyEffect()
{}

void CDynaNest::Start()
{
	CGameObject::Start();
}

bool CDynaNest::Init()
{
	if (CGameObject::Init())
		return false;

	return true;
}

void CDynaNest::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// ��� �Ʒ��� ��������.

	// Dyna Baby ���� ������.
}

void CDynaNest::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
