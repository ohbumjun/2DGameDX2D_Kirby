#include "PaperBurnComponent.h"
#include "../Resource/Material/Material.h"
#include "../Scene/Scene.h"


CPaperBurnComponent::CPaperBurnComponent()
{
	m_CBuffer = new CPaperBurnConstantBuffer;
	m_CBuffer->Init();

	m_FinishTime = 5.f;

	m_StartPaperBurn = false;

	m_Filter = 0.f;
}

CPaperBurnComponent::CPaperBurnComponent(const CPaperBurnComponent& Component) :
CObjectComponent(Component)
{
	m_CBuffer = Component.m_CBuffer->Clone();

	m_FinishTime = Component.m_FinishTime;

	m_Material = Component.m_Material;

	m_StartPaperBurn = false;

	m_Filter = 0.f;
}

CPaperBurnComponent::~CPaperBurnComponent()
{
	SAFE_DELETE(m_CBuffer);

	if (m_Material)
		m_Material->DeleteRenderCallback(this);
}

void CPaperBurnComponent::StartPaperBurn()
{
	if (!m_Material || !m_StartPaperBurn)

	m_Material->SetPaperBurn(true);
	m_StartPaperBurn = true;
}

void CPaperBurnComponent::SetMaterial(CMaterial* Material)
{
	m_Material = Material;
	m_Material->AddRenderCallback(this, &CPaperBurnComponent::SetShader);
}

void CPaperBurnComponent::SetInverse(bool Enable)
{
	m_CBuffer->SetInverse(Enable);
}

void CPaperBurnComponent::SetBurnTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
		return;

	m_BurnTexture = m_Scene->GetResource()->FindTexture(Name);
}

void CPaperBurnComponent::SetOutLineColor(const Vector4& Color)
{
	m_CBuffer->SetOutLineColor(Color);
}

void CPaperBurnComponent::SetOutLineColor(float r, float g, float b, float a)
{
	m_CBuffer->SetOutLineColor(r, g, b, a);
}

void CPaperBurnComponent::SetCenterLineColor(const Vector4& Color)
{
	m_CBuffer->SetCenterLineColor(Color);
}

void CPaperBurnComponent::SetCenterLineColor(float r, float g, float b, float a)
{
	m_CBuffer->SetCenterLineColor(r, g, b, a);
}

void CPaperBurnComponent::SetInLineColor(const Vector4& Color)
{
	m_CBuffer->SetInLineColor(Color);
}

void CPaperBurnComponent::SetInLineColor(float r, float g, float b, float a)
{
	m_CBuffer->SetInLineColor(r, g, b, a);
}

void CPaperBurnComponent::SetInFilter(float Filter)
{
	m_CBuffer->SetInFilter(Filter);
}

void CPaperBurnComponent::SetCenterFilter(float Filter)
{
	m_CBuffer->SetCenterFilter(Filter);
}

void CPaperBurnComponent::SetOutFilter(float Filter)
{
	m_CBuffer->SetOutFilter(Filter);
}

void CPaperBurnComponent::SetShader()
{
	m_CBuffer->UpdateCBuffer();
	m_BurnTexture->SetShader(101, (int)Buffer_Shader_Type::Pixel, 0);
}

void CPaperBurnComponent::Start()
{
	CObjectComponent::Start();
}

bool CPaperBurnComponent::Init()
{
	// TextureManager 에서 세팅되어 있는 기본 Texture
	SetBurnTexture("DefaultBurnTexture", TEXT("DefaultPaperBurn.png"));

	return true;
}

void CPaperBurnComponent::Update(float DeltaTime)
{
	if (m_StartPaperBurn)
	{
		// 0 ~ FinishTime 동안, m_Filter 값이 0에서 1 로 증가할 것이다
		m_Filter += DeltaTime / m_FinishTime;

		if (m_Filter >= 1.f)
		{
			m_Filter = 1.f;

			if (m_FinishCallback)
				m_FinishCallback();
		}

 		m_CBuffer->SetFilter(m_Filter);
	}
}

void CPaperBurnComponent::PostUpdate(float DeltaTime)
{
}

void CPaperBurnComponent::PrevRender()
{
}

void CPaperBurnComponent::Render()
{
}

void CPaperBurnComponent::PostRender()
{
}

CPaperBurnComponent* CPaperBurnComponent::Clone()
{
	return new CPaperBurnComponent(*this);
}

