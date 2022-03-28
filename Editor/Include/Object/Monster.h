#pragma once
#include "GameObject\LifeObject.h"

class CMonster :
    public CLifeObject
{
public:
    CMonster();
    CMonster(const CMonster& Monster);
    virtual ~CMonster() override;
protected:
    CSharedPtr<class CSpriteComponent> m_Sprite;
    CSharedPtr<class CColliderCircle> m_ColliderBody;
    CSharedPtr<class CPaperBurnComponent> m_PaperBurn;
    CSharedPtr<class CWidgetComponent> m_SimpleHUDWidget;
    float m_HPMax;
    float m_HP;
    float m_DeathAccTime;
    float m_DeathFinishTime;
    bool m_DeathStart;
public:
    void LoadAnimationInstance(const std::string& Name, const TCHAR* FileName, const std::string& PathName = ENGINE_ANIMATION_PATH);
    void SetCurrentAnimation(const std::string& Name);
protected :
    virtual void ChangeIdleAnimation(){}
    virtual void ChangeWalkAnimation(){}
    virtual void ChangeHitAnimation(){}
    virtual void ChangeTraceAnimation(){}
    virtual void ChangeDeathAnimation(){}
    virtual void ChangeAttackAnimation(){}
    virtual void Damage(float Damage){}
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual CMonster* Clone() override;
    virtual void Save(FILE* pFile) override;
    virtual void Load(FILE* pFile) override;
};





