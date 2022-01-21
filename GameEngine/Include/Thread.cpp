#include "Thread.h"

CThread::CThread() :
	m_Thread(0),
	m_StartEvent(0)
{}

CThread::~CThread()
{
	if (m_StartEvent)
	{
		CloseHandle(m_StartEvent); // Usage Count�� ���� + �̺�Ʈ ������Ʈ �Ҹ�
		m_StartEvent = 0;
	}
}

bool CThread::Init()
{
	// �ڵ� �̺�Ʈ ����ȭ ��� ��� --> �����尡 �ϳ��� ���������� , �������� �� �˾Ƽ�
	// �ش� �̺�Ʈ�� Non-Signaled�� �ٽ� �ٲ��ش�.
	// ex) ���� ���ϸ�, �ü���� �ش� Thread�� Signaled�� �ٲ��ָ�, ���������� ���
	m_StartEvent = CreateEvent(
		nullptr, // ���� �Ӽ� -> �ڵ��� �ڽ� ���μ������� ����� �� Null���� �� ���� 
		FALSE,  // true ��, ����, false�� �ڵ� ���� ��� �̺�Ʈ
		FALSE,  // �̺�Ʈ ������Ʈ �ʱ� ���� : true�� signaled, false�� non-signaled 
		nullptr); // �̺�Ʈ ������Ʈ�� �̸��� �ֱ�

	// Thread �ϳ��� �����Ѵ�.
	//ThreadFunction �Լ��� main���� �����Ѵ�.
	// �������ڸ��� ThreadFunction �Լ��� �����ϰ� �ȴ�
	m_Thread = (HANDLE)_beginthreadex(
		nullptr, 
		0, 
		CThread::ThreadFunction,
		this,  // �ش� �Լ��� ������ ���� 
		0, // ������ ���ÿ� �Ͻ����� or �ٷ� ���� --> 0���� �����ϸ� �ٷ� ���� 
		nullptr);

	return true;
}

void CThread::Start()
{
	// �ش� Event�� ���¸� Signaled�� ���� --> Thread->Run(); �� ����� ���̴�.
	SetEvent(m_StartEvent);
}

void CThread::Pause()
{
	// SuspendThread, ResumeThread�� DWORD Ÿ��(unsigned long) �� return �Ѵ�.
	// ���������� Suspend Count��� ���� ���ϰ� �ִ�
	// Suspend Thread ȣ�� ����ŭ, Resume Thread�� ȣ���ؾ߸� �Ѵ�
	// �� �Լ� ȣ�� Ƚ���� ���� ������, ������� �߻��� �� �ִ�.
	// ���� �Ʒ��� ���� do while ������ ���� ������ Suspend Count�� ���������� ���õǰ� �ϴ� ���̴�

	// ���������� ����� Suspend Count���� �����ϱ� ���� �ش� ���� ����
	// Suspend Count���� �޾ƿ� ���̴�.
	DWORD Count = 0;

	// do, while�� ������ �ѹ��� ȣ��ȴ�.
	do
	{
		Count = SuspendThread(m_Thread); // Count���� ������Ų��.
	} while (Count <= 0);

}

void CThread::Resume()
{
	DWORD Count = 0;

	do
	{
		Count = ResumeThread(m_Thread); // Count �� ���� 
	} while (Count >= 0);

}

void CThread::WaitStartEvent()
{
	WaitForSingleObject(m_StartEvent, INFINITE);
}

unsigned CThread::ThreadFunction(void* Arg)
{
	// Thread�� ���� �����Ѵ�.
	CThread* Thread = (CThread*)Arg;

	// �ٷ� �ش� m_StartEvent�� Signaled ���°� �� ������ ��ٸ���
	Thread->WaitStartEvent();

	// �ش� m_StartEvent�� Signaled�� �Ǹ� �Ʒ��� �Լ� ����
	Thread->Run();

	return 0;
}
