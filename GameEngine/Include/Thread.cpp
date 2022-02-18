#include "Thread.h"

CThread::CThread()
{}

CThread::~CThread()
{}

bool CThread::Init()
{
	m_StartEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	m_Thread = (HANDLE)_beginthreadex(NULL, 0, &CThread::ThreadFunction, this, 0, nullptr);

	return true;
}

void CThread::Run()
{}

void CThread::Start()
{
	SetEvent(m_StartEvent);
}

void CThread::Pause()
{
	DWORD Count;

	do
	{
		Count = SuspendThread(m_Thread);
	} while (Count <= 0);
}

void CThread::Resume()
{
	DWORD Count;

	do
	{
		Count = SuspendThread(m_Thread);
	} while (Count <= 0);
}

void CThread::WaitStartEvent()
{
	WaitForSingleObject(m_StartEvent, INFINITE);
}

unsigned CThread::ThreadFunction(void* Arg)
{
	CThread* Thread = (CThread*)Arg;

	Thread->WaitStartEvent();

	Thread->Run();

	return 0;
}
