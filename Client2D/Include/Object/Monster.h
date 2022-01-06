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
    CSharedPtr<class CColliderCircle> m_ColliderBody;
    // CSharedPtr<class CColliderComponent> m_ColliderBody;
    float m_HP;
public :
    void Damage(float Damage);
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CMonster* Clone() override;
};

