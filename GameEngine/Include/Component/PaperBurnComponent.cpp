#include "PaperBurnComponent.h"

CPaperBurnComponent::CPaperBurnComponent()
{
	m_CBuffer = new CPaperBurnConstantBuffer;
	m_CBuffer->Init();
}

CPaperBurnComponent::CPaperBurnComponent(const CPaperBurnComponent& Component) :
CObjectComponent(Component)
{
	m_CBuffer = new CPaperBurnConstantBuffer;
	m_CBuffer->Init();
}

CPaperBurnComponent::~CPaperBurnComponent()
{}

void CPaperBurnComponent::Start()
{
	CObjectComponent::Start();
}

bool CPaperBurnComponent::Init()
{
	if (!CObjectComponent::Init())
		return false;



	return true;
}

void CPaperBurnComponent::Update(float DeltaTime)
{
	CObjectComponent::Update(DeltaTime);
}

void CPaperBurnComponent::PostUpdate(float DeltaTime)
{
	CObjectComponent::PostUpdate(DeltaTime);
}

void CPaperBurnComponent::PrevRender()
{
	CObjectComponent::PrevRender();
}

void CPaperBurnComponent::Render()
{
	CObjectComponent::Render();
}

void CPaperBurnComponent::PostRender()
{
	CObjectComponent::PostRender();
}

CPaperBurnComponent* CPaperBurnComponent::Clone()
{
	return new CPaperBurnComponent(*this);
}

