#pragma once

#include "Animation/AnimationManager.h"
#include "Material/MaterialManager.h"
#include "Mesh/MeshManager.h"
#include "Shader/ShaderManager.h"
#include "Texture/TextureManager.h"

class CResourceManager
{
private:
	CMeshManager*      m_MeshManager;
	CShaderManager*    m_ShaderManager;
	CMaterialManager*  m_MaterialManager;
	CTextureManager*   m_TextureManager;
	CAnimationManager* m_AnimationManager;

public:
	bool Init();

public: // =================== Mesh =====================


	class CMesh* FindMesh(const std::string& Name);

	void ReleaseMesh(const std::string& Name);


public: // =================== Shader =====================
	template <typename T>
	bool CreateShader(const std::string& Name)
	{
		return m_ShaderManager->CreateShader<T>(Name);
	}

	class CShader* FindShader(const std::string& Name);

	void ReleaseShader(const std::string& Name);

	bool CreateConstantBuffer(const std::string& Name, int Size, int Register,
	                          int ConstantBufferShaderType = static_cast<int>(ConstantBuffer_Shader_Type::All));

	class CConstantBuffer* FindConstantBuffer(const std::string& Name);


public: // =================== Material =====================
	CMaterial* FindMaterial(const std::string& Name);

	void ReleaseMaterial(const std::string& Name);

public:
	template <typename T>
	bool CreateMaterial(const std::string& Name)
	{
		return m_MaterialManager->CreateMaterial<T>(Name);
	}
	template <typename T>
	T* CreateMaterialEmpty()
	{
		return m_MaterialManager->CreateMaterialEmpty<T>();
	}

public: // =================== Texture =====================
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
	                 const std::string& PathName = TEXTURE_PATH);
	bool            LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	class CTexture* FindTexture(const std::string& Name);
	void            ReleaseTexture(const std::string& Name);

public: // =================== Sequence2D =====================
	bool CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName,
	                               const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& Name, CTexture* Texture);
	void AddAnimationSequence2DFrame(const std::string& Name, const Vector2& Start, const Vector2& Size);
	void AddAnimationSequence2DFrame(const std::string& Name, float StartX, float StartY, float Width, float Height);
	CAnimationSequence2D* FindAnimationSequence2D(const std::string& Name);
	void ReleaseAnimationSequence2D(const std::string& Name);
	auto GetAnimation2DCBuffer() const -> class CAnimation2DConstantBuffer*;

	bool AddSequence2D(CAnimationSequence2D* Sequence2D);
	bool LoadSequence2DFullPath(const char* FullPath, const class CScene* Scene = nullptr);
	bool LoadSequence2DFullPath(std::string& SequenceName, const char* FullPath, const class CScene* Scene = nullptr);
	bool LoadSequence2D(std::string& SequenceName, const char* FileName, const std::string& PathName = TEXTURE_PATH ,const class CScene* Scene = nullptr);

	DECLARE_SINGLE(CResourceManager);
};
