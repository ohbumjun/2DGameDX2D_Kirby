#pragma once
#include "GameObject\GameObject.h"

class CEffectWaterBlast :
    public CGameObject
{
	friend class CScene;
protected:
	CEffectWaterBlast();
	virtual ~CEffectWaterBlast() override;
private:
	CSharedPtr<class CSpriteComponent> m_Sprite;
public:
	virtual bool     Init() override;
	virtual void     Update(float DeltaTime) override;
	virtual void     PostUpdate(float DeltaTime) override;
};

