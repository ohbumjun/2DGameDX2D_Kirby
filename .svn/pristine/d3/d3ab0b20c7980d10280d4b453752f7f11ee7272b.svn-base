
#include "RenderStateManager.h"
#include "BlendState.h"

CRenderStateManager::CRenderStateManager()
{
}

CRenderStateManager::~CRenderStateManager()
{
}

bool CRenderStateManager::Init()
{
	AddBlendInfo("AlphaBlend");
	CreateBlendState("AlphaBlend", true, false);

	return true;
}

void CRenderStateManager::SetBlendFactor(const std::string& Name,
	float r, float g, float b, float a)
{
	CBlendState* State = (CBlendState*)FindRenderState(Name);

	if (!State)
	{
		State = new CBlendState;

		State->SetName(Name);

		m_mapRenderState.insert(std::make_pair(Name, State));
	}

	State->SetBlendFactor(r, g, b, a);
}

void CRenderStateManager::AddBlendInfo(const std::string& Name, 
	bool BlendEnable, D3D11_BLEND SrcBlend,
	D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp, D3D11_BLEND SrcBlendAlpha,
	D3D11_BLEND DestBlendAlpha, D3D11_BLEND_OP BlendOpAlpha, 
	UINT8 RenderTargetWriteMask)
{
	CBlendState* State = (CBlendState*)FindRenderState(Name);

	if (!State)
	{
		State = new CBlendState;

		State->SetName(Name);

		m_mapRenderState.insert(std::make_pair(Name, State));
	}

	State->AddBlendInfo(BlendEnable, SrcBlend, DestBlend, BlendOp,
		SrcBlendAlpha, DestBlendAlpha, BlendOpAlpha, RenderTargetWriteMask);
}

bool CRenderStateManager::CreateBlendState(const std::string& Name, bool AlphaToCoverageEnable,
	bool IndependentBlendEnable)
{
	CBlendState* State = (CBlendState*)FindRenderState(Name);

	if (!State)
		return false;

	if (!State->CreateState(AlphaToCoverageEnable, IndependentBlendEnable))
	{
		SAFE_RELEASE(State);
		return false;
	}

	return true;
}

CRenderState* CRenderStateManager::FindRenderState(const std::string& Name)
{
	auto	iter = m_mapRenderState.find(Name);

	if (iter == m_mapRenderState.end())
		return nullptr;

	return iter->second;
}
