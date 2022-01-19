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

	m_Image = CreateUIWidget<CUIImage>("Image");

	std::vector<TCHAR*> vecFileName;

	for (int i = 0; i <= 12; ++i)
	{
		TCHAR *FilePath = new TCHAR[MAX_PATH];
		memset(FilePath, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(FilePath, TEXT("Mouse/Default/%d.png"), i);
		vecFileName.push_back(FilePath);
	}

	m_Image->SetTexture("MouseNormal", vecFileName);
	m_Image->AddAnimationFrameData(13);

	// Mouse Image 실제 크기에 맞추었다.
	float Width  = (float)m_Image->GetTexture()->GetWidth();
	float Height = (float)m_Image->GetTexture()->GetHeight();
	SetSize(Width, Height);

	for (int i = 0; i < 13; i++)
	{
		SAFE_DELETE(vecFileName[i]);
	}

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
