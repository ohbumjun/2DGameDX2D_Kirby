#pragma once

#include "../../Ref.h"

class CSound :
    public CRef
{
    friend class CSoundManager;
protected :
    CSound();
    CSound(const CSound& Sound);
    ~CSound();
private :
    std::string m_Name;
    FMOD::System* m_System;
    FMOD::ChannelGroup* m_Group;
    FMOD::Channel* m_Channel;
    FMOD::Sound* m_Sound;
    bool m_IsLoop;
    bool m_IsPlay;
public :
    bool LoadSound(const std::string& Name, FMOD::System* System, FMOD::ChannelGroup* Group,
        bool Loop, const TCHAR* FileName, const std::string& PathName = SOUND_PATH);
    void Play();
    void Stop();
    void Resume();
    void Pause();
    void SetVolume(float Volume);
};

