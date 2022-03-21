#pragma once
#include "AttackEffect.h"

class CEffectStar :
    public CAttackEffect
{
    friend class CScene;
    friend class CPlayer2D;
    friend class CEffectKirbyRide;
protected:
    CEffectStar();
    CEffectStar(const CEffectStar& Dash);
    virtual ~CEffectStar();
private:
    bool m_IsRotate;
    bool m_IsSpecialKirbyStar;
public :
    void SetIsSpecialKirbyStar(bool Enable)
{
        m_IsSpecialKirbyStar = Enable;
}
    void SetIsRotate(bool Enable)
{
        m_IsRotate = Enable;
}
protected:
    virtual void BottomCollisionSpecificAction() override;
    virtual void SideCollisionSpecificAction() override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectStar* Clone() override;
private :
    void StarCollision(const CollisionResult& Result);
};

