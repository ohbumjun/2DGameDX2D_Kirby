#include "Sound.h"
#include "../../PathManager.h"

CSound::CSound() :
	m_IsPlay(false)
{}

CSound::CSound(const CSound& Sound)
{}

CSound::~CSound()
{}

bool CSound::LoadSound(const std::string& Name, FMOD::System* System, FMOD::ChannelGroup* Group,
	bool Loop, const TCHAR* FileName, const std::string& PathName)
{
	m_Name = Name;
	m_System = System;
	m_Group = Group;
	m_IsLoop = Loop;

	TCHAR FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);
	if (Info)
		lstrcpy(FullPath, Info->Path);
	lstrcat(FullPath, FileName);

	FMOD_MODE Mode = FMOD_DEFAULT;
	if (Loop)
		Mode = FMOD_LOOP_NORMAL;

	char FullPathMultibyte[MAX_PATH] = {};
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, ConvertLength, 0, 0);

	HRESULT result = m_System->createSound(FullPathMultibyte, Mode, nullptr, &m_Sound);
	if (result != FMOD_OK)
		return false;

	return true;
}

void CSound::Play()
{
	m_System->playSound(m_Sound, m_Group, false, &m_Channel);
	m_IsPlay = true;
}

void CSound::Stop()
{
	if (!m_Channel)
		return;
	bool Playing = false;
	m_Channel->isPlaying(&Playing);
	if (Playing)
	{
		m_Channel->stop();
		m_Channel = nullptr;
	}
	m_IsPlay = false;
}

void CSound::Resume()
{
	if (!m_Channel)
		return;
	bool Playing = false;
	m_Channel->isPlaying(&Playing);
	if (!Playing)
	{
		m_Channel->setPaused(false);
	}
	m_IsPlay = true;
}

void CSound::Pause()
{
	if (!m_Channel)
		return;
	bool Playing = false;
	m_Channel->isPlaying(&Playing);
	if (Playing)
	{
		m_Channel->setPaused(true);
	}
	m_IsPlay = false;
}
