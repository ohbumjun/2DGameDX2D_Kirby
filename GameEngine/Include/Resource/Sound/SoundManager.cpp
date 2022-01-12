#include "SoundManager.h"

CSoundManager::CSoundManager()
{}

CSoundManager::~CSoundManager()
{
	// Group 해제
	auto iter = m_mapChannelGroup.begin();
	auto iterEnd = m_mapChannelGroup.end();
	for (; iter != iterEnd; ++iter)
	{
		iter->second->release();
	}

	// Sound 해제
	m_mapSound.clear();

	// System 해제
	if (m_System)
	{
		m_System->close();
		m_System->release();
	}
}

bool CSoundManager::Init()
{
	HRESULT result = FMOD::System_Create(&m_System);
	if (result != FMOD_OK)
		return false;

	result = m_System->init(1000, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
		return false;

	m_System->getMasterChannelGroup(&m_MasterGroup);
	if (result != FMOD_OK)
		return false;

	m_mapChannelGroup.insert(std::make_pair("Master", m_MasterGroup));

	return true;
}

void CSoundManager::Update()
{
	m_System->update();
}

CSound* CSoundManager::LoadSound(const std::string& GroupName, 
								 const std::string& SoundName, bool Loop, const TCHAR* FileName,
								 const std::string& PathName)
{
	CSound* Sound = FindSound(SoundName);
	if (Sound)
		return Sound;

	FMOD::ChannelGroup* Group = FindGroup(GroupName);
	if (!Group)
		return nullptr;

	Sound = new CSound;
	if (!Sound->LoadSound(SoundName, m_System, Group, true, FileName, PathName))
		return nullptr;

	m_mapSound.insert(std::make_pair(SoundName, Sound));

	return Sound;
}

void CSoundManager::ReleaseSound(const std::string& SoundName)
{
	auto iter = m_mapSound.find(SoundName);
	if (iter == m_mapSound.end())
		return;
	if (iter->second->GetRefCount() == 1)
	{
		m_mapSound.erase(iter);
	}
}

CSound* CSoundManager::FindSound(const std::string& SoundName)
{
	auto iter = m_mapSound.find(SoundName);
	if (iter == m_mapSound.end())
		return nullptr;
	return iter->second;
}

FMOD::ChannelGroup* CSoundManager::FindGroup(const std::string& GroupName)
{
	auto iter = m_mapChannelGroup.find(GroupName);
	if (iter == m_mapChannelGroup.end())
		return nullptr;
	return iter->second;
}

void CSoundManager::SoundPlay(const std::string& SoundName)
{
	CSound* Sound = FindSound(SoundName);
	if (!Sound)
		return;
	Sound->Play();
}

void CSoundManager::SoundStop(const std::string& SoundName)
{
	CSound* Sound = FindSound(SoundName);
	if (!Sound)
		return;
	Sound->Stop();
}

void CSoundManager::SoundResume(const std::string& SoundName)
{
	CSound* Sound = FindSound(SoundName);
	if (!Sound)
		return;
	Sound->Resume();
}

void CSoundManager::SoundPause(const std::string& SoundName)
{
	CSound* Sound = FindSound(SoundName);
	if (!Sound)
		return;
	Sound->Pause();
}

void CSoundManager::SetVolume(const std::string& SoundName, float Volume)
{
	CSound* Sound = FindSound(SoundName);
	if (!Sound)
		return;
	Sound->SetVolume(Volume);
}

void CSoundManager::SetEntireVolume(float Volume)
{
	if (Volume > 100.f)
		Volume = 99.9f;
	m_MasterGroup->setVolume(Volume / 100.f);
}
