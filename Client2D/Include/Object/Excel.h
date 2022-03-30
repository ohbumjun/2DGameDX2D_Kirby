#pragma once
#include "GameInfo.h"

class CExcel
{
private :
	CExcel();
	~CExcel();
public :
	void LoadExcel();
	void SaveExcel();
	// SingleTon
private :
	static CExcel* m_Inst;
public :
	static CExcel* GetInst();

	void           DestroyInst();
};


