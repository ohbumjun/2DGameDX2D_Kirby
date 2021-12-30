#pragma once
#include "GameObject\GameObject.h"
class CMonster :
    public CGameObject
{
public :
    CMonster();
    CMonster(const CMonster& Monster);
    virtual ~CMonster() override;
};

