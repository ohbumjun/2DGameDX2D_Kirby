
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

	Desc.BlendEnable = BlendEnable; // �ȼ� ���̴��� ����, ���� Ÿ���� ���� ���� ó���� ���� �������� �����ϸ�, ���� Ÿ�ٸ��� ������ �� �ִ�. 
	Desc.SrcBlend = SrcBlend;
	Desc.DestBlend = DestBlend;
	Desc.BlendOp = BlendOp;
	Desc.SrcBlendAlpha = SrcBlendAlpha;
	Desc.DestBlendAlpha = DestBlendAlpha;
	Desc.BlendOpAlpha = BlendOpAlpha;
	// ���� Ÿ�ٿ� � ���� �� ���ΰ��� ���� --> �� ���� Ÿ�ٸ��� ���� ����, rgba ��� ����, D3D11_COLOR_WRITE_ENABLE_ALL�� ����Ѵ�. 
	Desc.RenderTargetWriteMask = RenderTargetWriteMask; 

	m_vecDesc.push_back(Desc);
}

bool CBlendState::CreateState(bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	if (m_vecDesc.empty())
		return false;

	D3D11_BLEND_DESC	Desc = {};

	Desc.AlphaToCoverageEnable = AlphaToCoverageEnable; // ����Ÿ�ٿ� �ȼ����� ������ �� ���İ��� ����� ���ΰ� 
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
		(ID3D11BlendState*)m_State,  // ������ ID3D11BlendState �������̽� 
		m_BlendFactor,  // r,g,b,a ����� 
		m_SampleMask);
}

void CBlendState::ResetState()
{
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)m_PrevState, m_PrevBlendFactor, m_PrevSampleMask);

	SAFE_RELEASE(m_PrevState);
}
