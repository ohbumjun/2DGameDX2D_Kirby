#include "SceneResource.h"

CSceneResource::CSceneResource()
{
}

CSceneResource::~CSceneResource()
{
	{
		auto iter    = m_mapMesh.begin();
		auto iterEnd = m_mapMesh.end();

		for (; iter != iterEnd;)
		{
			std::string Name = iter->first;

			iter = m_mapMesh.erase(iter);

			CResourceManager::GetInst()->ReleaseMesh(Name);
		}
	}

	{
		auto iter    = m_mapShader.begin();
		auto iterEnd = m_mapShader.end();

		for (; iter != iterEnd;)
		{
			std::string Name = iter->first;

			iter = m_mapShader.erase(iter);

			CResourceManager::GetInst()->ReleaseShader(Name);
		}
	}

	{
		auto iter    = m_mapTexture.begin();
		auto iterEnd = m_mapTexture.end();

		for (; iter != iterEnd;)
		{
			std::string Name = iter->first;

			iter = m_mapTexture.erase(iter);

			CResourceManager::GetInst()->ReleaseTexture(Name);
		}
	}

	{
		auto iter    = m_mapMaterial.begin();
		auto iterEnd = m_mapMaterial.end();

		for (; iter != iterEnd;)
		{
			std::string Name = iter->first;

			iter = m_mapMaterial.erase(iter);

			CResourceManager::GetInst()->ReleaseMaterial(Name);
		}
	}

	{
		auto iter    = m_mapSequence2D.begin();
		auto iterEnd = m_mapSequence2D.end();

		for (; iter != iterEnd;)
		{
			std::string Name = iter->first;

			iter = m_mapSequence2D.erase(iter);

			CResourceManager::GetInst()->ReleaseAnimationSequence2D(Name);
		}
	}

	// Sound 처리
	m_mapSound.clear();
}

CMesh* CSceneResource::FindMesh(const std::string& Name)
{
	auto iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
	{
		CMesh* Mesh = CResourceManager::GetInst()->FindMesh(Name);

		if (!Mesh)
			return nullptr;

		m_mapMesh.insert(std::make_pair(Name, Mesh));

		return Mesh;
	}

	return iter->second;
}

CShader* CSceneResource::FindShader(const std::string& Name)
{
	auto iter = m_mapShader.find(Name);

	if (iter == m_mapShader.end())
	{
		CShader* Shader = CResourceManager::GetInst()->FindShader(Name);

		if (!Shader)
			return nullptr;

		m_mapShader.insert(std::make_pair(Name, Shader));

		return Shader;
	}

	return iter->second;
}

CMaterial* CSceneResource::FindMaterial(const std::string& Name)
{
	auto iter = m_mapMaterial.find(Name);

	if (iter == m_mapMaterial.end())
	{
		CMaterial* Material = CResourceManager::GetInst()->FindMaterial(Name);

		if (!Material)
			return nullptr;

		m_mapMaterial.insert(std::make_pair(Name, Material));

		return Material;
	}

	return iter->second;
}

bool CSceneResource::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return false;

	m_mapTexture.insert(std::make_pair(Name, CResourceManager::GetInst()->FindTexture(Name)));

	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
		return false;

	m_mapTexture.insert(std::make_pair(Name, CResourceManager::GetInst()->FindTexture(Name)));

	return true;
}

bool CSceneResource::LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
	const std::string& PathName)
{
	if (FindTexture(Name))
		return true;
	if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
		return false;

	m_mapTexture.insert(std::make_pair(Name, CResourceManager::GetInst()->FindTexture(Name)));

	return true;
}

CTexture* CSceneResource::FindTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
	{
		CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

		if (!Texture)
			return nullptr;

		m_mapTexture.insert(std::make_pair(Name, Texture));

		return Texture;
	}

	return iter->second;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& Name,
                                               const std::string& TextureName, const TCHAR* FileName,
                                               const std::string& PathName)
{
	if (FindAnimationSequence2D(Name))
		return true;

	if (!CResourceManager::GetInst()->CreateAnimationSequence2D(Name, TextureName, FileName, PathName))
		return false;

	m_mapSequence2D.insert(std::make_pair(Name, CResourceManager::GetInst()->FindAnimationSequence2D(Name)));

	return true;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& Name, CTexture* Texture)
{
	if (FindAnimationSequence2D(Name))
		return true;

	if (!CResourceManager::GetInst()->CreateAnimationSequence2D(Name, Texture))
		return false;

	m_mapSequence2D.insert(std::make_pair(Name, CResourceManager::GetInst()->FindAnimationSequence2D(Name)));

	return true;
}

