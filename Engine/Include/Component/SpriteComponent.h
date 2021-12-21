#pragma once

#include "SceneComponent.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Mesh/SpriteMesh.h"

class CSpriteComponent :
	public CSceneComponent
{
	friend class CGameObject;

protected:
	CSpriteComponent();
	CSpriteComponent(const CSpriteComponent& com);
	virtual ~CSpriteComponent() override;

protected:
	CSharedPtr<CSpriteMesh>             m_Mesh;
	CSharedPtr<CMaterial>               m_Material;
	class CAnimationSequence2DInstance* m_Animation;


public:
	CMaterial* GetMaterial() const
	{
		return m_Material;
	}

	std::string GetTextureName(int Index = 0) const
	{
		return m_Material->GetTextureName(Index);
	}

	class CTexture* GetTexture(int Index = 0) const
	{
		return m_Material->GetTexture(Index);
	}

public:
	void SetMaterial(CMaterial* Material);
public:
	void SetBaseColor(const Vector4& Color);
	void SetBaseColor(float r, float g, float b, float a);
	void SetRenderState(class CRenderState* State);
	void SetRenderState(const std::string& Name);
	void SetTransparency(bool Enable);
	void SetOpacity(float Opacity);
	void AddOpacity(float Opacity);

public:
	void AddTexture(int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
	void AddTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	                const std::string& PathName = TEXTURE_PATH);
	void AddTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
	void AddTexture(int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName,
	                const std::string& PathName = TEXTURE_PATH);

	void SetTexture(int Index, class CTexture* Texture);
	void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
	void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	                const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
	void SetTexture(int                        Index, int Register, int ShaderType, const std::string& Name,
	                const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);

public:
	virtual void              Start() override;
	virtual bool              Init() override;
	virtual void              Update(float DeltaTime) override;
	virtual void              PostUpdate(float DeltaTime) override;
	virtual void              PrevRender() override;
	virtual void              Render() override;
	virtual void              PostRender() override;
	virtual CSpriteComponent* Clone() override;

public:
	template <typename T>
	void CreateAnimationInstance()
	{
		T* Anim = new T;

		Anim->SetScene(m_Scene);
		Anim->SetOwner(this);

		if (!Anim->Init())
		{
			SAFE_DELETE(Anim);
			return;
		}

		SAFE_DELETE(m_Animation);

		m_Animation = Anim;
	}
};
