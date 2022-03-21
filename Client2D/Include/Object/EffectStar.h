#pragma once
#include "AttackEffect.h"
#include "../Client.h"

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
    Ability_State m_AbilityState;
    bool m_IsBeingPulled;
    float m_BeginPulledAccelSum;
	float m_BeginPulledAccel;
    bool m_IsPulledAgainStar;
    float m_ColliderProfileChangeTime;
    float m_ColliderProfileChangeTimeMax;
private :
    // ¹ÝÂ¦ÀÓ
    bool m_SpecialStarLightToggle;
    float m_SpecialStarLightTime;
    float m_SpecialStarLightTimeMax;
public :
    Ability_State GetAbilityState() const
{
        return m_AbilityState;
}
    bool IsSpecialKirbyStar() const
{
        return m_IsSpecialKirbyStar;
}
public :
    void SetIsBeingPulled(bool Enable)
{
        m_IsBeingPulled = Enable;
}
    void SetAbilityState(Ability_State State)
{
        m_AbilityState = State;
}
    void SetIsSpecialKirbyStar(bool Enable)
{
        m_IsSpecialKirbyStar = Enable;
}
    void SetIsRotate(bool Enable)
{
        m_IsRotate = Enable;
}
    void SetIsPulledAgainStar(bool Enable)
{
        m_IsPulledAgainStar = Enable;
}
protected:
    virtual void BottomCollisionSpecificAction() override;
    virtual void SideCollisionSpecificAction() override;
protected :
    void UpdateBeingPulled(float DeltaTime);
    void UpdateLightToggle(float DeltaTime);
 public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectStar* Clone() override;
private :
    void StarCollision(const CollisionResult& Result);
};

