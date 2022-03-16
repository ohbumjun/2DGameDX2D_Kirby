#pragma once
#include "GameObject\GameObject.h"

class CBeamMonsterAttack :
    public CGameObject
{
    friend class CBeamMonster;
public :
    CBeamMonsterAttack();
    CBeamMonsterAttack(const CBeamMonsterAttack& Attack);
    virtual ~CBeamMonsterAttack() override;
private :
    class CBeamMonster* m_BeamOwner;
private:
    CSharedPtr<class CSpriteComponent> m_FirstSprite;
    CSharedPtr<class CSpriteComponent> m_SecondSprite;
    CSharedPtr<class CSpriteComponent> m_ThirdSprite;
    CSharedPtr<class CSpriteComponent> m_FourthSprite;
    float m_AttackImageSize;
    float m_RotateLimit;
protected:
    void SetBeamOwner(class CBeamMonster* Owner)
    {
        m_BeamOwner = Owner;
    }
protected:
    void SetRightAttackDir();
    void SetLeftAttackDir();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
};

