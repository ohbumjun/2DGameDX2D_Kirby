#pragma once

#include "../../GameInfo.h"
#include "Sound.h"

class CSoundManager
{
	friend class CResourceManager;

	// SingleTon
private :
	CSoundManager();
	~CSoundManager();
private :
	FMOD::System* m_System;
	FMOD::ChannelGroup* m_MasterGroup;
	std::unordered_map<std::string, CSharedPtr<CSound>> m_mapSound;
	std::unordered_map<std::string, FMOD::ChannelGroup*> m_mapChannelGroup;
public :
	bool Init();
	void Update();
public :
	CSound* LoadSound(const std::string& GroupName, const std::string& SoundName,
		bool Loop, const TCHAR* FileName,
		const std::string& PathName = SOUND_PATH);
	void ReleaseSound(const std::string& SoundName);
	CSound* FindSound(const std::string& SoundName);
	FMOD::ChannelGroup* FindGroup(const std::string& GroupName);
	void SoundPlay(const std::string& SoundName);
	void SoundStop(const std::string& SoundName);
	void SoundResume(const std::string& SoundName);
	void SoundPause(const std::string& SoundName);
	void SetVolume(const std::string& SoundName, float Volume);
	void SetEntireVolume(float Volume);
private :
	static CSoundManager* m_Inst;
public :
	static bool GetInst()
	{
			if (!m_Inst)
				m_Inst = new CSoundManager;
			return m_Inst;
	}
	static void DestroyInst()
	{
		if (m_Inst)
		{
			SAFE_DELETE(m_Inst);
		}
	}
};

