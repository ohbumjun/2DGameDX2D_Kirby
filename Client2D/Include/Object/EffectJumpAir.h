#pragma once
#include "GameObject\GameObject.h"

class CEffectJumpAir :
    public CGameObject
{
	friend class CScene;
	friend class CPlayer2D;
protected:
	CEffectJumpAir();
	virtual ~CEffectJumpAir() override;
private:
	CSharedPtr<class CSpriteComponent> m_Sprite;
	float m_MoveDir;
	float m_EffectMoveSpeed;
public :
	void SetRightEffect();
	void SetLeftEffect();
	void SetMoveVelocity(float Speed)
	{
		m_EffectMoveSpeed = Speed;
	}
public:
	virtual bool     Init() override;
	virtual void     Update(float DeltaTime) override;
	virtual void     PostUpdate(float DeltaTime) override;
};

