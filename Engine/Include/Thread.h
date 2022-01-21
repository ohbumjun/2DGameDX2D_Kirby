#pragma once

#include "GameInfo.h"

// ���� ��� : ũ��Ƽ�� ���� + Ŀ�� ��� : �̺�Ʈ ����ȭ ���
// <process.h>�� �ʿ��ϴ�. 

class CThread
{
protected:
	CThread();
	virtual ~CThread(); // ��ӿ�
private :
	std::string m_Name; // ������� ���Ǹ� ���� Thread�� ��Ƶ� ���̴�.
	HANDLE m_Thread;  // �ش� Thread�� �ڵ鰪 --> Thread �ĺ��� ���� , �ڵ� ���̺� ����
	HANDLE m_StartEvent; // �̺�Ʈ ����ȭ ��� �������� ���� Event�� �ڵ鰪 
public :
	bool Init();
	virtual void Run();
public :
	// Thread�� ���¸� ���� �������ֱ� ���� �Լ� ( ��� ���� �Ⱦ��� �Ѵ� )
	void Start();
	void Pause();
	void Resume();
};

