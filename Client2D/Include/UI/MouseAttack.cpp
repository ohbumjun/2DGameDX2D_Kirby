#include "MouseAttack.h"

CMouseAttack::CMouseAttack()
{}

CMouseAttack::CMouseAttack(const CMouseAttack& Attack)
{}

CMouseAttack::~CMouseAttack()
{}

bool CMouseAttack::Init()
{
	if (!CUIWindow::Init())
		return false;

	std::vector<TCHAR*> vecFileName;

	for (int i = 0; i < 7; i++)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(FileName, TEXT("Mouse/Temp/Attack/000%d.png"), i);
		vecFileName.push_back(FileName);
	}

	m_Image = CreateUIWidget<CUIImage>("MouseAttack");
	m_Image->SetTexture("MouseAttack", vecFileName);

	float Width = (float)m_Image->GetTexture()->GetWidth();
	float Height = (float)m_Image->GetTexture()->GetHeight();

	m_Image->SetSize(Width, Height);

	for (int i = 0; i < 7; i++)
	{
		SAFE_DELETE(vecFileName[i]);
	}

	return true;
}

CMouseAttack* CMouseAttack::Clone()
{
	return new CMouseAttack(*this);
}
