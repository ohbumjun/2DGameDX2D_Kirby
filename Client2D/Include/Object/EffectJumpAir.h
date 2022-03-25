#pragma once
#include "GameObject\GameObject.h"

class CEffectJumpAir :
    public CGameObject
{
	friend class CScene;
protected:
	CEffectJumpAir();
	virtual ~CEffectJumpAir() override;
private:
	CSharedPtr<class CSpriteComponent> m_Sprite;
	float m_MoveDir;
public :
	void SetRightEffect();
	void SetLeftEffect();
public:
	virtual bool     Init() override;
	virtual void     Update(float DeltaTime) override;
	virtual void     PostUpdate(float DeltaTime) override;
};

