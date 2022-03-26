#pragma once
#include "GameObject\GameObject.h"

class  CBombKirbyThrowBomb :
    public CGameObject
{
    friend class CBombKirbyState;
    friend class CScene;
protected:
	CBombKirbyThrowBomb();
	virtual ~CBombKirbyThrowBomb();
private:
    CSharedPtr<class CSceneComponent> m_Root;
    CSharedPtr<class CSpriteComponent> m_Bomb;
    bool m_IsSetRight;
    float m_RotateAmount;
    bool m_RotateComplete;
protected:
    void SetLeft();
    void SetRight();
protected:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
};

