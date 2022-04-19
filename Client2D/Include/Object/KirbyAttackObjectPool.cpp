#include "KirbyAttackObjectPool.h"

CKirbyAttackObjectPool::CKirbyAttackObjectPool()
{}

CKirbyAttackObjectPool::~CKirbyAttackObjectPool()
{}

void CKirbyAttackObjectPool::SetAttackType(KirbyAttackEffect_Type Type)
{}

bool CKirbyAttackObjectPool::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CKirbyAttackObjectPool::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}
