#pragma once
#include "SceneComponent.h"

class CParticleComponent :
    public CSceneComponent
{
public :
    CParticleComponent();
    CParticleComponent(const CParticleComponent& Component);
    virtual ~CParticleComponent();
public:
	virtual void             Start() override;
	virtual bool             Init() override;
	virtual void             Update(float DeltaTime) override;
	virtual void             PostUpdate(float DeltaTime) override;
	virtual void             PrevRender() override;
	virtual void             Render() override;
	virtual void CheckCollision();
	virtual void             PostRender() override;
	virtual CParticleComponent* Clone() override;
};

