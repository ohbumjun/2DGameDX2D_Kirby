#pragma once

#include "GameInfo.h"

// ũ��Ƽ�� ���� ����ȭ ����� ����Ϸ���
// ũ��Ƽ�� ���� ������Ʈ�� �����, �ʱ�ȭ �ؾ� �Ѵ�.
// CRITICAL_SECTION ������ �� ũ��Ƽ�� ���Ǳ�� ����ȭ ����̰�, ȭ��� Ű�� ����

// void InitializeCriticalSection(); �Լ��� ���� �ܺο��� �ʱ�ȭ ������ �־��� ���̰�
// �� �������� ���� CRITICAL_SECTION �� CSync ��ü�� ������ ���ڷ� �������ִ� ���̴�.

// EnterCriticalSection �� �� �Ӱ迵���� ���� ��,
// LeaveCriticalSection �� �� �Ӱ� ������ ������ ��
// �����ڿ� �Ҹ��ڿ� �����صΰ� �Ǹ�, �Ӱ迵���� ���� �� LeaveCriticalSection��
// �ݵ�� ���ֹǷ� DeadLock�� �ɸ� ���� ����.


// CRITICAL_SECTION crt;
// CSync sync(&crt);

class CSync
{
public :
	// ũ��Ƽ�� ������ �ܺο��� �޾ƿͼ� �������� ���̴�. 
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

