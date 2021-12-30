#pragma once
#include "GameObject\GameObject.h"

class CMonster :
    public CGameObject
{
public :
    CMonster();
    CMonster(const CMonster& Monster);
    virtual ~CMonster() override;
private :
    CSharedPtr<class CSpriteComponent> m_Sprite;
    CSharedPtr<class CColliderBox2D> m_Body;
public :
    virtual bool Init() override;
};

