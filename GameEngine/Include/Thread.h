#pragma once


#include "GameInfo.h"

// 이벤트 기반 only

class CThread {
protected :
	CThread();
	virtual ~CThread();
private :
	HANDLE m_Thread;
	HANDLE m_StartEvent;
	std::string m_Name;
public :
	virtual bool Init();
	virtual void Run();
public :
	void Resume();
	void Pause();
	void Start();
public :
	void WaitStartEvent();
	static unsigned int __stdcall ThreadFunction(void* Arg);
};