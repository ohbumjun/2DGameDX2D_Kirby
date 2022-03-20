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
protected: 
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

    // Player 에게 끌어당겨지기 + 나오기
    bool m_IsBeingPulled;
    float m_BeginPulledAccel;
    float m_BeginPulledAccelSum;
    Vector3 m_PulledDestPos;
    bool m_IsBeingSpitOut;
    float m_SpitOutDistance;
    float m_SpitOutDistanceMax;

    // AI
    Monster_AI m_AI;
    float m_AttackDistance;
    float m_DashDistance;

    // Hit
    bool m_IsBeingHit;
    float m_HitLimitTime;
    float m_HitLimitTimeMax;

    // Move
    float m_MonsterMoveVelocity;
    float m_RandomMoveTime;
    float m_RandomMoveTimeMax;

    // Monster Type
    Monster_Type m_MonsterType;

public :
    Monster_Type GetMonsterType() const
{
        return m_MonsterType;
}
    Monster_AI GetAIState() const
{
        return m_AI;
}
    bool IsBeingPulled() const
{
        return m_IsBeingPulled;
}
    bool IsBeingHit () const
{
        return m_IsBeingHit;
}
public :
    void SetAIState(Monster_AI State)
{
        m_AI = State;

		if (State == Monster_AI::Hit)
		{
            m_IsBeingHit = true;
		}
}
    void SetBeingSpitOut(bool Enable)
{
        m_IsBeingSpitOut = Enable;
}
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
protected:
    void AIIdle(float DeltaTime);
    void AIWalk(float DeltaTime);
    void AITrace(float DeltaTime, Vector3 PlayerPos);
    void AIAttack(float DeltaTime, Vector3 PlayerPos);
    void AIDeath(float DeltaTime);
    void AIHit(float DeltaTime);
    virtual void AIIdleSpecific(float DeltaTime){};
    virtual void AIWalkSpecific(float DeltaTime){};
    virtual void AITraceSpecific(float DeltaTime){};
    virtual void AIAttackSpecific(float DeltaTime){}
    virtual void AIDeathSpecific(float DeltaTime){};
    virtual void AIHitSpecific(float DeltaTime){};
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime)override;
    virtual void PostUpdate(float DeltaTime)override;
    virtual CMonster* Clone() override;
protected:
    void UpdateBeingHit(float DeltaTime);
    void UpdateBeingPulled(float DeltaTime);
    void UpdateBeingOutOfPlayer(float DeltaTime);
protected:
    void UpdateMonsterMove(float DeltaTime);
    void SetRandomTargetDir();
protected:
    void OnMouseBegin(const CollisionResult& Result);
    void OnMouseEnd(const CollisionResult& Result);
    void CreateDamageFont(const CollisionResult& Result);
    void OnCollisionBegin(const CollisionResult& Result);
protected: // Animation
    virtual void ChangeIdleAnimation();
    virtual void ChangeWalkAnimation();
    virtual void ChangeHitAnimation();
    virtual void ChangeTraceAnimation();
    virtual void ChangeDeathAnimation();
    virtual void ChangeAttackAnimation();
protected:
    void PaperBurnEnd();
public :
    virtual void Save(FILE* pFile)override;
    virtual void Load(FILE* pFile)override;
};


