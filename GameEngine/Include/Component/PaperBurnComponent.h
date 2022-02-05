#pragma once
#include "ObjectComponent.h"
#include "../Resource/Shader/PaperBurnConstantBuffer.h"
class CPaperBurnComponent :
    public CObjectComponent
{
protected:
	CPaperBurnComponent();
	CPaperBurnComponent(const CPaperBurnComponent& Component);
	virtual ~CPaperBurnComponent() override;
private :
    CPaperBurnConstantBuffer* m_CBuffer;
public :
public:
	virtual void             Start() override;
	virtual bool             Init() override;
	virtual void             Update(float DeltaTime) override;
	virtual void             PostUpdate(float DeltaTime) override;
	virtual void             PrevRender() override;
	virtual void             Render() override;
	virtual void             PostRender() override;
	virtual CPaperBurnComponent* Clone() override;
};

