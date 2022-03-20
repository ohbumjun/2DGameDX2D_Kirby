#pragma once
#include "AttackEffect.h"

class CTornado :
    public CAttackEffect
{
    friend class CMiddleBossHammer;
public:
    CTornado();
    CTornado(const CTornado& Attack);
    virtual ~CTornado() override;
private:
    class CBossTree* m_BossTree;
protected:
    void SetFireOwner(class CBossTree* Owner)
    {
        m_BossTree = Owner;
    }
protected:
    virtual void SetRightAttackDir(float YDir = 0.f) override;
    virtual void SetLeftAttackDir(float YDir = 0.f) override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
private:
    void CollisionCallback(const CollisionResult& Result);
};

