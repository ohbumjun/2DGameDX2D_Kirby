
#include "BlendState.h"
#include "../Device.h"

CBlendState::CBlendState()	:
	m_SampleMask(0xffffffff),
	m_PrevSampleMask(0),
	m_BlendFactor{},
	m_PrevBlendFactor{}
{
	m_Type = RenderState_Type::Blend;
}

CBlendState::~CBlendState()
{
}

void CBlendState::AddBlendInfo(bool BlendEnable, D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend, 
	D3D11_BLEND_OP BlendOp, D3D11_BLEND SrcBlendAlpha, D3D11_BLEND DestBlendAlpha, D3D11_BLEND_OP BlendOpAlpha,
	UINT8 RenderTargetWriteMask)
{
	D3D11_RENDER_TARGET_BLEND_DESC	Desc = {};

	Desc.BlendEnable = BlendEnable; // 픽셀 셰이더의 값과, 렌더 타겟의 값의 블렌드 처리를 할지 안할지를 설정하며, 렌더 타겟마다 설정할 수 있다. 
	Desc.SrcBlend = SrcBlend;
	Desc.DestBlend = DestBlend;
	Desc.BlendOp = BlendOp;
	Desc.SrcBlendAlpha = SrcBlendAlpha;
	Desc.DestBlendAlpha = DestBlendAlpha;
	Desc.BlendOpAlpha = BlendOpAlpha;
	// 렌더 타겟에 어떤 색을 쓸 것인가를 결정 --> 각 렌더 타겟마다 설정 가능, rgba 모두 사용시, D3D11_COLOR_WRITE_ENABLE_ALL을 사용한다. 
	Desc.RenderTargetWriteMask = RenderTargetWriteMask; 

	m_vecDesc.push_back(Desc);
}

bool CBlendState::CreateState(bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	if (m_vecDesc.empty())
		return false;

	D3D11_BLEND_DESC	Desc = {};

	Desc.AlphaToCoverageEnable = AlphaToCoverageEnable; // 렌더타겟에 픽셀값을 설정할 때 알파값을 사용할 것인가 
	Desc.IndependentBlendEnable = IndependentBlendEnable;

	memcpy(Desc.RenderTarget, &m_vecDesc[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * m_vecDesc.size());

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBlendState(&Desc, (ID3D11BlendState**)&m_State)))
		return false;

	return true;
}

void CBlendState::SetState()
{
	CDevice::GetInst()->GetContext()->OMGetBlendState(
		(ID3D11BlendState**)&m_PrevState, 
		m_PrevBlendFactor, 
		&m_PrevSampleMask);

	CDevice::GetInst()->GetContext()->OMSetBlendState(
		(ID3D11BlendState*)m_State,  // 설정할 ID3D11BlendState 인터페이스 
		m_BlendFactor,  // r,g,b,a 상수값 
		m_SampleMask);
}

void CBlendState::ResetState()
{
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)m_PrevState, m_PrevBlendFactor, m_PrevSampleMask);

	SAFE_RELEASE(m_PrevState);
}
