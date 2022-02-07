#pragma once

#include "../GameInfo.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Sound/Sound.h"
#include "../Resource/Particle/Particle.h"

class CSceneResource
{
	friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

private:
	class CScene* m_Scene;

private:
	std::unordered_map<std::string, CSharedPtr<CMesh>>                m_mapMesh;
	std::unordered_map<std::string, CSharedPtr<CShader>>              m_mapShader;
	std::unordered_map<std::string, CSharedPtr<CMaterial>>            m_mapMaterial;
	std::unordered_map<std::string, CSharedPtr<CTexture>>             m_mapTexture;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>> m_mapSequence2D;
	std::unordered_map<std::string, CSharedPtr<CSound>> m_mapSound;
	std::unordered_map<std::string, CSharedPtr<CParticle>>		m_mapParticle;

public: // =================== Mesh =====================
	CMesh* FindMesh(const std::string& Name);

public: // =================== Shader =====================
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


public: // =================== Material =====================
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
	template<typename T>
	T* CreateMaterialEmpty()
	{
		return CResourceManager::GetInst()->CreateMaterialEmpty<T>();
	}

public: // =================== Texture =====================
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
	                 const std::string& PathName = TEXTURE_PATH);
	bool            LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	class CTexture* FindTexture(const std::string& Name);

public: // =================== Sequence2D =====================
	bool CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName,
	                               const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& Name, CTexture* Texture);
	auto AddSequence2D(class CAnimationSequence2D* Sequence) -> void;
	bool AddSequence2D(const std::string& SequenceName, CTexture* Texture);
	void AddAnimationSequence2DFrame(const std::string& Name, const Vector2& Start, const Vector2& Size);
	void AddAnimationSequence2DFrame(const std::string& Name, float StartX, float StartY, float Width, float Height);
	CAnimationSequence2D* FindAnimationSequence2DData(const std::string& Name);
	class CAnimation2DConstantBuffer* GetAnimation2DCBuffer() const;
	// Save Load 
	bool LoadSequence2DFullPath(const char* FullPath);
	bool LoadSequence2DFullPath(std::string& SequenceName, const char* FullPath);
	bool LoadSequence2D(std::string& SequenceName, const char* FileName, const std::string PathName = ANIMATION_PATH);
	bool LoadSequence2D(const char* FileName, const std::string& PathName = ANIMATION_PATH);
public :
	void EditSequence2DName(const std::string& PrevName, const std::string& NewName);
public: // =================== Sound =====================
	CSound* LoadSound(const std::string& GroupName, const std::string& SoundName,
		bool Loop, const TCHAR* FileName,
		const std::string& PathName = SOUND_PATH);
	CSound* FindSound(const std::string& SoundName);
	FMOD::ChannelGroup* FindGroup(const std::string& GroupName);
	void SoundPlay(const std::string& SoundName);
	void SoundStop(const std::string& SoundName);
	void SoundResume(const std::string& SoundName);
	void SoundPause(const std::string& SoundName);
	void SetVolume(const std::string& SoundName, float Volume);
	void SetEntireVolume(float Volume);
	void SetGroupVolume(const std::string& GroupName, float Volume);

public: // =================== Particle =====================
	bool CreateParticle(const std::string& Name);
	class CParticle* FindParticle(const std::string& Name);

};
