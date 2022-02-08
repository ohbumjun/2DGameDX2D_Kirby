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
    CSharedPtr<class CWidgetComponent> m_SimpleHUDWidget;
    CSharedPtr<class CUIProgressBar> m_HpBar;
    CSharedPtr<class CPaperBurnComponent> m_PaperBurn;
    // CSharedPtr<class CColliderComponent> m_ColliderBody;
    float m_HPMax;
    float m_HP;
public :
    void Damage(float Damage);
    void SetHPMax(float HPMax);
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CMonster* Clone() override;
public :
    void OnMouseBegin(const CollisionResult& Result);
    void OnMouseEnd(const CollisionResult& Result);
    void CreateDamageFont(const CollisionResult& Result);
    void OnCollisionBegin(const CollisionResult& Result);
private :
    void PaperBurnEnd();
};


