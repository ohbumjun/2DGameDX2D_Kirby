#pragma once
#include "GameObject\LifeObject.h"
#include "../Client.h"

class CMonster :
    public CLifeObject
{
public :
    CMonster();
    CMonster(const CMonster& Monster);
    virtual ~CMonster();
private :
    CSharedPtr<class CSpriteComponent> m_Sprite;
    CSharedPtr<class CColliderCircle> m_ColliderBody;
    CSharedPtr<class CWidgetComponent> m_SimpleHUDWidget;
    CSharedPtr<class CUIProgressBar> m_HpBar;
    CSharedPtr<class CPaperBurnComponent> m_PaperBurn;
    float m_HPMax;
    float m_HP;
    float m_DeathAccTime;
    float m_DeathFinishTime;
    bool m_DeathStart;

    // Player 에게 끌어당겨지기
    bool m_IsBeingPulled;
    float m_BeginPulledAccel;
    float m_BeginPulledAccelSum;
    Vector3 m_PulledDestPos;

    // AI
    Monster_AI m_AI;
    float m_AttackDistance;
    float m_DashDistance;
    bool m_IsBeingHit;

    // Move
    float m_MonsterMoveVelocity;
    float m_RandomMoveTime;
    float m_RandomMoveTimeMax;
    Vector3 m_MonsterMoveDir;

public :
    bool IsBeingPulled() const
{
        return m_IsBeingPulled;
}
    bool IsBeingHit () const
{
        return m_IsBeingHit;
}
public :
    void SetBeingHit (bool Enable)
{
        m_IsBeingHit = Enable;
}
    void SetPulledDestPos(Vector3 Pos)
{
        m_PulledDestPos = Pos;
}
    void SetIsBeingPulled (bool Enable)
{
        m_IsBeingPulled = Enable;
}
    void ResetPulledInfo();

public :
    void LoadAnimationInstance(const std::string& Name, const TCHAR* FileName, const std::string& PathName = ENGINE_ANIMATION_PATH);
    void SetCurrentAnimation(const std::string& Name);
public :
    void Damage(float Damage);
    void SetHPMax(float HPMax);
    void DeathStart();
protected : // AI
    void AIStateUpdate(float DeltaTime);
    void AIActionUpdate(float DeltaTime);
    virtual void AIIdle(float DeltaTime);
    virtual void AIWalk(float DeltaTime);
    virtual void AITrace(float DeltaTime, Vector3 PlayerPos);
    virtual void AIAttack(float DeltaTime, Vector3 PlayerPos);
    virtual void AIDeath(float DeltaTime);
    virtual void AIHit(float DeltaTime);
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime)override;
    virtual void PostUpdate(float DeltaTime)override;
    virtual CMonster* Clone() override;
protected:
    void UpdateBeingPulled(float DeltaTime);
protected:
    void UpdateMonsterMove(float DeltaTime);
    void SetRandomTargetDir();
protected:
    void OnMouseBegin(const CollisionResult& Result);
    void OnMouseEnd(const CollisionResult& Result);
    void CreateDamageFont(const CollisionResult& Result);
    void OnCollisionBegin(const CollisionResult& Result);
protected: // Animation
    void ChangeIdleAnimation();
    void ChangeWalkAnimation();
    void ChangeHitAnimation();
    void ChangeTraceAnimation();
    void ChangeDeathAnimation();
    void ChangeAttackAnimation();
protected:
    void PaperBurnEnd();
public :
    virtual void Save(FILE* pFile)override;
    virtual void Load(FILE* pFile)override;
};


