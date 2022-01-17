#include "Progressbar.h"
#include "../Scene/SceneResource.h"
#include "../Scene/ViewPort.h"
#include "../Scene/Scene.h"

CProgressbarConstantBuffer* m_ProgressBarCBuffer;
WidgetImageInfo m_Info;
std::function<void()> m_MouseOnCallback;
float m_Percent;
bool m_StartMouseOn;
ProgressBar_Dir m_Dir;

CProgressbar::CProgressbar() :
m_Percent(1.f),
m_MouseOnCallback(nullptr),
m_Dir(ProgressBar_Dir::RightToLeft),
m_ProgressBarCBuffer(nullptr),
m_StartMouseOn(false)
{}

CProgressbar::CProgressbar(const CProgressbar& Bar) :
CUIWidget(Bar)
{
	m_Info = Bar.m_Info;
	m_Percent = Bar.m_Percent;
	m_Dir = Bar.m_Dir;

	m_MouseOnCallback = nullptr;
	m_StartMouseOn = false;

	m_ProgressBarCBuffer = new CProgressbarConstantBuffer;
	m_ProgressBarCBuffer->Init();

	m_ProgressBarCBuffer->SetPercent(m_Percent);
	m_ProgressBarCBuffer->SetDir(m_Dir);
}

CProgressbar::~CProgressbar()
{
	SAFE_DELETE(m_ProgressBarCBuffer);
}

void CProgressbar::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CSceneResource* Resource = m_Owner->GetViewPort()->GetScene()->GetResource();
	Resource->LoadTexture(Name, FileName, PathName);
	m_Info.m_Texture = Resource->FindTexture(Name);
	if (m_Info.m_Texture)
		SetUseTexture(true);
}

void CProgressbar::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CSceneResource* Resource = m_Owner->GetViewPort()->GetScene()->GetResource();
	Resource->LoadTextureFullPath(Name, FullPath);
	m_Info.m_Texture = Resource->FindTexture(Name);
	if (m_Info.m_Texture)
		SetUseTexture(true);
}

void CProgressbar::AddAnimationFrameData(const Vector2& Start, const Vector2& Size)
{
	AnimationFrameData Anim = {};
	Anim.Start = Start;
	Anim.Size = Size;
	m_Info.m_vecFrameData.push_back(Anim);
}

void CProgressbar::SetTextureTint(const Vector4& Tint)
{
	m_Info.m_Tint = Tint;
}

void CProgressbar::SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_Info.m_Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CProgressbar::Start()
{
	CUIWidget::Start();
}

bool CProgressbar::Init()
{
	if (!CUIWidget::Init())
		return false;

	m_Shader = m
}

void CProgressbar::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);
}

void CProgressbar::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);
}

void CProgressbar::Render()
{
	CUIWidget::Render();
}

CProgressbar* CProgressbar::Clone()
{
	return CUIWidget::Clone();
}
