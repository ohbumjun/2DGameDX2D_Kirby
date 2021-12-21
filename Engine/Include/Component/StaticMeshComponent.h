#pragma once
#include "SceneComponent.h"

class CStaticMeshComponent :
	public CSceneComponent
{
	friend class CGameObject;

public:
	CStaticMeshComponent();
	CStaticMeshComponent(const CStaticMeshComponent& Mesh);
	virtual ~CStaticMeshComponent() override;
private :
	CSharedPtr<class CStaticMesh> m_Mesh;
	CSharedPtr<class CMaterial>   m_Material;
public :
	class CStaticMesh* GetMesh() const
	{
		return m_Mesh;
	}

	class CMaterial* GetMaterial() const
	{
		return m_Material;
	}

public :
	void SetMesh(CStaticMesh* Mesh);
	void SetMesh(const std::string& Name);
	void SetMaterial(const CMaterial* Material);
public :
	void SetBaseColor(const Vector4& Color);
	void SetBaseColor(float r, float g, float b, float a);
	void SetRenderState(const std::string& Name);
	void SetRenderState(const class CRenderState* State);
	void SetTransparency(bool Enable);
	void SetOpacity(float Opacity);
	void AddOpacity(float Opacity);
public :
	void AddTexture(int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
	void AddTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	                const std::string& PathName = TEXTURE_PATH);
	void AddTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
	void AddTexture(int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName,
	                const std::string& PathName = TEXTURE_PATH);

public :
	void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
	void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	                const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
	void SetTexture(int                        Index, int Register, int ShaderType, const std::string& Name,
	                const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);

public :
	virtual bool                  Init() override;
	virtual void                  Start() override;
	virtual void                  Update(float DeltaTime) override;
	virtual void                  PostUpdate(float DeltaTime) override;
	virtual void                  PrevRender() override;
	virtual void                  Render() override;
	virtual void                  PostRender() override;
	virtual CStaticMeshComponent* Clone() override;
};
