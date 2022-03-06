#pragma once
#include "GameObject\LifeObject.h"

class CMonster :
    public CLifeObject
{
public:
    CMonster();
    CMonster(const CMonster& Monster);
    virtual ~CMonster();
private:
    CSharedPtr<class CSpriteComponent> m_Sprite;
    CSharedPtr<class CColliderCircle> m_ColliderBody;
    CSharedPtr<class CPaperBurnComponent> m_PaperBurn;
    // CSharedPtr<class CColliderComponent> m_ColliderBody;
    float m_HPMax;
    float m_HP;
    float m_DeathAccTime;
    float m_DeathFinishTime;
    bool m_DeathStart;
public:
    void LoadAnimationInstance(const std::string& Name, const TCHAR* FileName, const std::string& PathName = ENGINE_ANIMATION_PATH);
    void SetCurrentAnimation(const std::string& Name);
public:
    void SetHPMax(float HPMax);
    virtual void AIIdle(float DeltaTime);
    virtual void AIWalk(float DeltaTime);
    virtual void AIITrace(float DeltaTime, Vector3 PlayerPos);
    virtual void AIAttack(float DeltaTime, Vector3 PlayerPos);
    virtual void AIDeath(float DeltaTime);
    virtual void AIIHit(float DeltaTime);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CMonster* Clone() override;
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};





