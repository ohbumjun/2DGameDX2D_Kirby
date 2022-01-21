#include "Thread.h"

CThread::CThread() :
	m_Thread(0),
	m_StartEvent(0)
{}

CThread::~CThread()
{
	if (m_StartEvent)
	{
		CloseHandle(m_StartEvent); // Usage Count값 감소 + 이벤트 오브젝트 소멸
		m_StartEvent = 0;
	}
}

bool CThread::Init()
{
	// 자동 이벤트 동기화 기법 사용 --> 쓰레드가 하나씩 빠져나오고 , 빠져나올 때 알아서
	// 해당 이벤트를 Non-Signaled로 다시 바꿔준다.
	// ex) 쉽게 말하면, 운영체제가 해당 Thread를 Signaled로 바꿔주면, 빠져나오는 방식
	m_StartEvent = CreateEvent(
		nullptr, // 보안 속성 -> 핸들을 자식 프로세스에게 상속할 때 Null외의 값 지정 
		FALSE,  // true 면, 수동, false면 자동 리셋 모드 이벤트
		FALSE,  // 이벤트 오브젝트 초기 상태 : true면 signaled, false면 non-signaled 
		nullptr); // 이벤트 오브젝트에 이름을 주기

	// Thread 하나를 생성한다.
	//ThreadFunction 함수를 main으로 실행한다.
	// 생성되자마자 ThreadFunction 함수를 실행하게 된다
	m_Thread = (HANDLE)_beginthreadex(
		nullptr, 
		0, 
		CThread::ThreadFunction,
		this,  // 해당 함수에 전달할 인자 
		0, // 생성과 동시에 일시정지 or 바로 시작 --> 0으로 지정하면 바로 시작 
		nullptr);

	return true;
}

void CThread::Start()
{
	// 해당 Event의 상태를 Signaled로 변경 --> Thread->Run(); 가 실행될 것이다.
	SetEvent(m_StartEvent);
}

void CThread::Pause()
{
	// SuspendThread, ResumeThread는 DWORD 타입(unsigned long) 을 return 한다.
	// 내부적으로 Suspend Count라는 값을 지니고 있다
	// Suspend Thread 호출 수만큼, Resume Thread도 호출해야만 한다
	// 두 함수 호출 횟수가 맞지 않으면, 데드락이 발생할 수 있다.
	// 따라서 아래와 같이 do while 세팅을 통해 무조건 Suspend Count가 정상적으로 세팅되게 하는 것이다

	// 내부적으로 저장된 Suspend Count값을 조절하기 위해 해당 변수 세팅
	// Suspend Count값을 받아올 것이다.
	DWORD Count = 0;

	// do, while은 무조건 한번은 호출된다.
	do
	{
		Count = SuspendThread(m_Thread); // Count값을 증가시킨다.
	} while (Count <= 0);

}

void CThread::Resume()
{
	DWORD Count = 0;

	do
	{
		Count = ResumeThread(m_Thread); // Count 값 감소 
	} while (Count >= 0);

}

void CThread::WaitStartEvent()
{
	WaitForSingleObject(m_StartEvent, INFINITE);
}

unsigned CThread::ThreadFunction(void* Arg)
{
	// Thread를 복사 생성한다.
	CThread* Thread = (CThread*)Arg;

	// 바로 해당 m_StartEvent가 Signaled 상태가 될 때까지 기다린다
	Thread->WaitStartEvent();

	// 해당 m_StartEvent가 Signaled가 되면 아래의 함수 실행
	Thread->Run();

	return 0;
}
