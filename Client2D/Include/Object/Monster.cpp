#include "Monster.h"
#include "Component/ColliderBox2D.h"
#include "Component/SpriteComponent.h"

CMonster::CMonster()
{}

CMonster::CMonster(const CMonster& Monster)
{}

CMonster::~CMonster()
{}

bool CMonster::Init()
{
	return CGameObject::Init();
}
