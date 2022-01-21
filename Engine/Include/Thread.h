#pragma once

#include "GameInfo.h"

// 유저 모드 : 크리티컬 섹션 + 커널 모드 : 이벤트 동기화 기법
// <process.h>가 필요하다. 

class CThread
{
protected:
	CThread();
	virtual ~CThread(); // 상속용
private :
	std::string m_Name; // 디버깅의 편의를 위해 Thread를 담아둘 것이다.
	HANDLE m_Thread;  // 해당 Thread의 핸들값 --> Thread 식별에 사용됨 , 핸들 테이블에 저장
	HANDLE m_StartEvent; // 이벤트 동기화 기법 과정에서 사용될 Event의 핸들값 
public :
	bool Init();
	virtual void Run();
public :
	// Thread의 상태를 직접 관리해주기 위한 함수 ( 사실 거의 안쓰긴 한다 )
	void Start();
	void Pause();
	void Resume();
};

