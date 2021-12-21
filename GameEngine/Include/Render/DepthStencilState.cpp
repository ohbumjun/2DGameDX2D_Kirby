#include "DepthStencilState.h"
#include "../Device.h"

CDepthStencilState::CDepthStencilState() :
	m_StencilRef(0xffffffff),
	m_PrevStencilRef(0xffffffff)
{
}

CDepthStencilState::~CDepthStencilState()
{
}

// ���� �׽�Ʈ, ���ٽ� �׽�Ʈ�� �ʱ�ȭ �Ѵ�. 
bool CDepthStencilState::CreateState(bool                       DepthEnable, D3D11_DEPTH_WRITE_MASK   DepthWriteMAsk,
                                     D3D11_COMPARISON_FUNC      DepthFunc, bool                       StencilEnable,
                                     UINT8                      StencilReadMask, UINT8                StencilWriteMask,
                                     D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	D3D11_DEPTH_STENCIL_DESC Desc = {};

	// ���� �׽�Ʈ Ȱ��ȭ ���� 
	Desc.DepthEnable = DepthEnable;
	// ���� 
	Desc.DepthWriteMask = DepthWriteMAsk;
	// ���� ��ü ������ 
	Desc.DepthFunc = DepthFunc;
	// ���Ľ� �׽�Ʈ Ȱ��ȭ 
	Desc.StencilEnable = StencilEnable;
	// ���ٽ� �б�
	Desc.StencilReadMask = StencilReadMask;
	// ���Ľ� ���� 
	Desc.StencilWriteMask = StencilWriteMask;
	Desc.FrontFace        = FrontFace;
	Desc.BackFace         = BackFace;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateDepthStencilState(&Desc,
		           (ID3D11DepthStencilState**)&m_State)))
		return false;

	return true;
}

void CDepthStencilState::SetState()
{
	CDevice::GetInst()->GetContext()->
	                    OMGetDepthStencilState((ID3D11DepthStencilState**)&m_PrevState, &m_PrevStencilRef);
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState(static_cast<ID3D11DepthStencilState*>(m_State),
	                                                         m_StencilRef);
}

void CDepthStencilState::ResetState()
{
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState(static_cast<ID3D11DepthStencilState*>(m_PrevState),
	                                                         m_PrevStencilRef);
	SAFE_RELEASE(m_PrevState);
}
