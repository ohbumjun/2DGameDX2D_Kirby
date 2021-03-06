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
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBossTree* Clone() override;
private :
    float m_SceneChangeLimitTime;
private:
    void FarAttack();
    void CloseAttack();
    void ChangeFarAttackAnimation();
    void ChangeCloseAttackAnimation();
    void UpdateSceneChangeLimitTime(float DeltaTime);
    void ChangeSceneToFloat1Scene();
protected:
    virtual void ChangeTraceAnimation() override;
private:
    virtual void AIAttackSpecific(float DeltaTime) override;
    virtual void AITraceSpecific(float DeltaTime) override;
    virtual void AIDeathSpecific(float DeltaTime) override;
};

