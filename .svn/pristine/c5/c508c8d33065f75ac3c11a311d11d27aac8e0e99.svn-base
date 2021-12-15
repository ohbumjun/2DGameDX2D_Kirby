#pragma once

#include "../GameInfo.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Resource/ResourceManager.h"

class CSceneResource
{
	friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

private:
	class CScene* m_Scene;

private:
	std::unordered_map<std::string, CSharedPtr<CMesh>>		m_mapMesh;
	std::unordered_map<std::string, CSharedPtr<CShader>>	m_mapShader;
	std::unordered_map<std::string, CSharedPtr<CMaterial>>	m_mapMaterial;
	std::unordered_map<std::string, CSharedPtr<CTexture>>	m_mapTexture;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>>	m_mapSequence2D;

public:	// =================== Mesh =====================


	CMesh* FindMesh(const std::string& Name);


public:	// =================== Shader =====================
	template <typename T>
	bool CreateShader(const std::string& Name)
	{
		if (FindShader(Name))
			return true;

		if (!CResourceManager::GetInst()->CreateShader<T>(Name))
			return false;

		m_mapShader.insert(std::make_pair(Name, CResourceManager::GetInst()->FindShader(Name)));

		return true;
	}
	CShader* FindShader(const std::string& Name);


public:	// =================== Material =====================
	CMaterial* FindMaterial(const std::string& Name);

public:
	template <typename T>
	bool CreateMaterial(const std::string& Name)
	{
		if (FindMaterial(Name))
			return true;

		if (!CResourceManager::GetInst()->CreateMaterial<T>(Name))
			return false;

		m_mapMaterial.insert(std::make_pair(Name, CResourceManager::GetInst()->FindMaterial(Name)));

		return true;
	}

public:	// =================== Texture =====================
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	class CTexture* FindTexture(const std::string& Name);

public:	// =================== Sequence2D =====================
	bool CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName,
		const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	void AddAnimationSequence2DFrame(const std::string& Name, const Vector2& Start, const Vector2& Size);
	void AddAnimationSequence2DFrame(const std::string& Name, float StartX, float StartY, float Width, float Height);
	CAnimationSequence2D* FindAnimationSequence2D(const std::string& Name);
	class CAnimation2DConstantBuffer* GetAnimation2DCBuffer()	const;
};

