#pragma once
#include "GameObject\GameObject.h"

class CBeamMonsterAttack :
    public CGameObject
{
public :
    CBeamMonsterAttack();
    CBeamMonsterAttack(const CBeamMonsterAttack& Attack);
    virtual ~CBeamMonsterAttack() override;
private:
    CSharedPtr<class CSceneComponent> m_Root;
    CSharedPtr<class CSpriteComponent> m_FirstSprite;
    CSharedPtr<class CSpriteComponent> m_SecondSprite;
    CSharedPtr<class CSpriteComponent> m_ThirdSprite;
    CSharedPtr<class CSpriteComponent> m_FourthSprite;
    float m_AttackImageSize;
public :
    void SetRightAttackDir();
    void SetLeftAttackDir();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
};

