#pragma once
#include "BossMonster.h"
class CBossTree :
    public CBossMonster
{
    friend class CScene;
protected:
    CBossTree();
    CBossTree(const CBossTree& Monster);
    virtual ~CBossTree() override;
private :
    float m_FarAttackLimitTime;
    float m_FarAttackLimitTimeMax;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBossTree* Clone() override;
private:
    void FarAttack();
    void CloseAttack();
    void ChangeFarAttackAnimation();
    void ChangeCloseAttackAnimation();
    void ChangeJumpAttackAnimation();
    void ChangeJumpEndAttackAnimation();
    void ChangeToIdleAfterHit();
protected:
    virtual void ChangeTraceAnimation() override;
private:
    virtual void AIAttackSpecific(float DeltaTime) override;
    virtual void AITraceSpecific(float DeltaTime) override;
};

