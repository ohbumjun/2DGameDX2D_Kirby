#pragma once
#include "AttackEffect.h"

class CBeamMonsterAttack :
    public CAttackEffect
{
    friend class CBeamMonster;
public :
    CBeamMonsterAttack();
    CBeamMonsterAttack(const CBeamMonsterAttack& Attack);
    virtual ~CBeamMonsterAttack() override;
private:
    CSharedPtr<class CSpriteComponent> m_SecondSprite;
    CSharedPtr<class CSpriteComponent> m_ThirdSprite;
    CSharedPtr<class CSpriteComponent> m_FourthSprite;
    float m_AttackImageSize;
    float m_RotationSpeed;
    float m_RotateLimit;
    float m_AttackDir;
protected:
    virtual void SetRightAttackDir(float YDir = 0.f) override;
    virtual void SetLeftAttackDir(float YDir = 0.f) override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
private :
    void CollisionCallback(const CollisionResult& Result);
};