void CSceneResource::AddSequence2D(CAnimationSequence2D* Sequence)
{
	CAnimationSequence2D* Anim = FindAnimationSequence2D(Sequence->GetName());

	if (Anim)
		return;

	// 여기서 ResourceManager 에 까지 넣어줄 필요는 없는 것일까 ?

	m_mapSequence2D.insert(std::make_pair(Sequence->GetName(), Sequence));
}

bool CSceneResource::AddSequence2D(const std::string& SequenceName, CTexture* Texture)
{
	CAnimationSequence2D* Sequence2D = FindAnimationSequence2D(SequenceName);
	if (Sequence2D)
		return true;

	Sequence2D = new  CAnimationSequence2D;
	Sequence2D->m_Texture = Texture;

	m_mapSequence2D.insert(std::make_pair(SequenceName, Sequence2D));

	return true;
}


void CSceneResource::AddAnimationSequence2DFrame(const std::string& Name, const Vector2& Start,
												 const Vector2&     Size)
{
	CAnimationSequence2D* Anim = FindAnimationSequence2D(Name);

	if (!Anim)
		return;

	Anim->AddFrame(Start, Size);
}

void CSceneResource::AddAnimationSequence2DFrame(const std::string& Name, float   StartX,
                                                 float              StartY, float Width, float Height)
{
	CAnimationSequence2D* Anim = FindAnimationSequence2D(Name);

	if (!Anim)
		return;

	Anim->AddFrame(StartX, StartY, Width, Height);
}

CAnimationSequence2D* CSceneResource::FindAnimationSequence2D(const std::string& Name)
{
	auto iter = m_mapSequence2D.find(Name);

	if (iter == m_mapSequence2D.end())
	{
		CAnimationSequence2D* Anim = CResourceManager::GetInst()->FindAnimationSequence2D(Name);

		if (!Anim)
			return nullptr;

		m_mapSequence2D.insert(std::make_pair(Name, Anim));

		return Anim;
	}

	return iter->second;
}

CAnimation2DConstantBuffer* CSceneResource::GetAnimation2DCBuffer() const
{
	return CResourceManager::GetInst()->GetAnimation2DCBuffer();
}

bool CSceneResource::LoadSequence2DFullPath(const char* FullPath)
{
	std::string SequenceName;
	if (CResourceManager::GetInst()->LoadSequence2DFullPath(SequenceName, FullPath))
		return false;

	if (FindAnimationSequence2D(SequenceName))
		return true;

	CAnimationSequence2D* sequence2D = CResourceManager::GetInst()->FindAnimationSequence2D(SequenceName);

	m_mapSequence2D.insert(std::make_pair(SequenceName, sequence2D));

	return true;
}

bool CSceneResource::LoadSequence2DFullPath(std::string& SequenceName, const char* FullPath)
{
	if (!CResourceManager::GetInst()->LoadSequence2DFullPath(SequenceName, FullPath, m_Scene))
		return false;

	if (FindAnimationSequence2D(SequenceName))
		return true;

	CAnimationSequence2D* sequence2D = CResourceManager::GetInst()->FindAnimationSequence2D(SequenceName);

	m_mapSequence2D.insert(std::make_pair(SequenceName, sequence2D));

	return true;
}

bool CSceneResource::LoadSequence2D(std::string& SequenceName, const char* FileName, const std::string PathName)
{
	if (!CResourceManager::GetInst()->LoadSequence2D(SequenceName, FileName, PathName, m_Scene))
		return false;

	if (FindAnimationSequence2D(SequenceName))
		return true;

	CAnimationSequence2D* sequence2D = CResourceManager::GetInst()->FindAnimationSequence2D(SequenceName);

	m_mapSequence2D.insert(std::make_pair(SequenceName, sequence2D));

	return true;
}

