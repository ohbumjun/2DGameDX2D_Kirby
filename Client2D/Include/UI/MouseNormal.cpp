#include "MouseNormal.h"

CMouseNormal::CMouseNormal()
{}

CMouseNormal::CMouseNormal(const CMouseNormal& window)
{
	m_Image = FindUIWidget<CUIImage>("Image");
}

CMouseNormal::~CMouseNormal()
{}

void CMouseNormal::Start()
{
	CUIWindow::Start();
}

bool CMouseNormal::Init()
{
	if (!CUIWindow::Init())
		return false;

	// Mouse Image 실제 크기에 맞추었다.
	SetSize(32.f, 31.f);

	m_Image = CreateUIWidget<CUIImage>("Image");

	std::vector<TCHAR*> vecFileName;

	for (int i = 0; i <= 12; ++i)
	{
		TCHAR FilePath[MAX_PATH] = {};
		wsprintf(FilePath, TEXT("Mouse/Default/%d.png"), i);
		vecFileName.push_back(FilePath);
	}

	m_Image->SetTexture("MouseNormal", vecFileName);

	return true;
}

void CMouseNormal::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CMouseNormal::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CMouseNormal::Render()
{
	CUIWindow::Render();
}

CMouseNormal* CMouseNormal::Clone()
{
	return new CMouseNormal(*this);
}
