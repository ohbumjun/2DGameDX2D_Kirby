#pragma once
#include "GameObject\GameObject.h"
class CMonster :
    public CGameObject
{
public :
    CMonster();
    CMonster(const CMonster& Monster);
    virtual ~CMonster() override;
public :
    CSharedPtr<class CSpriteComponent> m_Sprite;
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CMonster* Clone() override;
};