bool CSceneResource::LoadSequence2D(const char* FileName, const std::string& PathName)
{
	std::string SequenceName;

	if (!CResourceManager::GetInst()->LoadSequence2D(SequenceName, FileName, PathName, m_Scene))
		return false;

	if (FindAnimationSequence2D(SequenceName))
		return true;

	CAnimationSequence2D* Sequence2D = CResourceManager::GetInst()->FindAnimationSequence2D(SequenceName);

	m_mapSequence2D.insert(std::make_pair(SequenceName, Sequence2D));

	return true;
}

void CSceneResource::EditSequence2DName(const std::string& PrevName, const std::string& NewName)
{
	auto iter = m_mapSequence2D.find(PrevName);

	if (iter == m_mapSequence2D.end())
		return;

	CAnimationSequence2D* Sequence2D = iter->second;
	if (!Sequence2D)
		return;

	CAnimationSequence2D* NewSequence2D = Sequence2D->Clone();
	NewSequence2D->SetName(NewName); // 실제 Sequence의 이름도 바꿔준다.

	m_mapSequence2D.erase(iter);

	m_mapSequence2D.insert(std::make_pair(NewName, Sequence2D));
}

CSound* CSceneResource::LoadSound(const std::string& GroupName, const std::string& SoundName, bool Loop,
								  const TCHAR* FileName, const std::string& PathName)
{
	CSound* Sound = FindSound(SoundName);
	if (Sound)
		return Sound;

	Sound = CResourceManager::GetInst()->LoadSound(GroupName, SoundName, Loop, FileName, PathName);

	if (!Sound)
		return nullptr;

	m_mapSound.insert(std::make_pair(SoundName, Sound));

	return Sound;
}

CSound* CSceneResource::FindSound(const std::string& SoundName)
{
	auto iter = m_mapSound.find(SoundName);

	if (iter == m_mapSound.end())
	{
		CSound* Sound = CResourceManager::GetInst()->FindSound(SoundName);

		if (!Sound)
			return nullptr;

		m_mapSound.insert(std::make_pair(SoundName, Sound));

		return Sound;
	}

	return iter->second;
}

FMOD::ChannelGroup* CSceneResource::FindGroup(const std::string& GroupName)
{
	return CResourceManager::GetInst()->FindGroup(GroupName);
}

void CSceneResource::SoundPlay(const std::string& SoundName)
{
	CResourceManager::GetInst()->SoundPlay(SoundName);
}

void CSceneResource::SoundStop(const std::string& SoundName)
{
	CResourceManager::GetInst()->SoundStop(SoundName);
}

void CSceneResource::SoundResume(const std::string& SoundName)
{
	CResourceManager::GetInst()->SoundResume(SoundName);
}

void CSceneResource::SoundPause(const std::string& SoundName)
{
	CResourceManager::GetInst()->SoundPause(SoundName);
}

void CSceneResource::SetVolume(const std::string& SoundName, float Volume)
{
	CResourceManager::GetInst()->SetVolume(SoundName, Volume);
}

void CSceneResource::SetEntireVolume(float Volume)
{
	CResourceManager::GetInst()->SetEntireVolume(Volume); 
}

void CSceneResource::SetGroupVolume(const std::string& GroupName, float Volume)
{
	CResourceManager::GetInst()->SetGroupVolume(GroupName, Volume); 
}

bool CSceneResource::CreateParticle(const std::string& Name)
{
	CParticle* Particle = FindParticle(Name);
	if (Particle)
		return true;

	if (!CResourceManager::GetInst()->CreateParticle(Name))
	{
		return false;
	}

	Particle = CResourceManager::GetInst()->FindParticle(Name);
	m_mapParticle.insert(std::make_pair(Name, Particle));

	return true;
}

CParticle* CSceneResource::FindParticle(const std::string& Name)
{
	auto iter = m_mapParticle.find(Name);

	if (iter == m_mapParticle.end())
	{
		CParticle* Particle = CResourceManager::GetInst()->FindParticle(Name);

		if (!Particle)
			return nullptr;

		m_mapParticle.insert(std::make_pair(Name, Particle));

		return Particle;
	}

	return iter->second;
}

