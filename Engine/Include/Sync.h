#pragma once

#include "GameInfo.h"

// 크리티컬 섹션 동기화 방식을 사용하려면
// 크리티컬 섹션 오브젝트를 만들고, 초기화 해야 한다.
// CRITICAL_SECTION 변수가 곧 크리티컬 섹션기반 동기화 기법이고, 화장실 키의 개념

// void InitializeCriticalSection(); 함수를 통해 외부에서 초기화 과정이 있었을 것이고
// 이 과정으로 만들어낸 CRITICAL_SECTION 을 CSync 객체에 생성자 인자로 전달해주는 것이다.

// EnterCriticalSection 가 곧 임계영역에 들어가는 것,
// LeaveCriticalSection 가 곧 임계 영역을 나오는 것
// 생성자와 소멸자에 세팅해두게 되면, 임계영역에 나올 때 LeaveCriticalSection를
// 반드시 해주므로 DeadLock에 걸릴 일이 없다.


// CRITICAL_SECTION crt;
// CSync sync(&crt);

class CSync
{
public :
	// 크리티컬 섹션을 외부에서 받아와서 세팅해줄 것이다. 
	CSync(CRITICAL_SECTION* Crt)
	{
		m_Crt = Crt;
		EnterCriticalSection(m_Crt);
	}
	~CSync()
{
		LeaveCriticalSection(m_Crt);
}
private :
	CRITICAL_SECTION* m_Crt;
};

