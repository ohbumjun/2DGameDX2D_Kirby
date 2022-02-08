#pragma once
#include "ObjectComponent.h"
#include "../Resource/Shader/PaperBurnConstantBuffer.h"
#include "../Resource/Texture/Texture.h"

class CPaperBurnComponent :
	public CObjectComponent
{
	friend class CGameObject;
protected:
	CPaperBurnComponent();
	CPaperBurnComponent(const CPaperBurnComponent& Component);
	virtual ~CPaperBurnComponent() override;
private:
	CPaperBurnConstantBuffer* m_CBuffer;
	std::function<void()> m_FinishCallback;
	float m_FinishTime;
	CSharedPtr<class CMaterial> m_Material;
	CSharedPtr<CTexture> m_BurnTexture;
	bool m_StartPaperBurn;
	float m_Filter;
public:
	void StartPaperBurn();
	void SetMaterial(class CMaterial*);
	void SetInverse(bool Enable);
	void SetFinishTime(float Time)
	{
		m_FinishTime = Time;
	}
	void SetBurnTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetOutLineColor(const Vector4& Color);
	void SetOutLineColor(float r, float g, float b, float a);
	void SetCenterLineColor(const Vector4& Color);
	void SetCenterLineColor(float r, float g, float b, float a);
	void SetInLineColor(const Vector4& Color);
	void SetInLineColor(float r, float g, float b, float a);
	void SetInFilter(float Filter);
	void SetCenterFilter(float Filter);
	void SetOutFilter(float Filter);
public:
	void SetShader();
public:
	template<typename T>
	void SetFinishCallback(T* Obj, void(T::* Func)())
	{
		m_FinishCallback = std::bind(Func, Obj);
	}

public:
	virtual void              Start() override;
	virtual bool              Init() override;
	virtual void              Update(float DeltaTime) override;
	virtual void              PostUpdate(float DeltaTime) override;
	virtual void              PrevRender() override;
	virtual void              Render() override;
	virtual void              PostRender() override;
	virtual CPaperBurnComponent* Clone() override;
};

